#include "collectsetdialog.h"
#include "ui_collectsetdialog.h"
#include <QMessageBox>
CollectSetDialog::CollectSetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CollectSetDialog)
{
    ui->setupUi(this);
}

CollectSetDialog::~CollectSetDialog()
{
    delete ui;
}
void CollectSetDialog::commitcollectconfig(CollectSigConfig configdata)
{
    collectconfigdata = configdata;
    //信号接口
    ui->comboBox->setCurrentIndex(configdata.bootsigtype); //开机信号类型
    ui->lineEdit->setText(QString::number(configdata.bootsigwidth));//开机信号宽度
    ui->comboBox_2->setCurrentIndex(configdata.stopsigtype);//关机信号类型
    ui->lineEdit_2->setText(QString::number(configdata.stopsigwidth)); //关机信号宽度
    ui->comboBox_3->setCurrentIndex(configdata.faultsigtype); //故障信号类型
    ui->lineEdit_3->setText(QString::number(configdata.faultsigwidth)); //关故障类型宽度
    //脉冲接口
    ui->comboBox_4->setCurrentIndex(configdata.mainpulsesigtype-2);
    ui->lineEdit_4->setText(QString::number(configdata.mainpulsesigwidth));
    ui->lineEdit_6->setText(QString::number(configdata.mainpulsesigmininterval));
    ui->lineEdit_7->setText(QString::number(configdata.mainpulsesigmaxinterval));

    ui->comboBox_5->setCurrentIndex(configdata.revpulsesigtype-2);
    ui->lineEdit_5->setText(QString::number(configdata.revpulsesigwidth));
    ui->lineEdit_9->setText(QString::number(configdata.revpulsesigmininterval));
    ui->lineEdit_8->setText(QString::number(configdata.revpulsesigmaxinterval));
    //基础参数
    ui->comboBox_6->setCurrentIndex(configdata.pulserelate);
    ui->lineEdit_10->setText(QString::number(configdata.uploadtime));
    ui->lineEdit_11->setText(QString::number(configdata.realtime_store));
    ui->timeEdit->setTime(QTime(configdata.cleartime.hour,configdata.cleartime.minutes,0));

}
//确认
void CollectSetDialog::on_pushButton_clicked()
{

    collectconfigdata.bootsigtype = ui->comboBox->currentIndex();
    collectconfigdata.bootsigwidth =  ui->lineEdit->text().toInt();

    collectconfigdata.stopsigtype  = ui->comboBox_2->currentIndex();
    collectconfigdata.stopsigwidth = ui->lineEdit_2->text().toInt();

    collectconfigdata.faultsigtype = ui->comboBox_3->currentIndex();
    collectconfigdata.faultsigwidth = ui->lineEdit_3->text().toInt();

    collectconfigdata.mainpulsesigtype = ui->comboBox_4->currentIndex()+2;
    collectconfigdata.mainpulsesigwidth = ui->lineEdit_4->text().toInt();
    collectconfigdata.mainpulsesigmininterval = ui->lineEdit_6->text().toInt();
    collectconfigdata.mainpulsesigmaxinterval = ui->lineEdit_7->text().toInt();


    collectconfigdata.revpulsesigtype = ui->comboBox_5->currentIndex()+2;
    collectconfigdata.revpulsesigwidth = ui->lineEdit_5->text().toInt();
    collectconfigdata.revpulsesigmininterval = ui->lineEdit_9->text().toInt();
    collectconfigdata.revpulsesigmaxinterval = ui->lineEdit_8->text().toInt();

    collectconfigdata.pulserelate = ui->comboBox_6->currentIndex();
    collectconfigdata.uploadtime = ui->lineEdit_10->text().toInt();
    collectconfigdata.realtime_store = ui->lineEdit_11->text().toInt();
    QTime _time = ui->timeEdit->time();
    collectconfigdata.cleartime.hour = _time.hour();
    collectconfigdata.cleartime.minutes = _time.minute();

    SetCollectConfig(collectconfigdata);

    QMessageBox msgBox(this);
    msgBox.setText(tr("配置下发"));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}
//取消
void CollectSetDialog::on_pushButton_2_clicked()
{

   this->close();

}
//获取配置
void CollectSetDialog::on_pushButton_3_clicked()
{
   emit GetCollectConfig();
    QMessageBox msgBox(this);
    msgBox.setText(tr("获取配置"));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}
