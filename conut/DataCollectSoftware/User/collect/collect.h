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
/* 传感器默认类型 */
#define  BOOTSENSOR   IO_PNP  //默认为PNP接口传感器
#define  STOPSENSOR   IO_PNP  //默认为PNP接口传感器
#define  FAULTSENSOR  IO_PNP  //默认为PNP接口传感器

#define  MAINPULSESENSOR     IO_HIGH_PULSE  //默认为PNP接口传感器，高脉冲有效
#define  REVPULSESENSOR      IO_HIGH_PULSE  //默认为PNP接口传感器，高脉冲有效
/* 2019年11月30日  刀版功能新增 */
#define  BLADEPULSESENSOR    IO_HIGH_PULSE  //默认为PNP接口传感器，高脉冲有效

/* 传感器信号滤波 */
#define  COLLECTSIGWIDTH       10   //默认信号宽度为10MS
#define  COLLECTSIGMININTERVAL 10  //默认信号间隔时间为10MS,最小值
#define  COLLECTSIGMAXINTERVAL 1000  //默认信号间隔时间为1000MS,最大值

/* 占空比检测滤波*/
#define  COLLECTMINPERIOD   10   //默认信号的最小周期为10MS,作为滤波

/* 数据上报定时 */
#define  UPLOADTIME       10     //设备10S钟上报一次
#define  REALSTORETIME    1      //设备1S钟缓存一次数据
#define  CLEARHOUR        6      //默认上午6点清零
#define  CLEARMINUTES     0      //默认上午6点清零
/* 信号输入 */ 
#define  BOOTSIG_IN        HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)  //开机信号读取
#define  STOPSIG_IN        HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1)  //停机信号读取
#define  FAULTSIG_IN       HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2)  //失效信号读取
#define  PANPAPER_IN       HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_7)  //正反纸信号读取
#define  PANPAPER_GPIO     GPIO_PIN_7                       //正反纸接口

/* 脉冲输入 */
#define  MAINPULSESIG_IN      HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3)  //主脉冲信号读取
#define  REVPULSESIG_IN       HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_9)  //预留脉冲信号读取
#define  RESERVEDPULSE_GPIO         	GPIO_PIN_7
#define  READ_RESERVEDPULSE_EDGE    	HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_7)
#define  MAINPULSESIG_GPIO    GPIO_PIN_3
#define  REVPULSESIG_GPIO     GPIO_PIN_9
/* 2019年11月30日  刀版功能新增 L2+  L2- */
#define  BLADEPULSESIG_IN     HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_8)  //刀版脉冲信号读取
#define  BLADEPULSESIG_GPIO   GPIO_PIN_8


/* 电平状态 */
#define  HIGHLEVEL    1
#define  LOWLEVEL     0




typedef struct _RTC_DateTime_ {
    RTC_DateTypeDef sdate; /* 日期 */
    RTC_TimeTypeDef stime; /* 时间 */
}RTC_DateTime;

