#include "updatadialog.h"
#include "ui_updatadialog.h"

UpDataDialog::UpDataDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UpDataDialog)
{
    ui->setupUi(this);
    ui->pushButton_3->setDisabled(true);
    ui->pushButton_4->setDisabled(true);
    ui->lineEdit->setReadOnly(true);
}

UpDataDialog::~UpDataDialog()
{
    delete ui;
}

// 取消，返回上一级
void UpDataDialog::on_pushButton_2_clicked()
{
    this->close();
    ui->pushButton_2->setText("取消");
    ui->lineEdit->clear();
    ui->progressBar->setValue(0);
}

// APP下升级，将文件按协议通过串口发送出去
void UpDataDialog::on_pushButton_3_clicked()
{
    QMessageBox mesg;
    file.setFileName(filename);
    if(!file.open(QIODevice::ReadOnly)) {
           ui->label_7->setText("升级失败");
           mesg.critical(this,tr("Error"),tr("打开Bin文件发生错误！"));
    }
    else {
        filesize=file.size();
        sendpackage                  = (2559 + filesize)/2560;
        current_sendpackage          =  0;
        dataLen                      =  0;
        sendflag                     =  1;
        ui->pushButton_3->setDisabled(true);
        ui->pushButton_2->setDisabled(true);
        ui->pushButton_4->setDisabled(true);
        emit UpGradeApplication(NULL,0);
    }
}

// BOOT下升级，将文件按协议通过串口发送出去
void UpDataDialog::on_pushButton_4_clicked()
{
    QMessageBox mesg;
    file.setFileName(filename);
    if(!file.open(QIODevice::ReadOnly)) {
       ui->label_7->setText("升级失败");
       mesg.critical(this,tr("Error"),tr("打开Bin文件发生错误！"));
    }
    else {
        filesize=file.size();
        ui->progressBar->setValue(0);
        sendpackage                  = (2559 + filesize)/2560;
        current_sendpackage          =  0;
        dataLen                      =  0;
        sendflag                     =  2;
        ui->pushButton_3->setDisabled(true);
        ui->pushButton_2->setDisabled(true);
        ui->pushButton_4->setDisabled(true);
        timer1timeout(upgradedata);
    }
}

//选择文件路径，打开一个文件对话框
void UpDataDialog::on_pushButton_clicked()
{
    ui->label_7->setText("请选择需要升级的文件!!!");
    ui->progressBar->setValue(0);
    filename = QFileDialog::getOpenFileName(this,tr("Select a Bin file"),"./",tr("bin file(*.bin)"));
    if(filename.isEmpty()){
        QMessageBox mesg;
        mesg.critical(this,tr("Error"),tr("打开Bin文件发生错误！"));
        return;
    }
    else{
        ui->lineEdit->setText(filename);
        ui->label_7->setText("文件准备就绪!!!");
        ui->pushButton_3->setDisabled(false);
        ui->pushButton_2->setDisabled(false);
        ui->pushButton_4->setDisabled(false);
        ui->pushButton_2->setText("取消");
    }
}

//CRC校验函数
uint16_t UpDataDialog::mc_check_crc16(uint8_t  *lpByte,uint16_t iLen)
{
    uint16_t iCrc = 0;
    uint16_t i=0;
    uint16_t n=0;
    if (iLen > 0){
        iCrc = 0xFFFF;
        for (n = 0; n < iLen; n++){
            iCrc = (uint16_t)(iCrc ^ lpByte[n]);
            for (i = 0; i < 8; i++){
                uint16_t TT;
                TT = (uint16_t)(iCrc & 1);
                iCrc = (uint16_t)(iCrc >> 1);
                iCrc = (uint16_t)(iCrc & 0x7FFF);
                if (TT == 1){
                    iCrc = (uint16_t)(iCrc ^ 0xA001);
                }
                iCrc = (uint16_t)(iCrc & 0xFFFF);
            }
        }
    }
    return iCrc;
}

