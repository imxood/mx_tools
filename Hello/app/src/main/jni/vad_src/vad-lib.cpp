#include <jni.h>
#include <string>
#include <malloc.h>
#include "webrtc_vad.h"
#include "vad_core.h"

//extern "C"
//JNIEXPORT jstring JNICALL
//Java_com_tanyaping_audio_vadtest_MainActivity_stringFromJNI(
//        JNIEnv *env,
//        jobject /* this */) {
//    std::string hello = "Hello from C++";
//    return env->NewStringUTF(hello.c_str());
//}



extern "C"
JNIEXPORT jboolean JNICALL
Java_com_mx_speech_DataReceiveListener_webRtcVad_1Process(JNIEnv *env, jclass thiz,
                                                                 jshortArray audioData_,
                                                                 jint offsetInshort,
                                                                 jint readSize) {

    // demo for vad 。 add by tanyaping welcome contact
    VadInst *handle = WebRtcVad_Create();
    WebRtcVad_Init(handle);
    WebRtcVad_set_mode(handle, 2);
    short speech[160] = {0};
    int index = readSize / 160;
    jshort *pcm_data = env->GetShortArrayElements(audioData_, JNI_FALSE);
    bool b;
    for (int i = 0; i < index; ++i) {
        int vad = WebRtcVad_Process(handle, 16000, pcm_data + offsetInshort + i * 160, 160);
        if (vad == 1) {
            b = JNI_TRUE;
        } else{
            b=JNI_FALSE;
        }
    }
    env->ReleaseShortArrayElements(audioData_, pcm_data, JNI_ABORT);
    WebRtcVad_Free(handle);
    return b;
}

