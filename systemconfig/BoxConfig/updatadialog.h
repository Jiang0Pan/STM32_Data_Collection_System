#ifndef UPDATADIALOG_H
#define UPDATADIALOG_H

#include <QDialog>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QFileInfo>
#include <QTime>
#include <QCoreApplication>
#include "datahandle/datastruct.h"

namespace Ui {
    class UpDataDialog;
}

class UpDataDialog : public QDialog {
    Q_OBJECT

public:
    explicit UpDataDialog(QWidget *parent = nullptr);
    ~UpDataDialog();
    uint16_t mc_check_crc16(unsigned char *lpByte,unsigned short int iLen);
    void sleep(int sectime);
signals:
    void UpGradeApplication(char* data,uint16_t length);
private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_clicked();
    void timer1timeout(UpGradeAsk upgradedata);
    void on_pushButton_4_clicked();

private:
    Ui::UpDataDialog *ui;
    QString          filename;
    QFile            file;
    uint32_t  filesize;
    uint8_t   sendflag = 0;
    uint16_t  sendpackage = 0;
    uint16_t  current_sendpackage = 0;
    uint16_t  crc_val;
    uint16_t  dataLen=0;
    QByteArray   file_t;
    uint8_t   file_tdata[2564] = {0};
    UpGradeAsk upgradedata ;
};

#endif // UPDATADIALOG_H
