/**
  ******************************************************************************
  * @file           : insidedata.h
  * @brief          : Header for insidedata.c file.
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


#ifndef __UARTDATA_H
#define __UARTDATA_H

#include "stm32f4xx_hal.h"
#include "pl_type.h"

//串口通讯结构体
#define UARTDATALENGTH 400    //串口数据长度
#define UARTHEAD       0xFEEF //接收头数据
#define UARTTAIL       0xEAAE //接收尾数据

#define UARTSKEW       5*sizeof(U16)  //数据头到数据的偏移
#define LOCALID        0x0000         //本机ID
#define PCID           0x0001         //PCID

typedef enum __DATA_CMD__ { 
    PC_ISSUE_DATA     = 0x0001,     //PC下发检测数据
    PC_ISSUE_INPAPER  = 0x0002,     //PC下发控制命令
    PC_ISSUE_CONFIG   = 0x0003,     //PC下发配置命令
	PC_ISSUE_UPGRADE  = 0x0004, 	 //PC下发升级命令
	PC_ISSUE_VERSION  = 0x0005, 	 //PC下发获取版本、编译时间命令
	PC_ISSUE_WIFISUS  = 0x0006, 	 //PC下发获取WIFI状态

    //配置软件
    CONFIG_TIME       = 0x0101,      //  同步时间
    CONFIG_ID         = 0x0102,      //  配置ID号
    CONFIG_CLEAR      = 0x0103,      //  计数清零
    CONFIG_GET        = 0x0104,      //  获取配置
    CONFIG_UPLOAD     = 0x0105,      //  配置上传
    CONFIG_COLLECT    = 0x0106,      //  采集参数配置
    CONFIG_NET        = 0x0107,      //  采集器网络配置
    GETCONFIG_COLLECT = 0x0108,      //  获取配置采集
    GETCONFIG_NET     = 0x0109,      //  获取网络配置
    GETCONFIG_OUT     = 0x010A,      //  获取输出配置
    CONFIG_OUT        = 0x010B,      //  配置输出
    PLATTOVISION      = 0x0201,      //  平台向视觉下发
    VISIONTOPLAT      = 0x0202,      //  视觉向平台上报
    HEARTBEAT         = 0x8001,       //心跳
} DATACMD; 

typedef enum __PC_CONTROL_CMD__ { 
    VIEW     = 0x0001,     //PC下发检测数据
    BOOT     = 0x0002,     //PC下发开机
    STOP     = 0x0003,     //PC下发停机
    INPAPER  = 0x0004,     //PC下发插纸     
} CONTROLCMD; 

//视觉上报数据
typedef struct
{
   S8   *data;
}PcIssueData;

//视觉上报控制
typedef struct {
    U16 controlcommend;     //命令，参考CONTROLCMD
    S8   *data;	//INPAPER：第一个字节为张数，VIEW:是字符串
}PcIssueControl;

//视觉配置数据
typedef struct {
   S8   *data;
}PcIssueConfig;

//数据通讯结构体,用于传输数据
typedef struct {
    U16 header;      //数据头
    U16 devSAID;     //设备源ID，由控制端软件连接设备，设备自动分配，握手为0xFFFF
    U16 devDAID;     //设备目标ID,发送数据给的对象
    U16 commend;     //命令
    U16 iLenght;     //数据区长度
    U8 databuf[UARTDATALENGTH];  //数据缓冲区
} UartUpData;//串口上传数据

//系统上报配置信息
typedef struct  
{
    U16 tailer;  //数据尾
}UartUpDatatail;


typedef struct   
{
    U8             rxbuf[UARTDATALENGTH];//读取数据
    UartUpData     RxUartData;      //接收数据
    UartUpDatatail RxUartTail;   //接收帧尾
    UartUpData     TxUartData;       //发送数据 
    UartUpDatatail TxUarttail;   //发送帧尾
    UartUpData     uartBackData;      //返回数据
    //接收数据时的步骤指针
    U8 RxSts;    
    //数据接收完成标志
    U8 bComProc;
} UartMgr;//usb上报数据


typedef struct   
{
    U8    rxbuf[UARTDATALENGTH];//读取数据
    U8    JsonDatabuf[UARTDATALENGTH];//读取数据
    U8    RxUartData;               //接收数据
    U8    RxState;                  //数据接收状态
    //接收数据时的步骤指针
    U16   RxStsLength;    
    U16   JsonStsLength;
    //数据接收完成标志
    U8    bComProc;
} JsonUartMgr;//Json上报数据



typedef struct  
{
    U8  S_Number[30];//编号
    U8  S_Name[30];//用户名
}PlatToVision;
typedef struct  
{
    U32 NumberOfRejects;//废品数
    U32 NumberOfQuality;//正品数   
    U8  S_Number[30];//编号
    U8  S_Name[30];//用户名
}VisionToPlat;


U8 InsideDataHandle(UartUpData *p_updata,UartUpData *p_backdata);

#endif

/************************ (C) COPYRIGHT Hopewellgroup ***********END OF FILE***/

