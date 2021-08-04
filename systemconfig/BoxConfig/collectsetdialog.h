#ifndef COLLECTSETDIALOG_H
#define COLLECTSETDIALOG_H

#include <QDialog>
#include "datahandle/datastruct.h"
namespace Ui {
class CollectSetDialog;
}

class CollectSetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CollectSetDialog(QWidget *parent = 0);
    ~CollectSetDialog();

signals:
    void GetCollectConfig();
    void SetCollectConfig(CollectSigConfig configdata);



private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void commitcollectconfig(CollectSigConfig configdata);//获取采集配置参数

private:
    Ui::CollectSetDialog *ui;
    CollectSigConfig collectconfigdata;

};

#endif // COLLECTSETDIALOG_H