//升级函数
void UpDataDialog::timer1timeout(UpGradeAsk upgradedata)
{
   if(sendflag == 1){
       if(strstr((char *)&upgradedata.gradeask,"APP:OK")!=NULL){
           memset((void *)&upgradedata,0,sizeof(upgradedata));
           sleep(100);
           sendflag = 2;
           ui->label_7->setText("重启中......");
       }
   }
   if(sendflag == 2){
        if(strstr((char *)&upgradedata.gradeask,"BOOT:OK")!=NULL){
            memset((void *)&upgradedata,0,sizeof(upgradedata));
            sleep(50);
            sendflag = 3;
            ui->label_7->setText("BOOT握手成功......");
        }
        else{
            memset(file_tdata,0,2564);
            file_tdata[0]   = 'B';
            file_tdata[1]   = 'O';
            file_tdata[2]   = 'O';
            file_tdata[3]   = 'T';
            file_tdata[4]   = '?';
            emit UpGradeApplication((char *)file_tdata,5);
        }
    }
    if(sendflag == 3){
        if(strstr((char *)&upgradedata.gradeask,"OK\n")!=NULL){
            memset((void *)&upgradedata,0,sizeof(upgradedata));
            sleep(50);
            sendflag = 4;
            ui->label_7->setText("升级准备中......");
        }
        else{
            memset(file_tdata,0,2564);
            file_tdata[2]  = (uint8_t)(sendpackage  & 0xFF);
            file_tdata[3]  = (uint8_t)((sendpackage>>8) & 0xFF);
            crc_val        = mc_check_crc16(&file_tdata[2],2);
            file_tdata[0]  = (uint8_t)(crc_val  & 0xFF);
            file_tdata[1]  = (uint8_t)((crc_val>>8)   & 0xFF);
            emit UpGradeApplication((char *)file_tdata,4);
        }
    }
    if(sendflag == 4){
        if(strstr((char *)&upgradedata.gradeask,"OK\n")!=NULL){
            memset((void *)&upgradedata,0,sizeof(upgradedata));
            current_sendpackage++;
            ui->progressBar->setValue(current_sendpackage*100/sendpackage);
            ui->label_7->setText("升级中......");
            sleep(550);
            if(current_sendpackage == sendpackage){
                sendflag = 5;
            }
            if(!file.atEnd()){
                memset(file_tdata,0,2564);
                dataLen                 = file.read((char *)&file_tdata[4],2560);
                file_tdata[2]           = (uint8_t)(current_sendpackage  & 0xFF);
                file_tdata[3]           = (uint8_t)((current_sendpackage>>8) & 0xFF);
                crc_val                 = mc_check_crc16(&file_tdata[2],dataLen+2);
                file_tdata[0]           = (uint8_t)(crc_val & 0xFF);
                file_tdata[1]           = (uint8_t)((crc_val>>8)   & 0xFF);
                emit UpGradeApplication((char *)file_tdata,dataLen+4);
            }
        }
        else if(strstr((char *)&upgradedata.gradeask,"Error\n")!=NULL){
            memset((void *)&upgradedata,0,sizeof(upgradedata));
            emit UpGradeApplication((char *)file_tdata,dataLen+4);
        }
        else {
            if(!file.atEnd()){
                memset(file_tdata,0,2564);
                dataLen                 = file.read((char *)&file_tdata[4],2560);
                file_tdata[2]           = (uint8_t)(current_sendpackage  & 0xFF);
                file_tdata[3]           = (uint8_t)((current_sendpackage>>8) & 0xFF);
                crc_val                 = mc_check_crc16(&file_tdata[2],dataLen+2);
                file_tdata[0]           = (uint8_t)(crc_val & 0xFF);
                file_tdata[1]           = (uint8_t)((crc_val>>8)   & 0xFF);
                emit UpGradeApplication((char *)file_tdata,dataLen+4);
            }
       }
    }
    if(sendflag == 5){
        ui->pushButton_2->setDisabled(false);
        ui->label_7->setText("升级完成!!!");
        ui->pushButton_2->setText("确定");
        file.close();
    }
}

void UpDataDialog::sleep(int sectime)
{
    QTime dieTime = QTime::currentTime().addMSecs(sectime);

    while (QTime::currentTime() < dieTime) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
}


