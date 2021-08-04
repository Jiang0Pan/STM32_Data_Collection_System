/**
  ******************************************************************************
  * @file           : diechangecount.h
  * @brief          : die change counter
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

#ifndef __DIECHANGECOUNTER_H
#define __DIECHANGECOUNTER_H

#include "stm32f4xx_hal.h"
#include "pl_type.h"

typedef enum _intflag{
	RESET_FLAG = 0,
	SET_FLAG = 1,
}Intflag;

typedef struct _diechangecounter {
	U8    intflag;			       /*  �жϱ�־λ                             	  */
	U32   epcnt;				   /*  ����Ʒ����                                  */
	RTC_TimeTypeDef eptime;		   /*  ��Ĥ����ʱ��           				      */                 
}EpCnt;							   /*  �豸����                                   */

extern RTC_HandleTypeDef hrtc;

extern void   collect_clear_ep_cnt(void);
extern void   collect_set_ep_intcnt(void);
extern EpCnt  collect_get_ep_cnt(void);

#endif

/************************ (C) COPYRIGHT Hopewellgroup ***********END OF FILE***/

