/**
  ******************************************************************************
  * @file           : qulitycounter.h
  * @brief          : qulity counter
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

#ifndef __QULITYCOUNTER_H
#define __QULITYCOUNTER_H

#include "stm32f4xx_hal.h"
#include "pl_type.h"

typedef struct _qulitycounter {
	U32   qtcnt;                   /*  质检计数                              	  */                    
}QtCnt;					            /*  设备质检计数： 		                      */

extern void collect_clear_qt_cnt(void);
extern void collect_set_qt_intcnt(void);
extern U32  collect_get_qt_cnt(void);

#endif

/************************ (C) COPYRIGHT Hopewellgroup ***********END OF FILE***/

