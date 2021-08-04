#ifndef NETSETDIALOG_H
#define NETSETDIALOG_H

#include <QDialog>
#include "datahandle/datastruct.h"

namespace Ui {
class NetSetDialog;
}

class NetSetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NetSetDialog(QWidget *parent = 0);
    ~NetSetDialog();

signals:
    void GetNetConfig();
    void SetNetConfig(NetConfig configdata);

private slots:
    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void commitnetconfig(NetConfig configdata);//提交网络参数配置


private:
    Ui::NetSetDialog *ui;
    NetConfig netconfigdata;
};

#endif // NETSETDIALOG_H
