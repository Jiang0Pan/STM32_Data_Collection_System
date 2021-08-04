/**
  ******************************************************************************
  * @file           : diechangecount.c
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

#include "diechangecounter.h"
#include "stdio.h"

static EpCnt ep_cnt;

/*******************************************************************************
函数名称：collect_clear_ep_cnt
功    能：清空当前的换膜计数值
说    明：
参    数:
返    回: 
注    意：
*******************************************************************************/
void collect_clear_ep_cnt(void)
{
	ep_cnt.epcnt = 0;
	memset(&ep_cnt.eptime,0,sizeof(RTC_TimeTypeDef));
}

/*******************************************************************************
函数名称：collect_set_ep_intcnt
功    能：统计换模的次数、换模的持续时间
说    明：
参    数:
返    回: 
注    意：
*******************************************************************************/
void collect_set_ep_intcnt(void)
{
	ep_cnt.intflag = SET_FLAG;
}

/*******************************************************************************
函数名称：collect_get_ep_cnt
功    能：返回换模的次数与持续时间
说    明：
参    数:
返    回: 
注    意：
*******************************************************************************/
EpCnt  collect_get_ep_cnt(void)
{
	static U8 intcnt = 0;
	static RTC_TimeTypeDef epstart = 0;	      /*  换产品开始时间					  */
	static RTC_TimeTypeDef epend = 0;  		  /*  换产品开始时间					  */

	if	(ep_cnt.intflag == SET_FLAG) {
		ep_cnt.intflag = RESET_FLAG;
		intcnt++;
		if (intcnt % 2) {
			HAL_RTC_GetTime(&hrtc, 
				&epstart, 
				RTC_FORMAT_BIN);
		}
		else {
			intcnt = 0;
			ep_cnt.epcnt++;
			HAL_RTC_GetTime(&hrtc, 
				&epend,
				RTC_FORMAT_BIN);
			if (epend.Seconds >=
					epstart.Seconds) {
				ep_cnt.eptime.Seconds = epend.Seconds - epstart.Seconds;
			}
			else {
				ep_cnt.eptime.Seconds = epend.Seconds + 60 - epstart.Seconds;
				epend.Minutes--; 		
			}
			if (epend.Minutes >=
					epstart.Minutes) {
				ep_cnt.eptime.Minutes = 
						epend.Minutes - 
						epstart.Minutes;
			}
			else {
				ep_cnt.eptime.Minutes = 
						epend.Minutes + 60 - 
						epstart.Minutes;
						epend.Hours--;
			}
			ep_cnt.eptime.Hours = 
					epend.Hours - 
					epstart.Hours;
				
			}
	}
	return ep_cnt;
}

/************************ (C) COPYRIGHT Hopewellgroup ***********END OF FILE***/

