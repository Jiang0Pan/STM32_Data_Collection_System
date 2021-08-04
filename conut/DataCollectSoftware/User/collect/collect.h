/**
  ******************************************************************************
  * @file           : collect.h
  * @brief          : Header for collect.c file.
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

#ifndef __COLLECT_H
#define __COLLECT_H

#include "stm32f4xx_hal.h"
#include "config.h"
/* ������Ĭ������ */
#define  BOOTSENSOR   IO_PNP  //Ĭ��ΪPNP�ӿڴ�����
#define  STOPSENSOR   IO_PNP  //Ĭ��ΪPNP�ӿڴ�����
#define  FAULTSENSOR  IO_PNP  //Ĭ��ΪPNP�ӿڴ�����

#define  MAINPULSESENSOR     IO_HIGH_PULSE  //Ĭ��ΪPNP�ӿڴ���������������Ч
#define  REVPULSESENSOR      IO_HIGH_PULSE  //Ĭ��ΪPNP�ӿڴ���������������Ч
/* 2019��11��30��  ���湦������ */
#define  BLADEPULSESENSOR    IO_HIGH_PULSE  //Ĭ��ΪPNP�ӿڴ���������������Ч

/* �������ź��˲� */
#define  COLLECTSIGWIDTH       10   //Ĭ���źſ��Ϊ10MS
#define  COLLECTSIGMININTERVAL 10  //Ĭ���źż��ʱ��Ϊ10MS,��Сֵ
#define  COLLECTSIGMAXINTERVAL 1000  //Ĭ���źż��ʱ��Ϊ1000MS,���ֵ

/* ռ�ձȼ���˲�*/
#define  COLLECTMINPERIOD   10   //Ĭ���źŵ���С����Ϊ10MS,��Ϊ�˲�

/* �����ϱ���ʱ */
#define  UPLOADTIME       10     //�豸10S���ϱ�һ��
#define  REALSTORETIME    1      //�豸1S�ӻ���һ������
#define  CLEARHOUR        6      //Ĭ������6������
#define  CLEARMINUTES     0      //Ĭ������6������
/* �ź����� */ 
#define  BOOTSIG_IN        HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)  //�����źŶ�ȡ
#define  STOPSIG_IN        HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1)  //ͣ���źŶ�ȡ
#define  FAULTSIG_IN       HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2)  //ʧЧ�źŶ�ȡ
#define  PANPAPER_IN       HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_7)  //����ֽ�źŶ�ȡ
#define  PANPAPER_GPIO     GPIO_PIN_7                       //����ֽ�ӿ�

/* �������� */
#define  MAINPULSESIG_IN      HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3)  //�������źŶ�ȡ
#define  REVPULSESIG_IN       HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_9)  //Ԥ�������źŶ�ȡ
#define  RESERVEDPULSE_GPIO         	GPIO_PIN_7
#define  READ_RESERVEDPULSE_EDGE    	HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_7)
#define  MAINPULSESIG_GPIO    GPIO_PIN_3
#define  REVPULSESIG_GPIO     GPIO_PIN_9
/* 2019��11��30��  ���湦������ L2+  L2- */
#define  BLADEPULSESIG_IN     HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_8)  //���������źŶ�ȡ
#define  BLADEPULSESIG_GPIO   GPIO_PIN_8


/* ��ƽ״̬ */
#define  HIGHLEVEL    1
#define  LOWLEVEL     0




typedef struct _RTC_DateTime_ {
    RTC_DateTypeDef sdate; /* ���� */
    RTC_TimeTypeDef stime; /* ʱ�� */
}RTC_DateTime;

