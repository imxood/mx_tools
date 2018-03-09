#include "frmmain.h"
#include "ui_frmmain.h"
#include "myStaticClass.h"

frmMain::frmMain(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::frmMain)
{
    ui->setupUi(this);

    FormHelper::FormInCenter(this);
    FormHelper::FormNotResize(this);

    this->setWindowIcon(QIcon(":/1.ico"));
    this->setWindowTitle(tr("串口调试工具"));

    this->InitForm();
    //安装监听器
    ui->txtSend->installEventFilter(this);

}

frmMain::~frmMain()
{
    delete ui;
}

//监听鼠标双击则清空接收数据
void frmMain::mouseDoubleClickEvent(QMouseEvent *e)
{
    if (e->button()==Qt::LeftButton)
    {
        ui->txtDataHex->clear();
    }
}

//监听回车键
bool frmMain::eventFilter(QObject *obj, QEvent *event)
{
    if (obj==ui->txtSend)
    {
        if (event->type()==QEvent::KeyPress)
        {
            QKeyEvent *keyEvent=static_cast<QKeyEvent *>(event);
            if (keyEvent->key()==Qt::Key_Return || keyEvent->key()==Qt::Key_Enter)
            {
                this->WriteMyCom();
                return true;
            }
        }
    }

    return QObject::eventFilter(obj,event);
}

void frmMain::InitForm()
{
    ReceiveCount=0;
    SendCount=0;
    this->IsShow=true;
    this->IsAutoClear=false;
    this->IsAutoSave=false;
    this->IsAutoSend=false;
    this->IsInit=false;

    QStringList comList;//串口号
    QStringList baudList;//波特率
    QStringList parityList;//校验位
    QStringList dataBitsList;//数据位
    QStringList stopBitsList;//停止位

    comList.clear();
    comList<<"/pts/23" << "ttyUSB0"<<"ttyUSB1"<<"ttyUSB2"<<"ttyUSB3"<<"left_wheel_port"<<"right_wheel_port";

    ui->cboxPortName->clear();
    ui->cboxPortName->addItems(comList);
    ui->cboxPortName->setCurrentIndex(0);

    baudList.clear();
    baudList<<"9600"<<"57600"<<"115200";
    ui->cboxBaudRate->clear();
    ui->cboxBaudRate->addItems(baudList);
    ui->cboxBaudRate->setCurrentIndex(12);

    parityList.clear();
    parityList<<"无"<<"奇"<<"偶"<<"标志"<<"空格";
    ui->cboxParity->clear();
    ui->cboxParity->addItems(parityList);
    ui->cboxParity->setCurrentIndex(0);

    dataBitsList.clear();
    dataBitsList<<"5"<<"6"<<"7"<<"8";
    ui->cboxDataBit->clear();
    ui->cboxDataBit->addItems(dataBitsList);
    ui->cboxDataBit->setCurrentIndex(3);

    stopBitsList.clear();
    stopBitsList<<"1"<<"1.5"<<"2";
    ui->cboxStopBit->clear();
    ui->cboxStopBit->addItems(stopBitsList);
    ui->cboxStopBit->setCurrentIndex(0);

    ui->cboxSend->clear();
    ui->cboxSave->clear();
    for (int i=1;i<61;i++)
    {
        ui->cboxSend->addItem(QString::number(i)+"秒");
        ui->cboxSave->addItem(QString::number(i)+"秒");
    }

    ui->cboxSave->setCurrentIndex(4);
    ui->cboxSend->setCurrentIndex(4);

    ui->cboxSend->setEnabled(false);
    ui->cboxSave->setEnabled(false);

    //读取数据
    myReadTimer=new QTimer(this);
    myReadTimer->setInterval(200);
    connect(myReadTimer,SIGNAL(timeout()),this,SLOT(ReadMyCom()));

    //发送数据
    mySendTimer=new QTimer(this);
    mySendTimer->setInterval(5000);
    connect(mySendTimer,SIGNAL(timeout()),this,SLOT(SendData()));

    //保存数据
    mySaveTimer=new QTimer(this);
    mySaveTimer->setInterval(5000);
    connect(mySaveTimer,SIGNAL(timeout()),this,SLOT(SaveData()));

    //显示日期时间
    myTimer=new QTimer(this);
    myTimer->start(1000);
    connect(myTimer,SIGNAL(timeout()),this,SLOT(GetDateTime()));

    this->ChangeEnable(false);
    this->ReadTxtData();
    this->IsInit=true;

}

