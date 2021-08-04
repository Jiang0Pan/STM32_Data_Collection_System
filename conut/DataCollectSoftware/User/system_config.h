/**************************************************************************//**
 * @file     system_config.h
 * @brief    系统配置头文件，用于GPIO、全局变量等的声明
 * @version  V1.0
 * @date     20/09/2019
 * @Author   张伟
 * @note
 *
 ******************************************************************************/
#ifndef __SYSTEM_CONFIG_H
#define __SYSTEM_CONFIG_H

#include "stm32f4xx_hal.h"
#include "stdio.h"


//硬件部分
#include "interruptservice.h"
#include "wifiuart.h"


//采集接口头文件
#include "collect.h"

//配置接口头文件
#include "config.h"

//字节通讯协议
#include "insidedata.h"


//JSON协议头文件
#include "cJSON.h"
#include "malloc.h"
#include "outsidedata.h"

//FLASH
#include "stmflash.h"

//外部控制
#include "outcontrol.h"
#include "pl_type.h"


//初始化参数
#define  DEVID             "1234567890"
/*
	版本信息: A.B.C
	A: 表示重大版本发布，涉及很多功能或者问题解决。
	B: 模块功能完善或者添加。
	C: 常规bug修改。
*/
#define  Firmware_Version  "1.0.1"
#define  APPASK            "APP:OK"

//其他
#define  VERSION_MAX       (100)           //版本信息数组大小
#define  COPMIER_MAX       (20)            //编译信息数组大小
#define  UPGRAGE_FLAG      (0x8888)        //置位升级标志位
#define  BOOTFLAG_ADDR     (0x0800FC04)    //升级标志位存放的地址
#define  FLASH_ENV_ADDR    (0x080E0000)    //最后128K给环境变量

//BOOL状态定义
typedef enum _STATE_ { 
    FALSE = 0,          //无效
    TRUE = 1,            //有效
}STATE;


#pragma pack (1) //内存一字节对齐
/* 系统配置 */
typedef struct _SystemConfig_ {
    U16           enable;   /* 使能配置 */
    U16          u16upgradeflag;   /* 升级flag, 0x8888 上位机控制 */
    DevCfg       dev_cfg;        /* 设备配置 */
    CollectCfg   col_cfg;   /* 采集信号配置 */
    NetCfg       net_cfg;        /* 网络配置  */
    OutCfg       out_cfg;        /* 输出配置 */
}SysCfg;
#pragma pack ()


/* 系统状态 */
typedef struct _SystemState_
{
	 //信号采集部分
	 CollectData        col_info; /* 采集信号中间状态 */
	 CollectSigState    col_sts;  /* 采集信号最终状态  */
	 CommitCollectData  commit_data;       /* 采集器定时提交数据 */
	 RealTimeStore      realtime_data;     /* 实时数据存储 */
	 //输出控制部分
	 InPaperMgr         inpaper_mgr;       /* 插纸管理 */
	 //停机部分
	 StopCtrCmd         stopCtrCmd;        /* 停机命令 */
	 StopCtrCmdBack     stopCtrCmdBack;    /* 停机命令反馈 */
	 StopMgr            stop_mgr;          /* 停机控制部分,主要用于延时操作 */
	 //指示灯部分
	 //通讯部分
	 JsonUartMgr wifi_cfg_mgr;
	 WifiMgr  wifi_mgr;
	 //配置部分
	 UartMgr   uart_frm;/* uart帧结构 */
	 ConfigMgr cfg_mgr;
	
}SysSts;

extern void system_init(void);
extern SysCfg g_sys_cfg;
extern SysSts g_sys_sts;
extern RTC_HandleTypeDef hrtc;

#endif