/* �ɼ��źŵĸ����м�ֵ */
typedef struct _CollectSigMedian_ {	
    /* Сʱ *///�ɼ����ݲ���
    U16 all_tm;     /* �豸������ʱ�䣬�ϵ��ʱ�䣬����ͣ����ת */
    RTC_DateTime pwr_on_tm;   /* �豸����ʱ�䣬���豸�ϵ� */
    RTC_DateTime cur_tm; /* �豸��ǰʱ�� */
    U8  bt_old_sts;  /* �����źž�״̬*/
    U8  bt_trg_md;/* �����ź��о��߼���������ʽ */
    U16 bt_wd;   /* �����źſ��,MS */
    U16 bt_tm;       /* ����ʱ�䣬��λS */
    U8  st_old_sts;  /*�ػ��źž�״̬*/
    U8  st_trg_md;/* �ػ��ź��о��߼� */
    U16 st_wd; /* �ػ��źſ��,MS *///
    U16 st_tm;       /* ͣ��ʱ�䣬��λS */
    U8  ft_old_sts;  /* �����źž�״̬ */
    U8  ft_trg_md;/* �����ź��о��߼� */
    U16 ft_wd; /* �����źſ��,MS */
    U16 ft_tm;       /* ����ʱ�䣬��λS*/
    U8   m_ch_trg_md;/* �����������߼��ж� */
    U16  m_ch_wd;/* �������źſ��,MS */
    U16  m_ch_tm_cnt;/* ���������ʱ�䣬1ms�ж�һ�� */
    U8   r_ch_trg_md;/* Ԥ�����������߼��ж� */
    U16  r_ch_wd;/* Ԥ�������źſ��,MS */
    U16  r_ch_tm_cnt;/* Ԥ���������ʱ�� */
    //2019��11��30��  ���湦������
    U8   b_ch_trg_md;/* ������������߼��ж� */
    U16  b_ch_wd;/* ��������źſ��,MS */
    U16  b_ch_tm_cnt;/* ����������ʱ�� */
    //2020��01��12��  ����������Ƽ��
    /* ���жϵ�ƽʱ��,����ռ�ձ���һ����Ҫ�ж�����ʱ�� */
    U16  m_ch_pwm_h_tm;/* ���жϵ�ƽʱ��,�ߵ�ƽ���� */
    U16  m_ch_pwm_l_tm;/* ���жϵ�ƽʱ��,�͵�ƽ���� */
    U16  r_ch_pwm_h_tm;/* Ԥ���жϵ�ƽʱ��,�ߵ�ƽ���� */
    U16  r_ch_pwm_l_tm;/* Ԥ���жϵ�ƽʱ��,�͵�ƽ���� */
    /* ���Ʋ��� */
    U8   upload_en;  /* ���ݶ�ʱ�ϴ�ʹ�� */
    U8   storeenable;   /* �洢���ݶ�ʱʹ��*/
    /* 2019��11��30��  ���湦������ */
    U8   bladeenable;   /* ���������ϴ�ʹ�� */
    U8   cl_en; /* 2020��1��1��    ��ʱ�������� */
}CollectData;

/* Сʱ *///�ⲿ�źŲɼ�״̬
typedef struct _CollectSigState_
{
	  U8 bootsig;   /* �����ź�״̬,��ӦCOLLECTSTATE */
	  U8 stopsig;   /* �ػ��ź�״̬ */
	  U8 faultsig;	 /* �����ź�״̬ */
	  U8 pnopaperstate;	 /* ����ֽ�ź�״̬ */
	  
	  U8  mainpulsesig;  /* �������ź�״̬ */
	  U32 mainpulsecount;/* �������źż��� */
	
	  U8  revpulsesig;  /* Ԥ�������ź�״̬ */
	  U32 revpulsecount;/* Ԥ�������źż��� */
	
	  /* 2019��11��30��  ���湦������ */
	  U8  bladepulsesig;  /* ���������ź�״̬ */
	  U32 bladepulsecount;/* ���������źż��� */
	  RTC_TimeTypeDef bladepulsetime;
}CollectSigState;

/* ʵʱ�������� */
typedef struct _RealTimeStore_
{
    RTC_DateTime  MachineCurrent; /* ���浱ǰʱ�� */
    U16  dataindex;          /* �ϱ���Ϣ���к� */
    U8   bootstopsig;        /* �����ź�״̬, */
    U8   faultsig;	        /* �����ź�״̬ */
    U16 alltime;            /* ������ʱ�� */
    U16 bootsigtime;         /* ����ʱ�䣬��λS */
    U16 stopsigtime;        /* ͣ��ʱ�䣬��λS */ 
    U16 faultsigtime;       /* ����ʱ�䣬��λS */ 
    U32 pulsecount;          /* �������źż��� */
    /* Сʱ *//* 2019��11��30��  ���湦������ */
    U32 bladepulsecount;    /* ���������źż��� */ 
}RealTimeStore;



void  Collect_config(void);//�źų�ʼ������
void  Collect_init(void);//����źų�ʼ��
void  Collect_TimerHandle(void);//�źſ�Ⱥͼ�����
void  Collect_ExitHandle (SIGINTERFACE siginterface);//�ⲿ�źż��
void  Collect_Handle(void);//�ɼ�״̬����
void  Collect_StateTimeAggregation(void);//�豸״̬�ۼ�ʱ��
void  Collect_UploadTimeCount(void);//���ݶ�ʱ�ϱ�
void  Collect_UploadHandle(void);
U8    Collect_Judge_ClearData(RTC_DateTime _timebegin);
//��ֽ������
void  Collect_Panpaper(void);

#endif

/************************ (C) COPYRIGHT Hopewellgroup ***********END OF FILE***/