void frmMain::ChangeEnable(bool b)
{
    ui->cboxBaudRate->setEnabled(!b);
    ui->cboxDataBit->setEnabled(!b);
    ui->cboxParity->setEnabled(!b);
    ui->cboxPortName->setEnabled(!b);
    ui->cboxStopBit->setEnabled(!b);

    ui->gbox2->setEnabled(b);
    ui->gbox3->setEnabled(b);    
    ui->gbox5->setEnabled(b);
    ui->gbox6->setEnabled(b);
    ui->btnOpen->setEnabled(true);

    if (b)
    {
        this->myReadTimer->start();
        this->mySaveTimer->start();
        this->mySendTimer->start();
    }
    else
    {
        this->myReadTimer->stop();
        this->mySaveTimer->stop();
        this->mySendTimer->stop();
    }
}

void frmMain::GetDateTime()
{
    QTime timeNow=QTime::currentTime();
    ui->labTime->setText(tr("时间:%1").arg(timeNow.toString()));
    ui->labDate->setText(tr("日期:%1").arg(QDate::currentDate().toString(tr("yyyy年MM月dd日 dddd"))));
}

QByteArray frmMain::GetHexValue(QString str)
{
    QByteArray senddata;
    int hexdata,lowhexdata;
    int hexdatalen = 0;
    int len = str.length();
    senddata.resize(len/2);
    char lstr,hstr;
    for(int i=0; i<len; )
    {
        hstr=str[i].toLatin1();
        if(hstr == ' ')
        {
            i++;
            continue;
        }
        i++;
        if(i >= len)
            break;
        lstr = str[i].toLatin1();
        hexdata = ConvertHexChar(hstr);
        lowhexdata = ConvertHexChar(lstr);
        if((hexdata == 16) || (lowhexdata == 16))
            break;
        else
            hexdata = hexdata*16+lowhexdata;
        i++;
        senddata[hexdatalen] = (char)hexdata;
        hexdatalen++;
    }
    senddata.resize(hexdatalen);
    return senddata;
}

char frmMain::ConvertHexChar(char ch)
{
    if((ch >= '0') && (ch <= '9'))
        return ch-0x30;
    else if((ch >= 'A') && (ch <= 'F'))
        return ch-'A'+10;
    else if((ch >= 'a') && (ch <= 'f'))
        return ch-'a'+10;
    else return (-1);
}

void frmMain::ReadMyCom()
{
    MyHelper::DelayTime(100);//延时100毫秒

    QString strHex;//16进制数据    
    QString strNomal;//正常数据
    QByteArray temp=myCom->readAll();

    if (!temp.isEmpty() && IsShow)
    {
        QDataStream out(&temp,QIODevice::ReadWrite);
        while (!out.atEnd())
        {
            qint8 outChar=0;
            out>>outChar;
            QString str=QString("%1").arg(outChar&0xFF,2,16,QLatin1Char(','));

            if (str.length()>1)
            {
                strHex+=str+" ";
            }
            else
            {
                strHex+="0"+str+" ";
            }
        }

        strNomal=temp;

        if (ui->ckHexReceive->isChecked())
        {
            ui->txtDataHex->append(strHex.toUpper());
        }
        else
        {
            ui->txtDataHex->append(strNomal);
        }

        ReceiveCount=ReceiveCount+temp.size();
        ui->labReceive->setText(tr("接收:%1 字节").arg(ReceiveCount));
    }
}

void frmMain::WriteMyCom()
{
    QString str=ui->txtSend->currentText();
    QByteArray outData=str.toLatin1();

    int size=outData.size();

    //如果串口没有打开
    if (!myCom->isOpen())
    {
        return;
    }
    //如果发送数据为空
    if (outData.isEmpty())
    {
        ui->txtSend->setFocus();
        return;
    }

    //linux下的回车符为一个字符\r
//    if (outData[size-1]!='\r')
//    {
//        outData.resize(size+1);
//        outData[size]='\r';
//    }

    //默认16进制发送不加入回车符
    if (ui->ckHexSend->isChecked())//转化为16进制发送
    {
        outData=this->GetHexValue(str);
        size=outData.size();
        myCom->write(outData);
    }
    else
    {
        size=outData.size();
        myCom->write(outData);
    }

    SendCount=SendCount+size;
    ui->labSend->setText(tr("发送:%1 字节").arg(SendCount));

    if (this->IsAutoClear)
    {
        ui->txtSend->setCurrentIndex(-1);
        ui->txtSend->setFocus();
    }
}

void frmMain::SendData()
{
    if (this->IsAutoSend)
    {        
        this->WriteMyCom();
    }
}

