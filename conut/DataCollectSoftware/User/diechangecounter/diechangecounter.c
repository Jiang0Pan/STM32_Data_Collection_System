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
�������ƣ�collect_clear_ep_cnt
��    �ܣ���յ�ǰ�Ļ�Ĥ����ֵ
˵    ����
��    ��:
��    ��: 
ע    �⣺
*******************************************************************************/
void collect_clear_ep_cnt(void)
{
	ep_cnt.epcnt = 0;
	memset(&ep_cnt.eptime,0,sizeof(RTC_TimeTypeDef));
}

/*******************************************************************************
�������ƣ�collect_set_ep_intcnt
��    �ܣ�ͳ�ƻ�ģ�Ĵ�������ģ�ĳ���ʱ��
˵    ����
��    ��:
��    ��: 
ע    �⣺
*******************************************************************************/
void collect_set_ep_intcnt(void)
{
	ep_cnt.intflag = SET_FLAG;
}

/*******************************************************************************
�������ƣ�collect_get_ep_cnt
��    �ܣ����ػ�ģ�Ĵ��������ʱ��
˵    ����
��    ��:
��    ��: 
ע    �⣺
*******************************************************************************/
EpCnt  collect_get_ep_cnt(void)
{
	static U8 intcnt = 0;
	static RTC_TimeTypeDef epstart = 0;	      /*  ����Ʒ��ʼʱ��					  */
	static RTC_TimeTypeDef epend = 0;  		  /*  ����Ʒ��ʼʱ��					  */

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

