#ifndef DATAHANDLE_H
#define DATAHANDLE_H

#include <QList>
#include <QByteArray>
#include "datastruct.h"
#include <QTimer>
class DataHandle : public QObject
{
    Q_OBJECT
public:
    DataHandle();
    ~DataHandle();
    //交互部分
    void SetiD(QString DevId);
    void SetTimeSynchronization();
    void SetClearCount();
    void SetInpaperConfig();

private:
    QList<quint8> revdatabuf;//串口接收数据缓冲区
    UartMgr       uartmgr;   //串口数据管理
    QTimer        comTimer;//通道计时，判断通讯状态
    quint8        comstate;//通讯状态
private:
   QByteArray Pack_Protocol(quint16 cmd, char *data,quint16 length);
   void PcDataAnaly(quint8 data);
   void PcDataHandle( UartUpData uartData);
   void UartCommitDataHandle(quint8 * Databuf);
   void UartConfigHandle(quint8 * Databuf);
   void UartNetConfigHandle(quint8 * Databuf);
   void UartOutConfigHandle(quint8 * Databuf);
   void UartCommitUpGrade(quint8 * Databuf);
   void UartCommitVersionCompier(quint8 * Databuf);
private://电机管理部分
   QTimer motoTimer;     //电机时间
   quint32 currentspeed; //当前电机转速
   QList<qint32> speedbuf;//速度缓冲区
signals:
   void sendData(QByteArray data);//提交需要发送数据给串口
    //交互部分
   void commitdata(PraseCollectData prasedata);//提交检测数据
   void commitcollectconfig(CollectSigConfig configdata);//获取采集配置参数
   void commitnetconfig(NetConfig configdata);//提交网络参数配置

   void commitoutconfig(OutConfig configdata);//提交输出参数配置

   void displayversioncompier(VersionCompier data);
   void timer1timeout(UpGradeAsk upgradedata);
private slots:
   void readyReadSlot(QByteArray data);//解析串口接收数据
   void GetCollectConfig(void);//获取采集配置参数
   void SetCollectConfig(CollectSigConfig collectconfig);//设置采集配置参数
   void GetNetConfig(void);
   void SetNetConfig(NetConfig netconfig);//

   void GetOutConfig(void);
   void SetOutConfig(OutConfig netconfig);//设置输出配置

   void UpGradeApplication(char* data,uint16_t length);
   void GetVersionCompieer(void);
   void GetWifiStatus(void);
};

#endif // SERIALPORT_H
