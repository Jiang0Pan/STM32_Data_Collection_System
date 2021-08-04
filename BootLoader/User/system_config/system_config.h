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
//����ͨѶ�ṹ��   							                /* �������ݳ��� 		      */
#define UARTDATALENGTH 		(2564)    					/* �������ݳ��� 		      */
#define UARTHEAD       		(0xFEEF) 					/* ����ͷ����   		      */
#define UARTTAIL       		(0xEAAE) 					/* ����β����        	  */

  
/*  global variable */
#pragma pack (1) /* �ڴ�һ�ֽڶ��� */
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

//��ֽʱ��
typedef struct {
    U16 InPaperTime; //��ֽʱ��
    U16 InPaperAfterCut;//��ֽ���ж�ʱ��
    U16 StopSigType;//ͣ���ź�����
}OutCfg; //�������

/* ϵͳ���� */
typedef struct _SystemConfig_ {
    U16          enable;   /* ʹ������ */
    U16          u16upgradeflag;   /* ����flag, 0x8888 ��λ������ */
    DevCfg       dev_cfg;        /* �豸���� */
    CollectCfg   col_cfg;   /* �ɼ��ź����� */
    NetCfg       net_cfg;        /* ��������  */
    OutCfg       out_cfg;        /* ������� */
}SysCfg;
#pragma pack ()

/* ����ͨѶ�ṹ��,���ڴ������� */
typedef struct {
    U16 header;      /* ����ͷ */
    U16 devSAID;     /* �豸ԴID���ɿ��ƶ���������豸���豸�Զ����䣬����Ϊ0xFFFF */
    U16 devDAID;     /* �豸Ŀ��ID,�������ݸ��Ķ��� */
    U16 commend;     /* ���� */
    U16 iLenght;     /* ���������� */
    U8  databuf[UARTDATALENGTH];  /* ���ݻ����� */
} UartUpData;/* �����ϴ����� */

/* ϵͳ�ϱ�������Ϣ */
typedef struct  
{
    U16 tailer;  /* ����β */
}UartUpDatatail;

typedef struct   
{
    U8             rxbuf[UARTDATALENGTH];/* ��ȡ���� */
    UartUpData     RxUartData;      /* �������� */
    UartUpDatatail RxUartTail;   /* ����֡β */
    UartUpData     TxUartData;       /* ��������     */
    UartUpDatatail TxUarttail;   /* ����֡β */
    UartUpData     uartBackData;      /* �������� */
    /* ��������ʱ�Ĳ���ָ�� */
    U8 RxSts;    
    /* ���ݽ�����ɱ�־ */
    U8 bComProc;
} UartMgr;/* usb�ϱ����� */

/* ϵͳ״̬ */
typedef struct _SystemState_
{
	 UartMgr   uart_frm;/* uart֡�ṹ */
}SysSts;

/*  function declaration */ 
void system_init(void);

#endif

/* *********************** (C) COPYRIGHT Hopewellgroup ***********END OF FILE***/

