#include "serialPort.h"

SerialPort::SerialPort(){
   connect(&my_serialport, SIGNAL(readyRead()), this, SLOT(readyReadSlot()));

}
SerialPort::~SerialPort(){
   disconnect(&my_serialport, SIGNAL(readyRead()), this, SLOT(readyReadSlot()));
}

/**
 * 打开串口
 */
bool SerialPort::openPort(QSerialPortInfo &info,QSerialPort::BaudRate BaudRate){
    if(my_serialport.isOpen()) my_serialport.close();
    my_serialinfo = info;
    my_serialport.setPort(my_serialinfo);
    if(my_serialport.open(QIODevice::ReadWrite)){
        my_serialport.setBaudRate(BaudRate);
        my_serialport.setParity(QSerialPort::NoParity);
        my_serialport.setDataBits(QSerialPort::Data8);
        my_serialport.setStopBits(QSerialPort::OneStop);
        my_serialport.setFlowControl(QSerialPort::NoFlowControl);
        my_serialport.clearError();
        my_serialport.clear();
    } else {
        return false;
    }
    return  true;
}

/**
 * 串口发送数据
 */
void SerialPort::sendDataPort(QByteArray data){

    if(my_serialport.isOpen()) my_serialport.write(data);
}

/**
 * 提交数据
 */
void SerialPort::readyReadSlot(){
    datas = my_serialport.readAll();
    emit commitData(datas);
}


