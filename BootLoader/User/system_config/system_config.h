/* *
  ******************************************************************************
  * @file           : system_config.h
  * @brief          : system config
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020 hopewellgroup.
  * All rights reserved.
  *
  * This software is licensed by hopewellgroup under Ultimate Liberty license
  *
  ******************************************************************************
  */
  
#ifndef __SYSTEM_CONFIG_H
#define __SYSTEM_CONFIG_H

/*  data types */
typedef signed char         S8;
typedef unsigned char       U8;
typedef short               S16;
typedef unsigned short      U16;
typedef int                 S32;
typedef unsigned            U32;
typedef long long           S64;
typedef unsigned long long  U64;
typedef volatile unsigned int VU32;

/*  define */
#define    ZERO 			(0)
#define    UUIDBUFLENGTH    (40)
#define    UNIQUE_ID_SIZE   (12) /*  96bits for unique id with STM32. */
#define    CONFIGUPLOADTIME (1)
//串口通讯结构体   							                /* 串口数据长度 		      */
#define UARTDATALENGTH 		(2564)    					/* 串口数据长度 		      */
#define UARTHEAD       		(0xFEEF) 					/* 接收头数据   		      */
#define UARTTAIL       		(0xEAAE) 					/* 接收尾数据        	  */

  
/*  global variable */
#pragma pack (1) /* 内存一字节对齐 */
/* 设备配置 */
typedef struct _DeviceConfig_ {	
    S8 guid[UUIDBUFLENGTH];              /* 盒子guid号 */
}DevCfg;

/* 设备配置 */
typedef struct _TimeType_ {	
    U8  hr;              /* 小时 */
    U8  min;           /* 分钟	 */
}TimeType;

/* 信号采集配置*/
typedef struct _CollectSigConfig_ {
    TimeType cl_tm;    /* 定时清零时间点  */
    U8  rt_prd;          /* 实时存储时间间隔, 周期 */
    U8  up_prd;     /* 定时上报时间, 周期 */
    U8  pulserelate;    /* 脉冲关联判断 对应PULSERELATE */
    U8  bt_typ;   /* 开机信号传感器类型,对应COLLECTIOSTATE, 触发模式 */
    U16 bt_wd;  /* 开机信号宽度,MS */
    U8  st_typ;   /* 关机信号传感器类型 */
    U16 st_wd;  /* 关机信号宽度,MS */
    U8  ft_typ;	    /* 错误信号传感器类型 */
    U16 ft_wd;     /* 错误信号宽度,MS */
    U8  m_ch_typ;		  /* 主脉冲信号传感器类型	    2020年1月12日增加占空比检测 */
    U16 m_ch_wd;    /* 主脉冲信号宽度,MS         周期限制 */
    U16 m_ch_prd_min;/* 主脉冲信号间隔时间(最小) 占空比最小值，周期*/
    U16 m_ch_prd_max;/* 主脉冲信号间隔时间(最大) 占空比最大值，周期*/

    U8  r_ch_typ;		 /* 预留脉冲信号传感器类型	     2020年1月12日增加占空比检测 */
    U16 r_ch_wd;    /* 预留脉冲信号宽度,MS           周期限制 */
    U16 r_ch_prd_min;/* 预留脉冲信号间隔时间(最小)    占空比最小值 */
    U16 r_ch_prd_max;/* 预留脉冲信号间隔时间(最大)    占空比最大值 */

    U8  b_ch_typ;		  /* 刀版脉冲信号传感器类型 */	
    U16 b_ch_wd;      /* 刀版脉冲信号宽度,MS */
    U16 b_ch_prd_min;/* 刀版脉冲信号间隔时间(最小)  */
    U16 b_ch_prd_max;/* 刀版脉冲信号间隔时间(最大) */
}CollectCfg;

/* 网络设置,设置为*/
typedef struct _NetConfig_ {
    U8 mode[10];      	/* STA/AP/STA&AP模式 */
    U8 name[10];      	/* Wi-Fi模组名称 */
    U8 wifiname[20];  	/* 路由器WIFI名称*/
    U8 wifi_pwd[20];  	/* 路由器WIFI名称密码 */
	U8 apname[20];    	/* AP的wifi名称 */
	U8 appasswd[20];    /* AP的wifi密码 */
    U8 ser_ip[20];    	/* tcp/udp服务器ip */
    U8 ser_port[10]; 	/* tcp/udp服务器端口 */
    U8 dhcp_en[10];     /* DHCP/STATIC */
    U8 ip[20];        	/* 本机IP */
    U8 subnet_mask[20]; /* 本机的子网掩码 */
    U8 gateway[20];     /* 本机的网关 */
}NetCfg;

//插纸时间
typedef struct {
    U16 InPaperTime; //插纸时间
    U16 InPaperAfterCut;//插纸后切断时间
    U16 StopSigType;//停机信号类型
}OutCfg; //输出配置

/* 系统配置 */
typedef struct _SystemConfig_ {
    U16          enable;   /* 使能配置 */
    U16          u16upgradeflag;   /* 升级flag, 0x8888 上位机控制 */
    DevCfg       dev_cfg;        /* 设备配置 */
    CollectCfg   col_cfg;   /* 采集信号配置 */
    NetCfg       net_cfg;        /* 网络配置  */
    OutCfg       out_cfg;        /* 输出配置 */
}SysCfg;
#pragma pack ()

/* 数据通讯结构体,用于传输数据 */
typedef struct {
    U16 header;      /* 数据头 */
    U16 devSAID;     /* 设备源ID，由控制端软件连接设备，设备自动分配，握手为0xFFFF */
    U16 devDAID;     /* 设备目标ID,发送数据给的对象 */
    U16 commend;     /* 命令 */
    U16 iLenght;     /* 数据区长度 */
    U8  databuf[UARTDATALENGTH];  /* 数据缓冲区 */
} UartUpData;/* 串口上传数据 */

/* 系统上报配置信息 */
typedef struct  
{
    U16 tailer;  /* 数据尾 */
}UartUpDatatail;

typedef struct   
{
    U8             rxbuf[UARTDATALENGTH];/* 读取数据 */
    UartUpData     RxUartData;      /* 接收数据 */
    UartUpDatatail RxUartTail;   /* 接收帧尾 */
    UartUpData     TxUartData;       /* 发送数据     */
    UartUpDatatail TxUarttail;   /* 发送帧尾 */
    UartUpData     uartBackData;      /* 返回数据 */
    /* 接收数据时的步骤指针 */
    U8 RxSts;    
    /* 数据接收完成标志 */
    U8 bComProc;
} UartMgr;/* usb上报数据 */

/* 系统状态 */
typedef struct _SystemState_
{
	 UartMgr   uart_frm;/* uart帧结构 */
}SysSts;

/*  function declaration */ 
void system_init(void);

#endif

/* *********************** (C) COPYRIGHT Hopewellgroup ***********END OF FILE***/

