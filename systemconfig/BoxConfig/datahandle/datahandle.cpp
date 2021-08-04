#include "datahandle.h"
#include <QDateTime>
#include <QLatin1String>
DataHandle::DataHandle(){

     qRegisterMetaType<PraseCollectData>("PraseCollectData");
     qRegisterMetaType<CollectSigConfig>("CollectSigConfig");
     qRegisterMetaType<NetConfig>("NetConfig");
     qRegisterMetaType<OutConfig>("OutConfig");
     revdatabuf.clear();
     uartmgr.RxSts = 0xff;
}
DataHandle::~DataHandle(){

}

//数据打包
QByteArray DataHandle::Pack_Protocol(quint16 cmd, char *data,quint16 length)
{
     UartUpData uartdata;
     uartdata.header = UARTHEAD;
     uartdata.devDAID = 0;
     uartdata.devSAID = 1;
     uartdata.commend = cmd;
     uartdata.iLenght = length;
     QByteArray _databuf;
     _databuf.append((char*)(&uartdata),sizeof(quint16)*5);
     if(length != 0) _databuf.append(data,length);
     quint16 tail = UARTTAIL;
     _databuf.append((char*)(&tail),sizeof(quint16));
     return _databuf;
}

//交互部分
void DataHandle::SetiD(QString DevId)
{
    emit sendData(Pack_Protocol(CONFIG_ID,DevId.toLatin1().data(),DevId.length()));
}
//时间同步
void DataHandle::SetTimeSynchronization()
{
    QDateTime _datetime = QDateTime::currentDateTime();
    RTC_DateType timestruct;
    timestruct.Date = _datetime.date().day();
    timestruct.Month = _datetime.date().month();
    timestruct.Year = _datetime.date().year()-2000;
    timestruct.Hours = _datetime.time().hour();
    timestruct.Minutes = _datetime.time().minute();
    timestruct.Seconds = _datetime.time().second();
    timestruct.WeekDay = 5;
    emit sendData(Pack_Protocol(CONFIG_TIME,(char *)&timestruct,sizeof(RTC_DateType)));
}
//
//清除计数
void DataHandle::SetClearCount()
{
    emit sendData(Pack_Protocol(CONFIG_CLEAR,NULL,0));
}
//获取采集配置
void DataHandle::GetCollectConfig(void)
{
    emit sendData(Pack_Protocol(GETCONFIG_COLLECT,NULL,0));

}
//采集参数配置
void DataHandle::SetCollectConfig(CollectSigConfig collectconfig)
{
     emit sendData(Pack_Protocol(CONFIG_COLLECT,(char *)&collectconfig,sizeof(CollectSigConfig)));
}


//获取网络配置
void DataHandle::GetNetConfig(void)
{
    emit sendData(Pack_Protocol(GETCONFIG_NET,NULL,0));
}
//网络配置
void DataHandle::SetNetConfig(NetConfig netconfig)
{
     emit sendData(Pack_Protocol(CONFIG_NET,(char *)&netconfig,sizeof(NetConfig)));
}
//插纸配置
void DataHandle::SetInpaperConfig(void)
{
     emit sendData(Pack_Protocol(PC_ISSUE_INPAPER,NULL,0));
}
//获取输出配置
void DataHandle::GetOutConfig(void)
{
     emit sendData(Pack_Protocol(GETCONFIG_OUT,NULL,0));
}
//设置输出配置
void DataHandle::SetOutConfig(OutConfig outconfig)//设置输出配置
{
    emit sendData(Pack_Protocol(CONFIG_OUT,(char *)&outconfig,sizeof(OutConfig)));
}
//升级
void DataHandle::UpGradeApplication(char* data,uint16_t length)
{
    if (data == NULL) {
        emit sendData(Pack_Protocol(PC_ISSUE_UPGRADE,NULL,0));
    }
    else {
        emit sendData(Pack_Protocol(PC_ISSUE_UPGRADE,data,length));
    }
}

//获取版本信息
void DataHandle::GetVersionCompieer()
{
    emit sendData(Pack_Protocol(PC_ISSUE_VERSION,NULL,0));
}