/* 采集信号的辅助中间值 */
typedef struct _CollectSigMedian_ {	
    /* 小时 *///采集数据部分
    U16 all_tm;     /* 设备运行总时间，上电的时间，运行停机空转 */
    RTC_DateTime pwr_on_tm;   /* 设备开机时间，即设备上电 */
    RTC_DateTime cur_tm; /* 设备当前时间 */
    U8  bt_old_sts;  /* 开机信号旧状态*/
    U8  bt_trg_md;/* 开机信号判决逻辑，触发方式 */
    U16 bt_wd;   /* 开机信号宽度,MS */
    U16 bt_tm;       /* 开机时间，单位S */
    U8  st_old_sts;  /*关机信号旧状态*/
    U8  st_trg_md;/* 关机信号判决逻辑 */
    U16 st_wd; /* 关机信号宽度,MS *///
    U16 st_tm;       /* 停机时间，单位S */
    U8  ft_old_sts;  /* 错误信号旧状态 */
    U8  ft_trg_md;/* 错误信号判决逻辑 */
    U16 ft_wd; /* 错误信号宽度,MS */
    U16 ft_tm;       /* 错误时间，单位S*/
    U8   m_ch_trg_md;/* 主计数脉冲逻辑判断 */
    U16  m_ch_wd;/* 主计数信号宽度,MS */
    U16  m_ch_tm_cnt;/* 主计数间隔时间，1ms判断一次 */
    U8   r_ch_trg_md;/* 预留计数脉冲逻辑判断 */
    U16  r_ch_wd;/* 预留计数信号宽度,MS */
    U16  r_ch_tm_cnt;/* 预留计数间隔时间 */
    //2019年11月30日  刀版功能新增
    U8   b_ch_trg_md;/* 刀版计数脉冲逻辑判断 */
    U16  b_ch_wd;/* 刀版计数信号宽度,MS */
    U16  b_ch_tm_cnt;/* 刀版计数间隔时间 */
    //2020年01月12日  新增脉宽调制检测
    /* 主判断电平时间,就是占空比那一块主要判断依据时间 */
    U16  m_ch_pwm_h_tm;/* 主判断电平时间,高电平区间 */
    U16  m_ch_pwm_l_tm;/* 主判断电平时间,低电平区间 */
    U16  r_ch_pwm_h_tm;/* 预留判断电平时间,高电平区间 */
    U16  r_ch_pwm_l_tm;/* 预留判断电平时间,低电平区间 */
    /* 控制部分 */
    U8   upload_en;  /* 数据定时上传使能 */
    U8   storeenable;   /* 存储数据定时使能*/
    /* 2019年11月30日  刀版功能新增 */
    U8   bladeenable;   /* 刀版数据上传使能 */
    U8   cl_en; /* 2020年1月1日    定时清零增加 */
}CollectData;

/* 小时 *///外部信号采集状态
typedef struct _CollectSigState_
{
	  U8 bootsig;   /* 开机信号状态,对应COLLECTSTATE */
	  U8 stopsig;   /* 关机信号状态 */
	  U8 faultsig;	 /* 错误信号状态 */
	  U8 pnopaperstate;	 /* 正反纸信号状态 */
	  
	  U8  mainpulsesig;  /* 主脉冲信号状态 */
	  U32 mainpulsecount;/* 主脉冲信号计数 */
	
	  U8  revpulsesig;  /* 预留脉冲信号状态 */
	  U32 revpulsecount;/* 预留脉冲信号计数 */
	
	  /* 2019年11月30日  刀版功能新增 */
	  U8  bladepulsesig;  /* 刀版脉冲信号状态 */
	  U32 bladepulsecount;/* 刀版脉冲信号计数 */
	  RTC_TimeTypeDef bladepulsetime;
}CollectSigState;

/* 实时缓存数据 */
typedef struct _RealTimeStore_
{
    RTC_DateTime  MachineCurrent; /* 缓存当前时间 */
    U16  dataindex;          /* 上报信息序列号 */
    U8   bootstopsig;        /* 开机信号状态, */
    U8   faultsig;	        /* 错误信号状态 */
    U16 alltime;            /* 机器总时间 */
    U16 bootsigtime;         /* 开机时间，单位S */
    U16 stopsigtime;        /* 停机时间，单位S */ 
    U16 faultsigtime;       /* 错误时间，单位S */ 
    U32 pulsecount;          /* 主脉冲信号计数 */
    /* 小时 *//* 2019年11月30日  刀版功能新增 */
    U32 bladepulsecount;    /* 刀版脉冲信号计数 */ 
}RealTimeStore;



void  Collect_config(void);//信号初始化配置
void  Collect_init(void);//检测信号初始化
void  Collect_TimerHandle(void);//信号宽度和间隔检测
void  Collect_ExitHandle (SIGINTERFACE siginterface);//外部信号检测
void  Collect_Handle(void);//采集状态处理
void  Collect_StateTimeAggregation(void);//设备状态累计时间
void  Collect_UploadTimeCount(void);//数据定时上报
void  Collect_UploadHandle(void);
U8    Collect_Judge_ClearData(RTC_DateTime _timebegin);
//插纸器部分
void  Collect_Panpaper(void);

#endif

/************************ (C) COPYRIGHT Hopewellgroup ***********END OF FILE***/

