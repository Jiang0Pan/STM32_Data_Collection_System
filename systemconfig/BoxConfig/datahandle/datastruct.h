#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include<QByteArray>
#include<QString>

//串口通讯结构体
#define UARTDATALENGTH 2564    //串口数据长度
#define UARTHEAD       0xFEEF //接收头数据
#define UARTTAIL       0xEAAE //接收尾数据


#define DEVLOCALID      0x8888 //监控设备ID号
#define HANDSHAKEID     0x9999 //本机未分配前的ID号

#define    UUIDBUFLENGTH   40
#define    TIMEBUFLENGTH   30

//数据类型
typedef enum __PCDATA_TYPE__ {
         PC_ISSUE_DATA     = 0x0001,      //PC下发检测数据
         PC_ISSUE_INPAPER  = 0x0002,      //PC下发插纸机控制
         PC_ISSUE_CONFIG   = 0x0003,      //PC下发配置命令
         PC_ISSUE_UPGRADE  = 0x0004,      //PC下发升级命令
         PC_ISSUE_VERSION  = 0x0005,      //PC下发获取版本、编译时间命令
         PC_ISSUE_WIFISUS  = 0x0006,      //PC下发获取WIFI状态
           //配置软件
         CONFIG_TIME       = 0x0101,      //  同步时间
         CONFIG_ID         = 0x0102,      //  配置ID号
         CONFIG_CLEAR      = 0x0103,      //  计数清零
         CONFIG_GET        = 0x0104,      //  获取配置
         CONFIG_UPLOAD     = 0x0105,      //  配置上传
         CONFIG_COLLECT    = 0x0106,      //  采集参数配置
         CONFIG_NET        = 0x0107,      //  网络配置

         GETCONFIG_COLLECT = 0x0108,      //  获取采集配置
         GETCONFIG_NET     = 0x0109,      //  获取网络配置
         GETCONFIG_OUT     = 0x010A,      //  获取输出配置
         CONFIG_OUT        = 0x010B,      //  输出配置

         HEARTBEAT         = 0x8001,      //心跳

} PCDATATYPE;

//数据通讯结构体，用于传输数据
typedef struct
{
   quint16 header;      //数据头
   quint16 devSAID;     //设备源ID号，握手的设备为0x9999
   quint16 devDAID;     //设备目标ID号，默认为0x8888，握手后会分配ID号
   quint16 commend;     //命令
   quint16 iLenght;               //数据区长度
   quint8 databuf[UARTDATALENGTH];  //数据BUF
   quint16 tailer;       //数据尾
} UartUpData;//

//串口命令回复
typedef struct
{
   quint16 header;      //数据头
   quint16 commend;     //数据命令
   quint16 tailer;      //数据尾
} UartReplyCmd;         //串口回复命令


//串口接收管理
typedef struct
{
   UartUpData uartData;     //串口数据
   quint8     RxSts;        //串口解析步骤
} UartMgr;//串口接收管理

//设置时间结构体
typedef struct
{
    uint8_t WeekDay; //星期
    uint8_t Year;   //年
    uint8_t Month;  //月
    uint8_t Date;   //日
    uint8_t Hours;  //时
    uint8_t Minutes; //分
    uint8_t Seconds;  //秒
}RTC_DateType; //时间结构体

#pragma pack(push) //保存对齐状态
#pragma pack(1)//设定为1字节对齐
//传感器状态
typedef enum _COLLECTSTATE_
{
     INVALID = 0,          //无效
     VALID = 1,            //有效
     WAITDELAY = 2,        //等待延时，用于计数
}COLLECTSTATE;

//传感器接口配置
typedef enum _COLLECTIOTYPE_
{
     IO_NPN = 0,          //低电平有效
     IO_PNP = 1,          //高电平有效
     IO_HIGH_PULSE = 2,   //高脉冲有效
     IO_LOW_PULSE = 3,    //低脉冲有效
}COLLECTIOTYPE;

//传感器判断流程
typedef enum _COLLECTJUDGE_
{
     JUDGE_EDGE  = 0,          //等待边沿检测
     JUDGE_WITCH = 1,          //判脉冲宽度
}COLLECTJUDGE;

//传感器信号边沿检测
typedef enum _COLLECTJUDGEEDGE_   //边沿检测
{
     NO_EDGE  = 0,           //无边沿
     RISE_EDGE = 1,          //上升沿
     DESC_EDGE = 2,          //下降沿
}COLLECTJUDGEEDGE;

