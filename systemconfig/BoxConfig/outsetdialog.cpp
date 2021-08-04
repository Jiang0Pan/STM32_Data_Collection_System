#include "outsetdialog.h"
#include "ui_outsetdialog.h"
#include <QMessageBox>

OutSetDialog::OutSetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OutSetDialog)
{
    ui->setupUi(this);
}

OutSetDialog::~OutSetDialog()
{
    delete ui;
}
void OutSetDialog::commitoutconfig(OutConfig configdata)//提交网络参数配置
{
    outconfigdata = configdata;
    //设置插纸机
    ui->lineEdit_5->setText(QString::number(configdata.InPaperTime));
    ui->lineEdit->setText(QString::number(configdata.InPaperAfterCut));
    ui->comboBox->setCurrentIndex(configdata.StopSigType);
}
//获取配置
void OutSetDialog::on_pushButton_3_clicked()
{
    emit GetOutConfig();
    QMessageBox msgBox(this);
    msgBox.setText(tr("获取配置"));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}
//下发配置
void OutSetDialog::on_pushButton_clicked()
{
   memset(&outconfigdata,0,sizeof(OutConfig));
   outconfigdata.InPaperTime = ui->lineEdit_5->text().toInt();
   outconfigdata.InPaperAfterCut = ui->lineEdit->text().toInt();
   outconfigdata.StopSigType = ui->comboBox->currentIndex();

   emit SetOutConfig(outconfigdata);

   QMessageBox msgBox(this);
   msgBox.setText(tr("配置下发"));
   msgBox.setStandardButtons(QMessageBox::Ok);
   msgBox.exec();
}
//取消配置
void OutSetDialog::on_pushButton_2_clicked()
{
    this->close();
}
