/**
  ******************************************************************************
  * @file           : outsidedata.h
  * @brief          : upload data control
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

#ifndef __OUTSIDEDATA_H
#define __OUTSIDEDATA_H

#include "stm32f4xx_hal.h"
#include  <string.h>
#include  "config.h"

/* JSON全局配置 */
#define   JSONLENGTH     400

/* 采集器部分JSON */
#define    TIMEBUFLENGTH   30

#define    COLLECTDATA     "CollectData" /* 采集器 */

#define    COLLECTCLEAR    "CollectClear" /* 采集器清除数据 */

#define    MACHUUID        "mid"        /* 机器ID号 */

#define    UPINDEX         "uindex"     /* 上传的序列号 */

#define    MACHATIME       "mat"       /* 机器总运行时间 */

#define    MACHBTIME       "mbt"       /* 机器正常运行时间 */

#define    MACHSTIME       "mst"       /* 机器停机时间 */

#define    MACHFTIME       "mft"       /* 机器故障时间 */

#define    MACHSTA         "mbs"       /* 机器状态开机或者停机 */

#define    MACHFAU         "mf"         /* 机器故障 */

#define    MACHCOUNT       "mc"         /* 机器计数 */

#define    MACHTIMEB       "mtb"       /* 机器开机时间(实时时间) */

#define    MACHTIMEC       "mtc"         /* 机器当前时间(当前数据上传时间) */

/* 2019年11月30日   刀版数据 */
#define    BLADEDATA     "BladeData" /* 刀版数据 */
#define    BLADECOUNT    "bc"        /* 刀版计数 */

/* 2019年12月3日    停机控制数据 */

#define    CONMACH       "Conmach" /* 执行停机操作 */
#define    UUID          "uuid"   /* 设备UUID */
#define    STATUS        "status" /* 控制状态 */
#define    MODE          "mode"   /* 模式 */
#define    OPERATEID     "operateid" /* 控制端执行流水号 */
#define    OPERATETIME   "operatetime" /* 接收到信号后延迟时间MS */

#define    CONMACHRS     "ConmachRs" /* 执行结果反馈 */
#define    UINDEX        "uindex"   /* 控制执行序列 */
#define    OPTIME        "optime"   /* 执行操作的时间 */
#define    MSGSTATUS     "msgstatus" /* 信号状态 */
#define    MSG           "msg"/* 备注说明 */

#define    SUCCESS       "success" /* 成功 */ 

#define    DATAFORMATERROR          "data formatting error" /* 数据格式错误 */
#define    UUIDERROR                "uuid error" /* 设备ID错误 */


#pragma pack (1) /* 内存一字节对齐 */
/* 提交采集数据 */
typedef struct _CommitCollectData_ {	
    int8_t    guid[UUIDBUFLENGTH];  /* 提交的盒子guid号 */
    uint16_t  dataindex;            /* 上报信息序列号 */
    uint16_t  alltime;              /* 机器总时间 */
    uint16_t  boottime;             /* 正常开机总时间 */
    uint16_t  stoptime;             /* 停机总时间 */
    uint16_t  faulttime;            /* 故障总时间 */
    uint8_t   bootstopsig;          /* 设备状态开机或者停机 */
    uint8_t   faultsig;	           /* 错误信号状态 */	
    uint32_t  pulsecount;           /* 脉冲数 */
    int8_t    timebegin[TIMEBUFLENGTH];  /* 开机时间 2019-12-02 12:12:12 */
    int8_t    timecurrent[TIMEBUFLENGTH];/* 设备当前时间 2019-12-02 12:12:12 */
    uint8_t   pnopaperstate;	   /* 正反纸信号状态 */	
	
	/* 2020年06月13日  新增质检、换膜计数 */
	U8	revpulsesig;  /* 质检信号状态 */
	U32 revpulsecount;/* 质检计数 */
	U8	bladepulsesig;	/* 换模信号状态 */
	U32 bladepulsecount;/* 换模计数 */
	int8_t bladepulsetime[TIMEBUFLENGTH];/* 换模时间 */
}CommitCollectData;

/* 停机控制指令 */
typedef struct
{
   int8_t  uuid[UUIDBUFLENGTH];  /* 盒子guid号 */
	 int8_t  status;               /* 1为开机，0为停机 */
	 int8_t  mode;                 /* 0:及时执行,1为延时执行 */
	 int8_t  operateid[UUIDBUFLENGTH];/* 控制端的执行流水号 */
	 uint32_t operatetime;         /* 延迟操作时间，单位MS */
}StopCtrCmd;/* 停机控制指令 */

/* 停机控制指令反馈 */
typedef struct {
    int8_t   uuid[UUIDBUFLENGTH];     /* 盒子guid号 */
    int8_t   operateid[UUIDBUFLENGTH];/* 控制端的执行流水号 */
    uint32_t uindex;                  /* 操作序列号 */
    int8_t   optime[30];              /* 执行操作时间 */
    int8_t   msgstatus;               /* 0操作成功，1操作失败 */
    int8_t   msginfo[UUIDBUFLENGTH];  /* 消息信息 */
}StopCtrCmdBack;/* 停机控制指令反馈 */

typedef struct _UpGradeAsk_ {
	uint8_t gradeask[8];   /* 升级的响应缓冲区 */
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

#pragma pack ()

char * CommitCollectData_to_cJSON( char * cJsonRoom, CommitCollectData collectdata,uint8_t datatype);
void   CommitCollectData_Upload(void);
void   CommitBladeData_Upload(void);
uint8_t  CJSON_to_StopCtrCmd( char * json_string,StopCtrCmd * stopctr);
char * CommitStopData_to_cJSON( char * cJsonRoom, StopCtrCmdBack stopdata);
void CommitStopData_Upload(void);
void CommitClearData_Upload(void);

#endif

/************************ (C) COPYRIGHT Hopewellgroup ***********END OF FILE***/

