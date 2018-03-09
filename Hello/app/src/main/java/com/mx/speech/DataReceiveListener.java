package com.mx.speech;

import android.media.AudioFormat;
import android.media.AudioRecord;
import android.media.MediaPlayer;
import android.media.MediaRecorder;
import android.os.Environment;
import android.os.Process;
import android.util.Log;

import java.io.File;
import java.io.IOException;

import utils.ChangeTool;
import utils.SerialPortUtils;

/**
 * Created by peak on 18-3-1.
 */

public class DataReceiveListener implements SerialPortUtils.OnDataReceiveListener{

    private final static String TAG = "DataReceiveListener";

    private boolean mSpeaking;
    private boolean mIsRecording=false;

    private  int mMinBufferSize;
    private AudioRecord mRecorder;

    private void initRecord(){
        mMinBufferSize = AudioRecord.getMinBufferSize(16000, AudioFormat.CHANNEL_IN_MONO, AudioFormat.ENCODING_PCM_16BIT);
        mRecorder = new AudioRecord(MediaRecorder.AudioSource.MIC, 16000, AudioFormat.CHANNEL_IN_MONO, AudioFormat.ENCODING_PCM_16BIT, mMinBufferSize * 2);
    }

    // 开始录音，并且实时检测当前是否有人在讲话（实时静音检测）。
    private void startRecord() {
        mIsRecording = true;
        new Thread(new Runnable() {
            @Override
            public void run() {

                try {
                    Process.setThreadPriority(Process.THREAD_PRIORITY_URGENT_AUDIO);
                    int readSize;
                    mMinBufferSize = 320;
                    short [] audioData=new short[mMinBufferSize];
                    if (mRecorder.getState() != AudioRecord.STATE_INITIALIZED) {
                        stopRecord();
                        return;
                    }
                    mRecorder.startRecording();

                    while (mIsRecording) {
                        if (null != mRecorder) {
                            readSize = mRecorder.read(audioData, 0, mMinBufferSize);

                            if (readSize == AudioRecord.ERROR_INVALID_OPERATION || readSize == AudioRecord.ERROR_BAD_VALUE) {
                                continue;
                            }
                            if (readSize != 0 && readSize != -1) {
                                //在此可以对录制音频的数据进行二次处理 比如变声，压缩，降噪，增益等操作
                                // 当前对PCM录音数据进行VAD检测（语音活动检测）。
                                mSpeaking = webRtcVad_Process( audioData, 0,readSize);
                                if (mSpeaking) {
                                    Log.d("tanyaping", "---正在讲话-------");
                                } else {
                                    Log.d("tanyaping", "---当前无声音-----");
                                }
                            } else {
                                break;
                            }
                        }
                    }

                } catch (Exception e) {
                    e.printStackTrace();
                }

            }
        }).start();
    }

    private void stopRecord(){
        mIsRecording=false;
        if(mRecorder!=null) {
            mRecorder.stop();
        }
    }


    public void onDataReceive(byte[] buffer, int size) {
        Log.d(TAG, "接收数据---" + "大小：" + String.valueOf(size) + ", 内容：" + ChangeTool.ByteArrToHex(buffer));
        if(size == 12){
            String data = new String(buffer, 0, size);
            if(data.startsWith("wakeup:")){
                Log.d(TAG, "接收到数据：" + data);

                initRecord();
                startRecord();

                Log.d(TAG, "麦克风已被激活!");
            }

        }

    }

    /**
     * 语音活动检测。add by tanyaping
     * @param audioData
     * @param offsetInshort
     * @param readSize
     * @return
     */
    public native boolean webRtcVad_Process(short[] audioData,int offsetInshort,int readSize);


}
