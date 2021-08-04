#include "netsetdialog.h"
#include "ui_netsetdialog.h"
#include <QMessageBox>
NetSetDialog::NetSetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NetSetDialog)
{
    ui->setupUi(this);
}

NetSetDialog::~NetSetDialog()
{
    delete ui;
}
void NetSetDialog::commitnetconfig(NetConfig configdata)//提交网络参数配置
{
    netconfigdata = configdata;
    //设置选择模式

    ui->comboBox->setCurrentText(QLatin1String((const char *)(configdata.devmode)));
    ui->lineEdit->setText(QLatin1String((const char *)(configdata.wifiname)));
    ui->lineEdit_2->setText(QLatin1String((const char *)(configdata.wifipassword)));
    ui->lineEdit_3->setText(QLatin1String((const char *)(configdata.serverip)));
    ui->lineEdit_4->setText(QLatin1String((const char *)(configdata.serverport)));
    ui->lineEdit_5->setText(QLatin1String((const char *)(configdata.devname)));

    ui->lineEdit_6->setText(QLatin1String((const char *)(configdata.localip)));
    ui->lineEdit_7->setText(QLatin1String((const char *)(configdata.localmask)));
    ui->lineEdit_8->setText(QLatin1String((const char *)(configdata.localgateway)));

    ui->comboBox_2->setCurrentText(QLatin1String((const char *)(configdata.dhcp_en)));
    ui->lineEdit_9->setText(QLatin1String((const char *)(configdata.apname)));
    ui->lineEdit_10->setText(QLatin1String((const char *)(configdata.appassword)));
}
//获取网络配置
void NetSetDialog::on_pushButton_3_clicked()
{
    emit GetNetConfig();
    QMessageBox msgBox(this);
    msgBox.setText(tr("获取配置"));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}
//配置确认
void NetSetDialog::on_pushButton_clicked()
{
   QString _str = "";

   memset(&netconfigdata,0,sizeof(NetConfig));

   _str = ui->comboBox->currentText();
   memcpy(&netconfigdata.devmode,_str.toLatin1().data(),_str.length());
   _str = ui->lineEdit_5->text();
   memcpy(&netconfigdata.devname,_str.toLatin1().data(),_str.length());
   _str = ui->lineEdit->text();
   memcpy(&netconfigdata.wifiname,_str.toLatin1().data(),_str.length());
   _str = ui->lineEdit_2->text();
   memcpy(&netconfigdata.wifipassword,_str.toLatin1().data(),_str.length());
   _str = ui->lineEdit_3->text();
   memcpy(&netconfigdata.serverip,_str.toLatin1().data(),_str.length());
   _str = ui->lineEdit_4->text();
   memcpy(&netconfigdata.serverport,_str.toLatin1().data(),_str.length());

   _str = ui->lineEdit_6->text();
   memcpy(&netconfigdata.localip,_str.toLatin1().data(),_str.length());
   _str = ui->lineEdit_7->text();
   memcpy(&netconfigdata.localmask,_str.toLatin1().data(),_str.length());
   _str = ui->lineEdit_8->text();
   memcpy(&netconfigdata.localgateway,_str.toLatin1().data(),_str.length());

   _str = ui->lineEdit_9->text();
   memcpy(&netconfigdata.apname,_str.toLatin1().data(),_str.length());
   _str = ui->lineEdit_10->text();
   memcpy(&netconfigdata.appassword,_str.toLatin1().data(),_str.length());
   _str = ui->comboBox_2->currentText();
   memcpy(&netconfigdata.dhcp_en,_str.toLatin1().data(),_str.length());

   emit SetNetConfig(netconfigdata);
   QMessageBox msgBox(this);
   msgBox.setText(tr("配置下发"));
   msgBox.setStandardButtons(QMessageBox::Ok);
   msgBox.exec();
}
//配置取消
void NetSetDialog::on_pushButton_2_clicked()
{
    this->close();
}
