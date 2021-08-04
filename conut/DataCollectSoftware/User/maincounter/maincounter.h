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
	U8  dsts;					   /*  �豸״̬                                   */
	U8  dpegd[2];				   /*  ֽ�������˳�����               	              */
	U16 dppcnt;  				   /*  ��������������                       		  */
}DeSts;                            /*  �豸�ɼ�����                                 */ 

typedef struct _maincounter {
	U8    tooth_row_count_flag;    /*  ���ű�־����ֵ                                */
	U8    ecnt;             	   /*  ��ת״̬�£�ֽ���������ֵ                          */
    U32   pcnt;             	   /*  ֽ�ŵļ���ֵ                          	      */
	U64   inttime;        		   /*  �����жϵĵ�ǰʱ��                    	      */
	U64   curtime;            	   /*  �߳��еĵ�ǰʱ��                      		  */ 	
	DeSts odests;	   			   /*  �豸��һ�βɼ�����                              */				
	DeSts cdests;   			   /*  �豸��ǰ�ɼ�����                               */                         
}MainCnt;/* �豸������ */

typedef enum _DEVICE_STATUS_ {
    STOPSTATUS  = 0,    /*  ͣ��״̬  */
    EMPTYSTATUS = 1,    /*  ��ת״̬  */
    RUNSTATUS   = 2,    /*  ����״̬  */
}DEVICE_STATUS;/* �豸״̬��ͣ��״̬������״̬����ת״̬ */

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