//获取wifi状态
void DataHandle::GetWifiStatus()
{
    emit sendData(Pack_Protocol(PC_ISSUE_WIFISUS,NULL,0));
}
//槽函数
void DataHandle::readyReadSlot(QByteArray data)//解析串口接收数据
{
    foreach (quint8 var, data) {
        revdatabuf.push_back(var);
    }
     while(!revdatabuf.isEmpty())
     {
         PcDataAnaly(revdatabuf.first());
         revdatabuf.pop_front();
     }
}
//数据解析
void DataHandle::PcDataAnaly(quint8 data)
{
     static quint8 RecDataBuf[2];
     quint32 TempComDat = 0;
     static quint16 datacount = 0;
     RecDataBuf[0]=RecDataBuf[1];
     RecDataBuf[1]=data;
     TempComDat = data;
     if(RecDataBuf[0]==0xEF && RecDataBuf[1]==0xFE)
        {
            uartmgr.RxSts = 0;//
            datacount = 0;    //
            memset(&uartmgr.uartData,0,sizeof(UartUpData));
            uartmgr.uartData.header = UARTHEAD;
        }
        switch (uartmgr.RxSts)
        {
            case 0:     //
                    uartmgr.RxSts = 1;
            break;

            case 1:     //接收命令
                uartmgr.uartData.devSAID+=  TempComDat<<(datacount*8);
                datacount++;
                if(datacount >= sizeof(quint16))
                {
                     datacount = 0;
                     uartmgr.RxSts= 2;
                }
            break;
            case 2:     //接收源ID号
                uartmgr.uartData.devDAID +=  TempComDat<<(datacount*8);
                datacount++;
                if(datacount >= sizeof(quint16))
                {
                     datacount = 0;
                     uartmgr.RxSts= 3;
                }
            break;
            case 3:     //接收目标ID号
                uartmgr.uartData.commend +=  TempComDat<<(datacount*8);
                datacount++;
                if(datacount >= sizeof(quint16))
                {
                     datacount = 0;
                     uartmgr.RxSts=4;
                     uartmgr.uartData.iLenght=0;
                }
            break;

            case 4:     //接收数据长度
                uartmgr.uartData.iLenght +=  TempComDat<<(datacount*8);
                datacount++;
                if(datacount >= sizeof(quint16))
                {
                      datacount = 0;
                      if(uartmgr.uartData.iLenght != 0 ) uartmgr.RxSts= 5;
                      else uartmgr.RxSts= 6;
                }
            break;

            case 5:     //接收数据
                uartmgr.uartData.databuf[datacount++] = data;
                if(datacount >= uartmgr.uartData.iLenght)
                {
                     datacount = 0;
                     uartmgr.RxSts = 6;
                     uartmgr.uartData.tailer = 0;
                }
            break;

            case 6:     //接收数据尾
                uartmgr.uartData.tailer +=  TempComDat<<(datacount*8);
                datacount++;
                if(datacount >= sizeof(quint16))
                {
                    datacount = 0;
                    uartmgr.RxSts = 255;
                    if(uartmgr.uartData.tailer == UARTTAIL) //判断目标ID和本身ID一一致，为当前设备需要解析的数据
                     {
                          //处理数据,如果数据处理吃力可以考虑采用线程
                          PcDataHandle(uartmgr.uartData);
                          comstate = 1;//接收到通讯数据
                     }
                }
                else
                    {
                }
            break;

            default:
                 uartmgr.RxSts = 255;
            break;
        }
}
//数据处理
void DataHandle::PcDataHandle( UartUpData uartData)
{
    switch (uartData.commend) {
        case PC_ISSUE_DATA:

            break;

        case PC_ISSUE_INPAPER:

            break;

        case PC_ISSUE_CONFIG:

            break;
        case PC_ISSUE_UPGRADE:
            UartCommitUpGrade(uartData.databuf);
            break;
        case PC_ISSUE_VERSION:
            UartCommitVersionCompier(uartData.databuf);
            break;
        case CONFIG_TIME:

            break;
        case CONFIG_ID:

            break;
        case CONFIG_CLEAR:

            break;
        case CONFIG_GET:

            break;
        case CONFIG_UPLOAD:
            UartCommitDataHandle(uartData.databuf);
            break;
        case CONFIG_COLLECT:

            break;
        case CONFIG_NET:

            break;
        case GETCONFIG_COLLECT:
            UartConfigHandle(uartData.databuf);
            break;
        case GETCONFIG_NET:
            UartNetConfigHandle(uartData.databuf);
            break;

        case GETCONFIG_OUT:    //  获取输出配置
            UartOutConfigHandle(uartData.databuf);
            break;

        case CONFIG_OUT:     //  输出配置

            break;
        case HEARTBEAT:

            break;
        default:

            break;
    }

}
//处理串口提交的数据
void DataHandle::UartCommitDataHandle(quint8 * Databuf)
{
    CommitCollectData *_data = (CommitCollectData *)Databuf;
    PraseCollectData prasedata;
    prasedata.guid =QLatin1String((const char *)(_data-> guid));
    prasedata.dataindex =_data->dataindex;

    prasedata.alltime =_data->alltime;
    prasedata.boottime =_data->boottime;
    prasedata.stoptime =_data->stoptime;
    prasedata.faulttime =_data->faulttime;


    prasedata.bootstopsig =_data->bootstopsig;
    prasedata.faultsig =_data->faultsig;
    prasedata.pulsecount =_data->pulsecount;
    prasedata.pnopaperstate = _data->pnopaperstate;
    prasedata.timebegin =QLatin1String((const char *)(_data->timebegin));
    prasedata.timecurrent =QLatin1String((const char *)(_data->timecurrent));

    prasedata.revpulsesig = _data->revpulsesig;
    prasedata.revpulsecount = _data->revpulsecount;
    prasedata.bladepulsesig = _data->bladepulsesig;
    prasedata.bladepulsecount = _data->bladepulsecount;
    prasedata.bladepulsetime = QLatin1String((const char *)(_data->bladepulsetime));

    commitdata(prasedata);
}
//处理获取到的采集配置
void DataHandle::UartConfigHandle(quint8 * Databuf)
{
     CollectSigConfig _configdata;
     memcpy(&_configdata,Databuf,sizeof(CollectSigConfig));
     commitcollectconfig(_configdata);
}
//处理获取的网络配置
void DataHandle::UartNetConfigHandle(quint8 * Databuf)
{
     NetConfig _configdata;
     memcpy(&_configdata,Databuf,sizeof(NetConfig));
     emit commitnetconfig(_configdata);
}
//处理获取的输出配置
void DataHandle::UartOutConfigHandle(quint8 * Databuf)
{
    OutConfig _configdata;
    memcpy(&_configdata,Databuf,sizeof(OutConfig));
    emit commitoutconfig(_configdata);
}

//处理获取的升级响应
void DataHandle::UartCommitUpGrade(quint8 *Databuf)
{
    UpGradeAsk _upgradedata;
    memcpy(&_upgradedata,Databuf,sizeof(_upgradedata));
    emit timer1timeout(_upgradedata);
}

//处理获取的版本与编译信息
void DataHandle::UartCommitVersionCompier(quint8 *Databuf)
{
    VersionCompier _versionCompierdata;
    memcpy(&_versionCompierdata,Databuf,sizeof(VersionCompier));
    emit displayversioncompier(_versionCompierdata);
}
