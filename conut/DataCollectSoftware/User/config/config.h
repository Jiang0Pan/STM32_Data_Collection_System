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

/* �������� */
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

/* �ڴ�һ�ֽڶ���*/
#pragma pack (1) 
/* ������״̬ */
typedef enum _COLLECTSTATE_ { 
    INVALID = 0,          /* ��Ч*/
    VALID = 1,            /* ��Ч*/
    WAITDELAY = 2,       /* ��ʱ�ȴ�����Ҫ���ڼ���*/
}COLLECTSTATE;

/* �������ӿ����� */
typedef enum _COLLECTIOTYPE_ {
    IO_PNP = 0,          /* �ߵ�ƽ��Ч*/
    IO_NPN = 1,          /* �͵�ƽ��Ч*/
    IO_HIGH_PULSE = 2,   /* ��������Ч*/
    IO_LOW_PULSE = 3,    /* ��������Ч*/	
    IO_PWM_HIGH = 4,     /* ռ�ձȼ��(�Ըߵ�ƽΪ׼)*/
    IO_PWM_LOW = 5,      /* ռ�ձȼ��(�Ե͵�ƽΪ׼)*/
}COLLECTIOTYPE;

/* �������ж����� */
typedef enum _COLLECTJUDGE_ { 
    JUDGE_EDGE  = 0,          /* �ȴ��б�Ե */
    JUDGE_WITCH = 1,          /* �������� */
    JUDGE_PWM_1 = 2,          /* �ж������һ�׶� */
    JUDGE_PWM_2 = 3,          /* �ж�����ڶ��׶� */
}COLLECTJUDGE;

/* �������źű��ؼ�� */
typedef enum _COLLECTJUDGEEDGE_  { 
    NO_EDGE  = 0,           /* �ޱ��� */
    RISE_EDGE = 1,          /* ������ */
    DESC_EDGE = 2,          /* �½��� */
}COLLECTJUDGEEDGE;

/* ������������ж� */
typedef enum _PULSERELATE_  { 
    RELATE_STATE  = 0,          /* ��������״̬ */
    RELATE_REVPULSE = 1,        /* ����Ԥ�������źź�״̬�ź� */
}PULSERELATE;

/* ����˿�״̬���� */
typedef enum _OUTSTATE_  { 
    CLOSE  = 0,          /* �ر� */
    OPEN = 1,        /* �� */
}OUTSTATE;

/* �ɼ��źŽӿ� */
typedef enum _SIGINTERFACE_  { 
    BOOTSIG  = 0,          /* �����ź� */
    STOPSIG = 1,           /* �ػ��ź� */
    FAULTSIG = 2,          /* �����ź� */
    MAINPULSESIG = 3,      /* �������ź� */
    REVPULSESIG = 4,       /* Ԥ�����ź� */
    BLADEPULSESIG = 5,     /* �����ź� */
    RESERVEDPULSESIG = 6,  /* ֽ����ź� */
     /* 2020��04��15�� ����  ������Ƽ������� */
}SIGINTERFACE;
/* �豸���� */
typedef struct _DeviceConfig_ {	
    S8 guid[UUIDBUFLENGTH];              /* ����guid�� */
}DevCfg;

/* �豸���� */
typedef struct _TimeType_ {	
    U8  hr;              /* Сʱ */
    U8  min;           /* ����	 */
}TimeType;

/* �źŲɼ�����*/
typedef struct _CollectSigConfig_ {
    TimeType cl_tm;    /* ��ʱ����ʱ���  */
    U8  rt_prd;          /* ʵʱ�洢ʱ����, ���� */
    U8  up_prd;     /* ��ʱ�ϱ�ʱ��, ���� */
    U8  pulserelate;    /* ��������ж� ��ӦPULSERELATE */
    U8  bt_typ;   /* �����źŴ���������,��ӦCOLLECTIOSTATE, ����ģʽ */
    U16 bt_wd;  /* �����źſ��,MS */
    U8  st_typ;   /* �ػ��źŴ��������� */
    U16 st_wd;  /* �ػ��źſ��,MS */
    U8  ft_typ;	    /* �����źŴ��������� */
    U16 ft_wd;     /* �����źſ��,MS */
    U8  m_ch_typ;		  /* �������źŴ���������	    2020��1��12������ռ�ձȼ�� */
    U16 m_ch_wd;    /* �������źſ��,MS         �������� */
    U16 m_ch_prd_min;/* �������źż��ʱ��(��С) ռ�ձ���Сֵ������*/
    U16 m_ch_prd_max;/* �������źż��ʱ��(���) ռ�ձ����ֵ������*/

    U8  r_ch_typ;		 /* Ԥ�������źŴ���������	     2020��1��12������ռ�ձȼ�� */
    U16 r_ch_wd;    /* Ԥ�������źſ��,MS           �������� */
    U16 r_ch_prd_min;/* Ԥ�������źż��ʱ��(��С)    ռ�ձ���Сֵ */
    U16 r_ch_prd_max;/* Ԥ�������źż��ʱ��(���)    ռ�ձ����ֵ */

    U8  b_ch_typ;		  /* ���������źŴ��������� */	
    U16 b_ch_wd;      /* ���������źſ��,MS */
    U16 b_ch_prd_min;/* ���������źż��ʱ��(��С)  */
    U16 b_ch_prd_max;/* ���������źż��ʱ��(���) */
}CollectCfg;

/* ��������,����Ϊ*/
typedef struct _NetConfig_ {
    U8 mode[10];      	/* STA/AP/STA&APģʽ */
    U8 name[10];      	/* Wi-Fiģ������ */
    U8 wifiname[20];  	/* ·����WIFI����*/
    U8 wifi_pwd[20];  	/* ·����WIFI�������� */
	U8 apname[20];    	/* AP��wifi���� */
	U8 appasswd[20];    /* AP��wifi���� */
    U8 ser_ip[20];    	/* tcp/udp������ip */
    U8 ser_port[10]; 	/* tcp/udp�������˿� */
    U8 dhcp_en[10];     /* DHCP/STATIC */
    U8 ip[20];        	/* ����IP */
    U8 subnet_mask[20]; /* �������������� */
    U8 gateway[20];     /* ���������� */
}NetCfg;

//ʱ��ͬ���ṹ��
typedef struct {
  U8 WeekDay; //����  
  U8 Year;   //��
  U8 Month;  //��
  U8 Date;   //��
  U8 Hours;  //ʱ         
  U8 Minutes; //��       
  U8 Seconds;  //��       
}RTC_DateType; //ʱ��ͬ���ṹ��

//ID������
typedef struct {
  int8_t Idnumber[1];
}ID_DateType; //���ð���ID��


//��ֽʱ��
typedef struct {
    U16 InPaperTime; //��ֽʱ��
    U16 InPaperAfterCut;//��ֽ���ж�ʱ��
    U16 StopSigType;//ͣ���ź�����
}OutCfg; //�������

//���ù���ṹ��
typedef struct {
    U8 upload_en;  //�������ݶ�ʱ�ϴ�ʹ��
}ConfigMgr; //���ù���

#pragma pack ()
void  Config_Init(void);
void  Config_PcDataAnaly(U8 data); //���ö����ݽ���
void  Config_Uart_Handle(void);         //���ô���
void  Config_uart_Upgrade(void);        //��������
void  Config_uart_GetVersion(void);     //�ϴ��̼��汾
void  Config_uart_GetCompier(void);     //�ϴ�����ʱ��
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