void frmMain::SaveMyCom()
{
    QDateTime now=QDateTime::currentDateTime();
    QString name=now.toString("yyyyMMddHHmmss");
    QString fileName=QCoreApplication::applicationDirPath()+"/"+name+".txt";
    //如果没有内容则不保存
    if (ui->txtDataHex->document()->toPlainText().length()==0)
    {
        return;
    }
    QFile file(fileName);
    file.open(QFile::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out<<ui->txtDataHex->document()->toPlainText();
    file.close();
}

void frmMain::SaveData()
{
    if (this->IsAutoSave)
    {        
        this->SaveMyCom();
    }
}

void frmMain::on_btnOpen_clicked()
{
    if (ui->btnOpen->text()=="打开串口")
    {
        QString portName="/dev/"+ui->cboxPortName->currentText();
        myCom = new Posix_QextSerialPort(portName, QextSerialBase::Polling);

        if (myCom->open(QIODevice::ReadWrite))
        {
            myCom->flush();
            myCom->setBaudRate((BaudRateType)ui->cboxBaudRate->currentIndex());
            myCom->setDataBits((DataBitsType)ui->cboxDataBit->currentIndex());
            myCom->setParity((ParityType)ui->cboxParity->currentIndex());
            myCom->setStopBits((StopBitsType)ui->cboxStopBit->currentIndex());
            myCom->setFlowControl(FLOW_OFF);
            myCom->setTimeout(10);

            this->ChangeEnable(true);
            ui->btnOpen->setText("关闭串口");
            ui->labIsOpen->setText(tr("串口状态：打开"));
            MyMessageBox::ChMessageOnlyOk_Information(tr("串口打开成功！(*^__^*) "));
        }
        else
        {
            MyMessageBox::ChMessageOnlyOk_Error(tr("串口打开失败！( ⊙ o ⊙ )"));
            return;
        }
    }
    else
    {
        this->ChangeEnable(false);
        ui->btnOpen->setText("打开串口");
        ui->labIsOpen->setText(tr("串口状态：关闭"));
        myCom->close();
    }
}

void frmMain::on_ckIsAutoSend_stateChanged(int )
{
    this->IsAutoSend=ui->ckIsAutoSend->isChecked();
    ui->cboxSend->setEnabled(this->IsAutoSend);
}

void frmMain::on_ckIsAutoSave_stateChanged(int )
{
    this->IsAutoSave=ui->ckIsAutoSave->isChecked();
    ui->cboxSave->setEnabled(this->IsAutoSave);
}

void frmMain::on_ckIsAutoClear_stateChanged(int )
{
    this->IsAutoClear=ui->ckIsAutoClear->isChecked();
}

void frmMain::on_btnClearSend_clicked()
{
    SendCount=0;
    ui->labSend->setText(tr("发送:0 字节"));
}

void frmMain::on_btnClearReceive_clicked()
{
    ReceiveCount=0;
    ui->labReceive->setText(tr("接收:0 字节"));
}

void frmMain::on_btnStopShow_clicked()
{
    if (ui->btnStopShow->text()=="停止显示")
    {
        IsShow=false;
        ui->btnStopShow->setText("开始显示");
    }
    else
    {
        IsShow=true;
        ui->btnStopShow->setText("停止显示");
    }
}

void frmMain::on_btnClearAll_clicked()
{    
    ui->txtDataHex->document()->clear();
}

void frmMain::on_btnSave_clicked()
{
    this->SaveMyCom();
}

void frmMain::on_btnSend_clicked()
{
    this->WriteMyCom();
}

void frmMain::ReadTxtData()
{
    ui->txtSend->clear();
    QString fileName=QCoreApplication::applicationDirPath()+"/data.txt";

    QFile file(fileName);
    file.open(QFile::ReadOnly | QIODevice::Text);
    QTextStream in(&file);
    QString line;

    do
    {
        line=in.readLine();

        if (line!="")
        {
            ui->txtSend->addItem(line);
        }
    }
    while(!line.isNull());

    file.close();
}

void frmMain::on_btnData_clicked()
{
    QString fileName=QCoreApplication::applicationDirPath()+"/data.txt";

    QFile file(fileName);

    if (!file.exists())
    {
        MyMessageBox::ChMessageOnlyOk_Error(tr("数据文件不存在！"));
        return;
    }

    if (ui->btnData->text()==tr("管理数据"))
    {
        ui->txtDataHex->setReadOnly(false);
        ui->gbox2->setTitle(tr("管理数据"));
        ui->txtDataHex->clear();

        file.open(QFile::ReadOnly | QIODevice::Text);
        QTextStream in(&file);
        ui->txtDataHex->setText(in.readAll());
        file.close();

        ui->btnData->setText(tr("保存数据"));
    }
    else
    {
        ui->txtDataHex->setReadOnly(true);
        ui->gbox2->setTitle(tr("接收数据"));

        file.open(QFile::WriteOnly | QIODevice::Text);
        QTextStream out(&file);
        out<<ui->txtDataHex->document()->toPlainText();
        file.close();

        ui->txtDataHex->clear();
        ui->btnData->setText(tr("管理数据"));

        this->ReadTxtData();
    }
}

void frmMain::on_cboxSend_currentIndexChanged(int index)
{
    if (this->IsInit)
    {
        mySendTimer->setInterval((index+1)*1000);
    }
}

void frmMain::on_cboxSave_currentIndexChanged(int index)
{
    if (this->IsInit)
    {
        mySaveTimer->setInterval((index+1)*1000);
    }
}
