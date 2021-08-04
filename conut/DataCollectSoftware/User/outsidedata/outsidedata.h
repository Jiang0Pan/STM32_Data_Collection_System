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

/* JSONȫ������ */
#define   JSONLENGTH     400

/* �ɼ�������JSON */
#define    TIMEBUFLENGTH   30

#define    COLLECTDATA     "CollectData" /* �ɼ��� */

#define    COLLECTCLEAR    "CollectClear" /* �ɼ���������� */

#define    MACHUUID        "mid"        /* ����ID�� */

#define    UPINDEX         "uindex"     /* �ϴ������к� */

#define    MACHATIME       "mat"       /* ����������ʱ�� */

#define    MACHBTIME       "mbt"       /* ������������ʱ�� */

#define    MACHSTIME       "mst"       /* ����ͣ��ʱ�� */

#define    MACHFTIME       "mft"       /* ��������ʱ�� */

#define    MACHSTA         "mbs"       /* ����״̬��������ͣ�� */

#define    MACHFAU         "mf"         /* �������� */

#define    MACHCOUNT       "mc"         /* �������� */

#define    MACHTIMEB       "mtb"       /* ��������ʱ��(ʵʱʱ��) */

#define    MACHTIMEC       "mtc"         /* ������ǰʱ��(��ǰ�����ϴ�ʱ��) */

/* 2019��11��30��   �������� */
#define    BLADEDATA     "BladeData" /* �������� */
#define    BLADECOUNT    "bc"        /* ������� */

/* 2019��12��3��    ͣ���������� */

#define    CONMACH       "Conmach" /* ִ��ͣ������ */
#define    UUID          "uuid"   /* �豸UUID */
#define    STATUS        "status" /* ����״̬ */
#define    MODE          "mode"   /* ģʽ */
#define    OPERATEID     "operateid" /* ���ƶ�ִ����ˮ�� */
#define    OPERATETIME   "operatetime" /* ���յ��źź��ӳ�ʱ��MS */

#define    CONMACHRS     "ConmachRs" /* ִ�н������ */
#define    UINDEX        "uindex"   /* ����ִ������ */
#define    OPTIME        "optime"   /* ִ�в�����ʱ�� */
#define    MSGSTATUS     "msgstatus" /* �ź�״̬ */
#define    MSG           "msg"/* ��ע˵�� */

#define    SUCCESS       "success" /* �ɹ� */ 

#define    DATAFORMATERROR          "data formatting error" /* ���ݸ�ʽ���� */
#define    UUIDERROR                "uuid error" /* �豸ID���� */


#pragma pack (1) /* �ڴ�һ�ֽڶ��� */
/* �ύ�ɼ����� */
typedef struct _CommitCollectData_ {	
    int8_t    guid[UUIDBUFLENGTH];  /* �ύ�ĺ���guid�� */
    uint16_t  dataindex;            /* �ϱ���Ϣ���к� */
    uint16_t  alltime;              /* ������ʱ�� */
    uint16_t  boottime;             /* ����������ʱ�� */
    uint16_t  stoptime;             /* ͣ����ʱ�� */
    uint16_t  faulttime;            /* ������ʱ�� */
    uint8_t   bootstopsig;          /* �豸״̬��������ͣ�� */
    uint8_t   faultsig;	           /* �����ź�״̬ */	
    uint32_t  pulsecount;           /* ������ */
    int8_t    timebegin[TIMEBUFLENGTH];  /* ����ʱ�� 2019-12-02 12:12:12 */
    int8_t    timecurrent[TIMEBUFLENGTH];/* �豸��ǰʱ�� 2019-12-02 12:12:12 */
    uint8_t   pnopaperstate;	   /* ����ֽ�ź�״̬ */	
	
	/* 2020��06��13��  �����ʼ졢��Ĥ���� */
	U8	revpulsesig;  /* �ʼ��ź�״̬ */
	U32 revpulsecount;/* �ʼ���� */
	U8	bladepulsesig;	/* ��ģ�ź�״̬ */
	U32 bladepulsecount;/* ��ģ���� */
	int8_t bladepulsetime[TIMEBUFLENGTH];/* ��ģʱ�� */
}CommitCollectData;

/* ͣ������ָ�� */
typedef struct
{
   int8_t  uuid[UUIDBUFLENGTH];  /* ����guid�� */
	 int8_t  status;               /* 1Ϊ������0Ϊͣ�� */
	 int8_t  mode;                 /* 0:��ʱִ��,1Ϊ��ʱִ�� */
	 int8_t  operateid[UUIDBUFLENGTH];/* ���ƶ˵�ִ����ˮ�� */
	 uint32_t operatetime;         /* �ӳٲ���ʱ�䣬��λMS */
}StopCtrCmd;/* ͣ������ָ�� */

/* ͣ������ָ��� */
typedef struct {
    int8_t   uuid[UUIDBUFLENGTH];     /* ����guid�� */
    int8_t   operateid[UUIDBUFLENGTH];/* ���ƶ˵�ִ����ˮ�� */
    uint32_t uindex;                  /* �������к� */
    int8_t   optime[30];              /* ִ�в���ʱ�� */
    int8_t   msgstatus;               /* 0�����ɹ���1����ʧ�� */
    int8_t   msginfo[UUIDBUFLENGTH];  /* ��Ϣ��Ϣ */
}StopCtrCmdBack;/* ͣ������ָ��� */

typedef struct _UpGradeAsk_ {
	uint8_t gradeask[8];   /* ��������Ӧ������ */
}UpGradeAsk; /* ���� */

typedef struct _VersionCompier_ {
	uint8_t version[8];   /* �汾��Ϣ */ 
	uint8_t compier[20];  /* ����ʱ�� */
}VersionCompier; /* �汾�ͱ���ʱ�� */

typedef struct _WifiStatus_ {
	uint8_t wifistarssi[4]; /* wifi��STA���ź�ǿ�� */
	uint8_t wifistaconsts[15]; /* wifi��STA����״̬ */
	uint8_t wifisocasts[10]; /* wifi��socket_A��TCP����״̬ */
	uint8_t wifisocbsts[10]; /* wifi��socket_B��TCP����״̬ */
}WifiStatus;/* wifi״̬��Ϣ */

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

