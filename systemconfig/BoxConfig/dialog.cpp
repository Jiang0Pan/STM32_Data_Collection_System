#include "dialog.h"
#include "ui_dialog.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QList>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    //获取计算机上所有可用串口并添加到comboBox中
    ui->comboBox->clear();
    QList<QSerialPortInfo>  infos = QSerialPortInfo::availablePorts();
    if(infos.isEmpty())
    {
        ui->comboBox->addItem("无串口");
        return;
    }
    foreach (QSerialPortInfo info, infos) {
        ui->comboBox->addItem(info.portName());
    }
    //信号和槽
    connect(&myserialport,SIGNAL(commitData( QByteArray  )),
            &dataHandle,SLOT(readyReadSlot(QByteArray )));
    connect(&dataHandle,SIGNAL(sendData(QByteArray  )),
            &myserialport,SLOT(sendDataPort(QByteArray )));
    connect(&dataHandle,SIGNAL(commitdata(PraseCollectData  )),
            this,SLOT(commitdata(PraseCollectData )));

    //采集参数配置部分
    connect(&dataHandle,SIGNAL(commitcollectconfig( CollectSigConfig  )),
            &collectDig,SLOT(commitcollectconfig(CollectSigConfig )));
    connect(&collectDig,SIGNAL(GetCollectConfig()),
            &dataHandle,SLOT(GetCollectConfig()));
    connect(&collectDig,SIGNAL(SetCollectConfig(CollectSigConfig)),
            &dataHandle,SLOT(SetCollectConfig(CollectSigConfig)));

    //网络参数配置部分
    connect(&dataHandle,SIGNAL(commitnetconfig( NetConfig  )),
            &netDig,SLOT(commitnetconfig(NetConfig )));
    connect(&netDig,SIGNAL(GetNetConfig()),
            &dataHandle,SLOT(GetNetConfig()));
    connect(&netDig,SIGNAL(SetNetConfig(NetConfig)),
            &dataHandle,SLOT(SetNetConfig(NetConfig)));

    //外部输出配置部分
    connect(&dataHandle,SIGNAL(commitoutconfig( OutConfig  )),
            &outDig,SLOT(commitoutconfig(OutConfig )));
    connect(&outDig,SIGNAL(GetOutConfig()),
            &dataHandle,SLOT(GetOutConfig()));
    connect(&outDig,SIGNAL(SetOutConfig(OutConfig)),
            &dataHandle,SLOT(SetOutConfig(OutConfig)));

    //升级部分
    connect(&upDig,SIGNAL(UpGradeApplication(char *,uint16_t)),
            &dataHandle,SLOT(UpGradeApplication(char *,uint16_t)));
    connect(&dataHandle,SIGNAL(timer1timeout(UpGradeAsk )),
            &upDig,SLOT(timer1timeout(UpGradeAsk )));

    //查看版本部分
    connect(&vsDig,SIGNAL(GetVersionCompieer()),
            &dataHandle,SLOT(GetVersionCompieer()));
    connect(&dataHandle,SIGNAL(displayversioncompier(VersionCompier)),
            &vsDig,SLOT(displayversioncompier(VersionCompier)));

    //获取WIFI状态部分
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::commitdata(PraseCollectData prasedata)
{
    ui->lineEdit_7->setText(prasedata.guid);
    ui->lineEdit_8->setText(QString::number(prasedata.alltime));
    ui->lineEdit_2->setText(QString::number(prasedata.boottime));
    ui->lineEdit_3->setText(QString::number( prasedata.stoptime));
    ui->lineEdit_4->setText(QString::number(prasedata.faulttime));

    if(prasedata.bootstopsig != 0) ui->label_10->setDisabled(false);
    else ui->label_10->setDisabled(true);
    if(prasedata.faultsig != 0) ui->label_11->setDisabled(false);
    else ui->label_11->setDisabled(true);
    if(prasedata.pnopaperstate != 0) ui->label_15->setDisabled(false);
    else ui->label_15->setDisabled(true);

    ui->lineEdit->setText(QString::number(prasedata.pulsecount));
    ui->lineEdit_5->setText(prasedata.timebegin);
    ui->lineEdit_6->setText( prasedata.timecurrent);
    ui->lineEdit_9->setText(QString::number(prasedata.revpulsecount));
    ui->lineEdit_10->setText(QString::number(prasedata.bladepulsecount));
    ui->lineEdit_11->setText(prasedata.bladepulsetime);
}
//打开串口
void Dialog::on_pushButton_4_clicked()
{
    QSerialPortInfo serialPort_com(ui->comboBox->currentText());
    bool state =  myserialport.openPort(serialPort_com,QSerialPort::BaudRate::Baud115200);
    if(state == true)
    {
        QMessageBox msgBox(this);
        msgBox.setText(tr("串口打开成功"));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
    }
    else
    {

        QMessageBox msgBox(this);
        msgBox.setText(tr("串口打开失败，请在设备管理器中确认串口号"));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
    }
}


void Dialog::on_pushButton_clicked()
{
    dataHandle.SetClearCount();
    QMessageBox msgBox(this);
    msgBox.setText(tr("计数清除成功"));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}

void Dialog::on_pushButton_2_clicked()
{
    dataHandle.SetTimeSynchronization();
    QMessageBox msgBox(this);
    msgBox.setText(tr("时间同步成功"));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}

void Dialog::on_pushButton_3_clicked()
{
    bool isOK = false;
    QString text = QInputDialog::getText(this, "设置设备ID",
                                                           "设备ID",
                                                           QLineEdit::Normal,
                                                           "ID",
                                                           &isOK);
        if(isOK) {
                dataHandle.SetiD(text);
                QMessageBox msgBox(this);
                msgBox.setText(tr("ID设置成功"));
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.exec();

        }

}

//采集参数设置
void Dialog::on_pushButton_5_clicked()
{
  collectDig.exec();
}
//
//网络参数设置
void Dialog::on_pushButton_6_clicked()
{

  netDig.exec();

}
//插纸操作
void Dialog::on_pushButton_7_clicked()
{
    dataHandle.SetInpaperConfig();
    QMessageBox msgBox(this);
    msgBox.setText(tr("设备插纸"));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}
//输出设置
void Dialog::on_pushButton_8_clicked()
{
    outDig.exec();
}

//升级
void Dialog::on_pushButton_9_clicked()
{
    upDig.exec();
}

//获取版本
void Dialog::on_pushButton_10_clicked()
{
     vsDig.exec();
}


