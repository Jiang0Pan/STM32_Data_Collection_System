#ifndef OUTSETDIALOG_H
#define OUTSETDIALOG_H

#include <QDialog>
#include "datahandle/datastruct.h"

namespace Ui {
class OutSetDialog;
}

class OutSetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OutSetDialog(QWidget *parent = 0);
    ~OutSetDialog();

signals:
    void GetOutConfig();
    void SetOutConfig(OutConfig configdata);
private slots:
    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void commitoutconfig(OutConfig configdata);//提交网络参数配置
private:
    Ui::OutSetDialog *ui;
    OutConfig outconfigdata;
};

#endif // OUTSETDIALOG_H
