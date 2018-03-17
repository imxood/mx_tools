#ifndef FRMMAIN_H
#define FRMMAIN_H

#include <QWidget>
#include <QMouseEvent>
#include "posix_qextserialport.h"

namespace Ui {
    class frmMain;
}

class frmMain : public QWidget
{
    Q_OBJECT

public:
    explicit frmMain(QWidget *parent = 0);
    ~frmMain();

protected:
    void mouseDoubleClickEvent(QMouseEvent *e);//监听鼠标双击事件
    bool eventFilter(QObject *obj, QEvent *event);//监听事件

private:
    Ui::frmMain *ui;

    Posix_QextSerialPort *myCom;
    QTimer *myTimer;
    QTimer *myReadTimer;//读取串口数据
    QTimer *mySendTimer;//发送串口数据
    QTimer *mySaveTimer;//保存串口数据

    int SendCount;//发送数据计数
    int ReceiveCount;//接收数据计数
    bool IsShow;//是否显示数据

    bool IsAutoSend;//是否自动发送
    bool IsAutoSave;//是否自动保存
    bool IsAutoClear;//是否自动清空

    bool IsInit;//是否完成了初始化

private:
    void InitForm();//初始化界面以及其他处理
    void ChangeEnable(bool b);//改变状态

    QByteArray GetHexValue(QString str);//格式化16进制数据,参数为字符串
    char ConvertHexChar(char ch);

private slots:
    void on_cboxSave_currentIndexChanged(int index);
    void on_cboxSend_currentIndexChanged(int index);
    void on_btnData_clicked();
    void on_btnSend_clicked();
    void on_btnSave_clicked();
    void on_btnClearAll_clicked();
    void on_btnStopShow_clicked();
    void on_btnClearReceive_clicked();
    void on_btnClearSend_clicked();
    void on_ckIsAutoSave_stateChanged(int );
    void on_ckIsAutoSend_stateChanged(int );
    void on_ckIsAutoClear_stateChanged(int );
    void on_btnOpen_clicked();
    void GetDateTime();//动态显示时间

    void ReadMyCom();//读取串口数据
    void WriteMyCom();//写串口数据
    void SaveMyCom();//保存串口数据    

    void SendData();//发送数据
    void SaveData();//保存数据

    void ReadTxtData();//读取配置文件数据
};

#endif // FRMMAIN_H
