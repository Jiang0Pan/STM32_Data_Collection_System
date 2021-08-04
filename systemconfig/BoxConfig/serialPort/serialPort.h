#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QByteArray>

class SerialPort : public QObject
{
    Q_OBJECT
public:
    SerialPort();
    ~SerialPort();
    bool openPort(QSerialPortInfo &info,QSerialPort::BaudRate BaudRate);//打开串口

    QSerialPortInfo my_serialinfo;
    QSerialPort my_serialport;
    QByteArray datas;
signals:
    void commitData(QByteArray data);//接收到串口数据

private slots:
    void readyReadSlot();//解析数据
    void sendDataPort(QByteArray data);//发送串口数据
};

#endif // SERIALPORT_H
