#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "serialPort/serialPort.h"
#include "datahandle/datahandle.h"
#include "collectsetdialog.h"
#include "netsetdialog.h"
#include "outsetdialog.h"//输出配置
#include "updatadialog.h"
#include "version.h"


namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    void serial_send_data(QString data);
private slots:
    void on_pushButton_4_clicked();

    void on_pushButton_clicked();

    void commitdata(PraseCollectData prasedata);//提交检测数据

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

private:
    Ui::Dialog *ui;
    DataHandle dataHandle;
    SerialPort myserialport;
    CollectSetDialog collectDig;
    NetSetDialog     netDig;
    OutSetDialog     outDig;//输出对话框
    UpDataDialog     upDig;
    Version          vsDig;
};

#endif // DIALOG_H