//脉冲计数关联判断
typedef enum _PULSERELATE_
{
     RELATE_STATE  = 0,          //关联运行状态
     RELATE_REVPULSE = 1,        //关联两个脉冲
}PULSERELATE;
typedef struct _CommitCollectData_
{
     int8_t    guid[UUIDBUFLENGTH];
     uint16_t  dataindex;

     uint16_t  alltime;
     uint16_t  boottime;
     uint16_t  stoptime;
     uint16_t  faulttime;


     uint8_t   bootstopsig;
     uint8_t   faultsig;
     uint32_t  pulsecount;

     int8_t   timebegin[TIMEBUFLENGTH];
     int8_t   timecurrent[TIMEBUFLENGTH];
     uint8_t   pnopaperstate;

     /* 2020年06月13日  新增质检、换膜计数 */
     /* 2020年06月13日  新增质检、换膜计数 */
     uint8_t	revpulsesig;  /* 质检信号状态 */
     uint32_t   revpulsecount;/* 质检计数 */
     uint8_t	bladepulsesig;	/* 换模信号状态 */
     uint32_t   bladepulsecount;/* 换模计数 */
     int8_t    bladepulsetime[TIMEBUFLENGTH];/* 换模时间 */

}CommitCollectData;


//时间
typedef struct _TimeType_
{
    uint8_t  hour;              //小时
    uint8_t  minutes;           //分钟

}TimeType;

//采集信号配置
typedef struct _CollectSigConfig_
{
      TimeType cleartime;     //清除时间
      uint8_t realtime_store; //实时存储时间间隔
      uint8_t uploadtime;     //上传时间
      uint8_t pulserelate;    //关联判断

      uint8_t  bootsigtype;   //开机传感器类型
      uint16_t bootsigwidth;  //开机信号宽度

      uint8_t  stopsigtype;      //关机信号传感器类型
      uint16_t stopsigwidth;     //关机信号宽度

      uint8_t  faultsigtype;	    //错误信号传感器类型
      uint16_t faultsigwidth;        //错误信号宽度

      uint8_t  mainpulsesigtype;		  //主脉冲信号传感器类型
      uint16_t mainpulsesigwidth;         //主脉冲信号宽度,MS
      uint16_t mainpulsesigmininterval;   //主脉冲间隔最小时间
      uint16_t mainpulsesigmaxinterval;   //主脉冲间隔最大时间

      uint8_t  revpulsesigtype;		   //副脉冲信号传感器类型
      uint16_t revpulsesigwidth;       //副脉冲信号宽度
      uint16_t revpulsesigmininterval; //副脉冲间隔最小时间
      uint16_t revpulsesigmaxinterval; //副脉冲间隔最大时间

}CollectSigConfig;

//网络设置
typedef struct _NetConfig_
{
      uint8_t  devmode[10];
      uint8_t  devname[10];
      uint8_t  wifiname[20];
      uint8_t  wifipassword[20];
      uint8_t  apname[20];
      uint8_t  appassword[20];
      uint8_t  serverip[20];
      uint8_t  serverport[10];
      uint8_t  dhcp_en[10];      /* 1, DHCP; 0, STATIC */
      uint8_t  localip[20];      /* 本机IP              */
      uint8_t  localmask[20];    /* 子网掩码             */
      uint8_t  localgateway[20]; /* 网关                */
}NetConfig;

//输出配置
typedef struct
{
  uint16_t InPaperTime; //插纸时间
  uint16_t InPaperAfterCut;//插纸后剪断时间
  uint16_t StopSigType;//停机信号类型
}OutConfig;

typedef struct _UpGradeAsk_ {
    uint8_t gradeask[10
    ];   /* 升级的响应缓冲区 */
}UpGradeAsk; /* 升级 */

typedef struct _VersionCompier_ {
    uint8_t version[8];   /* 版本信息 */
    uint8_t compier[20];  /* 编译时间 */
}VersionCompier; /* 版本和编译时间 */

typedef struct _WifiStatus_ {
    uint8_t wifistarssi[4]; /* wifi的STA的信号强度 */
    uint8_t wifistaconsts[15]; /* wifi的STA连接状态 */
    uint8_t wifisocasts[10]; /* wifi的socket_A的TCP连接状态 */
    uint8_t wifisocbsts[10]; /* wifi的socket_B的TCP连接状态 */
}WifiStatus;/* wifi状态信息 */

#pragma pack(pop)//恢复对齐状态

typedef struct _PraseCollectData_
{
     QString    guid;
     uint16_t   dataindex;

     uint16_t  alltime;
     uint16_t  boottime;
     uint16_t  stoptime;
     uint16_t  faulttime;


     uint8_t   bootstopsig;
     uint8_t   faultsig;
     uint32_t  pulsecount;

     QString   timebegin;
     QString   timecurrent;

     uint8_t  pnopaperstate;

     /* 2020年06月13日  新增质检、换膜计数 */
     /* 2020年06月13日  新增质检、换膜计数 */
     uint8_t	revpulsesig;  /* 质检信号状态 */
     uint32_t   revpulsecount;/* 质检计数 */
     uint8_t	bladepulsesig;	/* 换模信号状态 */
     uint32_t   bladepulsecount;/* 换模计数 */
     QString    bladepulsetime;/* 换模计数 */
}PraseCollectData;


#endif // DATASTRUCT_H
