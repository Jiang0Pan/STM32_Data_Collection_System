/**
  ******************************************************************************
  * @file           : maincounter.c
  * @brief          : main counter
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

#ifndef __MAINCOUNTER_H
#define __MAINCOUNTER_H

#include "stm32f4xx_hal.h"
#include "pl_type.h"
#include "diechangecounter.h"

typedef struct _DeSts {
	U8  dsts;					   /*  设备状态                                   */
	U8  dpegd[2];				   /*  纸脉冲边沿顺序情况               	              */
	U16 dppcnt;  				   /*  脉冲边沿数量情况                       		  */
}DeSts;                            /*  设备采集属性                                 */ 

typedef struct _maincounter {
	U8    tooth_row_count_flag;    /*  牙排标志计数值                                */
	U8    ecnt;             	   /*  空转状态下，纸的脉冲计数值                          */
    U32   pcnt;             	   /*  纸张的计数值                          	      */
	U64   inttime;        		   /*  牙排中断的当前时间                    	      */
	U64   curtime;            	   /*  线程中的当前时间                      		  */ 	
	DeSts odests;	   			   /*  设备上一次采集属性                              */				
	DeSts cdests;   			   /*  设备当前采集属性                               */                         
}MainCnt;/* 设备计数： */

typedef enum _DEVICE_STATUS_ {
    STOPSTATUS  = 0,    /*  停机状态  */
    EMPTYSTATUS = 1,    /*  空转状态  */
    RUNSTATUS   = 2,    /*  运行状态  */
}DEVICE_STATUS;/* 设备状态：停机状态、运行状态、空转状态 */

#define  SECOND            2
#define  FIVE              5
#define  EMPTY_DEFINE      2
#define  TIMEOUT_          3000000UL

#define  READ_RESERVEDPULSE_EDGE    	HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_7)

extern void  collect_init_paper_cnt(void);
extern void  collect_clear_paper_cnt(void);
extern void  collect_set_tooth_int_cnt(void);
extern void  collect_set_paper_int_cnt(void);
extern U32   collect_get_paper_cnt(void);  

#endif

/************************ (C) COPYRIGHT Hopewellgroup ***********END OF FILE***/

