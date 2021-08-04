#include "version.h"
#include "ui_version.h"

Version::Version(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Version)
{
    ui->setupUi(this);
    ui->lineEdit_2->setReadOnly(true);
    ui->lineEdit_3->setReadOnly(true);
}

Version::~Version()
{
    delete ui;
}

void Version::on_pushButton_2_clicked()
{
    this->close();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->pushButton_2->setText("取消");
}

void Version::on_pushButton_clicked()
{
    emit GetVersionCompieer();
    QMessageBox msgBox(this);
    msgBox.setText(tr("获取版本、编译信息"));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}

void Version::displayversioncompier(VersionCompier data)
{
   textdata = data;
   ui->lineEdit_2->setText(QLatin1String((const char *)(data.version)));
   ui->lineEdit_3->setText(QLatin1String((const char *)(data.compier)));
   ui->pushButton_2->setText("确定");
}


