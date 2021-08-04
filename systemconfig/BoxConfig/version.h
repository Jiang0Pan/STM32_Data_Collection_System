#ifndef VERSION_H
#define VERSION_H

#include <QDialog>
#include <QMessageBox>
#include "datahandle/datastruct.h"

namespace Ui {
class Version;
}

class Version : public QDialog
{
    Q_OBJECT

public:
    explicit Version(QWidget *parent = nullptr);
    ~Version();
signals:
    void GetVersionCompieer(void);
private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
    void displayversioncompier(VersionCompier data);
private:
    Ui::Version *ui;
    VersionCompier textdata;
};

#endif // VERSION_H
