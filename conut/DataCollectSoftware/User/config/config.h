/**
  ******************************************************************************
  * @file           : wificonfig.h
  * @brief          : Header for wificonfig.c file.
  *                   This file contains the common defines of the application.
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

#ifndef __CONFIG_H
#define __CONFIG_H

#include "stm32f4xx_hal.h"
#include "insidedata.h"
#include "collect.h"
#include "stmflash.h"
#include "pl_type.h"

#define    UUIDBUFLENGTH   40
#define    UNIQUE_ID_SIZE  (12) /*  96bits for unique id with STM32.*/
#define    CONFIGUPLOADTIME  1

/* 网络配置 */
#define    NETCONFIGTIME    2
#define    CMDSTART1        "+++"
#define    CMDSTART2        "a"
#define    BACKCMD          "AT+ENTM\r"
#define    RESTART          "AT+Z\r"  
#define    MODECMD          "AT+WMODE=%s\r"
#define    ROUTERCMD        "AT+WSTA=%s,%s\r"
#define    APNAME           "AT+WAP=11BGN,%s,AUTO\r"
#define    APPASSWORD       "AT+WAKEY=WPA2PSK,AES,%s\r"
#define    NETCMD           "AT+NETP=TCP,CLIENT,%s,%s\r"
#define    NETIPCONFIG      "AT+WANN=static,%s,%s,%s\r"
#define    NETAUTOCONFIG    "AT+WANN=DHCP\r"
#define    GETRSSI          "AT+WSLQ\r"

/* 内存一字节对齐*/
#pragma pack (1) 
/* 传感器状态 */
typedef enum _COLLECTSTATE_ { 
    INVALID = 0,          /* 无效*/
    VALID = 1,            /* 有效*/
    WAITDELAY = 2,       /* 延时等待，主要用于计数*/
}COLLECTSTATE;

/* 传感器接口配置 */
typedef enum _COLLECTIOTYPE_ {
    IO_PNP = 0,          /* 高电平有效*/
    IO_NPN = 1,          /* 低电平有效*/
    IO_HIGH_PULSE = 2,   /* 高脉冲有效*/
    IO_LOW_PULSE = 3,    /* 低脉冲有效*/	
    IO_PWM_HIGH = 4,     /* 占空比检测(以高电平为准)*/
    IO_PWM_LOW = 5,      /* 占空比检测(以低电平为准)*/
}COLLECTIOTYPE;

/* 传感器判断流程 */
typedef enum _COLLECTJUDGE_ { 
    JUDGE_EDGE  = 0,          /* 等待判边缘 */
    JUDGE_WITCH = 1,          /* 判脉冲宽度 */
    JUDGE_PWM_1 = 2,          /* 判断脉宽第一阶段 */
    JUDGE_PWM_2 = 3,          /* 判断脉宽第二阶段 */
}COLLECTJUDGE;

/* 传感器信号边沿检测 */
typedef enum _COLLECTJUDGEEDGE_  { 
    NO_EDGE  = 0,           /* 无边沿 */
    RISE_EDGE = 1,          /* 上升沿 */
    DESC_EDGE = 2,          /* 下降沿 */
}COLLECTJUDGEEDGE;

/* 计数脉冲关联判断 */
typedef enum _PULSERELATE_  { 
    RELATE_STATE  = 0,          /* 关联运行状态 */
    RELATE_REVPULSE = 1,        /* 关联预留脉冲信号和状态信号 */
}PULSERELATE;

/* 输出端口状态控制 */
typedef enum _OUTSTATE_  { 
    CLOSE  = 0,          /* 关闭 */
    OPEN = 1,        /* 打开 */
}OUTSTATE;

/* 采集信号接口 */
typedef enum _SIGINTERFACE_  { 
    BOOTSIG  = 0,          /* 开机信号 */
    STOPSIG = 1,           /* 关机信号 */
    FAULTSIG = 2,          /* 错误信号 */
    MAINPULSESIG = 3,      /* 主计数信号 */
    REVPULSESIG = 4,       /* 预计数信号 */
    BLADEPULSESIG = 5,     /* 刀版信号 */
    RESERVEDPULSESIG = 6,  /* 纸光电信号 */
     /* 2020年04月15日 新增  重新设计计数方案 */
}SIGINTERFACE;
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

//时间同步结构体
typedef struct {
  U8 WeekDay; //星期  
  U8 Year;   //年
  U8 Month;  //月
  U8 Date;   //日
  U8 Hours;  //时         
  U8 Minutes; //分       
  U8 Seconds;  //秒       
}RTC_DateType; //时间同步结构体

//ID号设置
typedef struct {
  int8_t Idnumber[1];
}ID_DateType; //设置板子ID号


//插纸时间
typedef struct {
    U16 InPaperTime; //插纸时间
    U16 InPaperAfterCut;//插纸后切断时间
    U16 StopSigType;//停机信号类型
}OutCfg; //输出配置

//配置管理结构体
typedef struct {
    U8 upload_en;  //配置数据定时上传使能
}ConfigMgr; //配置管理

#pragma pack ()
void  Config_Init(void);
void  Config_PcDataAnaly(U8 data); //配置端数据解析
void  Config_Uart_Handle(void);         //配置串口
void  Config_uart_Upgrade(void);        //串口升级
void  Config_uart_GetVersion(void);     //上传固件版本
void  Config_uart_GetCompier(void);     //上传编译时间
void  Config_TimeSynchronization(UartUpData *RxUartData);
void  Config_SetIdNumber(UartUpData *RxUartData);
void  Config_ClearNumber(UartUpData *RxUartData);
void  Config_UploadTimeCount(void);
void  Config_UploadMonitorState(void);
void  Config_Collect(UartUpData *RxUartData);
void  Config_Net(UartUpData *RxUartData);
void  Get_Collect_Config(void);
void  Get_Net_Config(void);
void  Config_InpaperEnable(UartUpData *RxUartData);

void  Get_Out_Config(void);
void  Config_Out(UartUpData *RxUartData);
#endif

/************************ (C) COPYRIGHT Hopewellgroup ***********END OF FILE***/

