/**
  ******************************************************************************
  * @file           : outcontrol.h
  * @brief          : out data control
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


#include "system_config.h"

/*******************************************************************************
函数名称：Outcontrol_config
功    能：输出控制上电配置
说    明：
参    数:
返    回: 
注    意：
*******************************************************************************/
void  Outcontrol_config(void)
{
	g_sys_cfg.out_cfg.InPaperTime = INPAPERTIME;
}

/*******************************************************************************
函数名称：Outcontrol_config
功    能：输出控制初始化
说    明：
参    数:
返    回: 
注    意：
*******************************************************************************/
void Outcontrol_Init(void)
{
	IN_PAPER_1_OFF;
	IN_PAPER_2_OFF;
	if (g_sys_cfg.out_cfg.StopSigType == CLOSE) {
		STOP_OPEN;
	}
	else {
		STOP_CLOSE;	
	}
	/* LED1_OFF;*/ 
	/*电源指示灯上电控制，除非断电，否则不再操作*/
	LED2_OFF;
	LED3_OFF;
	LED4_OFF;
}

/*******************************************************************************
函数名称：Outcontrol_Inpaper_TimeCount
功    能：输出定时检测,1MS检测一次
说    明：插纸
参    数:
返    回: 
注    意：
*******************************************************************************/
void Outcontrol_Inpaper_TimeCount(void)
{
	if (g_sys_sts.inpaper_mgr.inpaperstate == TRUE) {
		if (g_sys_sts.inpaper_mgr.inpaperTime > 0) {
			g_sys_sts.inpaper_mgr.inpaperTime--;
		}
		if (g_sys_sts.inpaper_mgr.inpaperTime <= 0) {
			g_sys_sts.inpaper_mgr.inpaperstate = FALSE;
			IN_PAPER_1_OFF;
			IN_PAPER_2_OFF;
			g_sys_sts.inpaper_mgr.cutpaperstate = TRUE;
			g_sys_sts.inpaper_mgr.cutpaperTime = 
					g_sys_cfg.out_cfg.InPaperAfterCut;
		}
	}
}

/*******************************************************************************
函数名称：Outcontrol_Stop
功    能：停机输出控制
说    明：
参    数:state:停机信号操作，1为停机，0为开机
返    回: 
注    意：
*******************************************************************************/
void Outcontrol_Stop(uint8_t state)
{
	if (state== FALSE) { /* 开机 */
		if (g_sys_cfg.out_cfg.StopSigType == CLOSE) {
			STOP_OPEN;
		}
		else {
			STOP_CLOSE;
		}
	}
    else {                                                      /* 停机 */
		if (g_sys_cfg.out_cfg.StopSigType == CLOSE) {
			STOP_CLOSE;
		}
		else {
			STOP_OPEN;
		}
    }
}

/*******************************************************************************
函数名称：Outcontrol_Cutpaper_TimeCount
功    能：输出定时检测,1MS检测一次
说    明：剪纸
参    数:
返    回: 
注    意：
*******************************************************************************/
void Outcontrol_Cutpaper_TimeCount(void)
{
	if (g_sys_sts.inpaper_mgr.cutpaperstate == TRUE) {
		if (g_sys_sts.inpaper_mgr.cutpaperTime > 0) {
			g_sys_sts.inpaper_mgr.cutpaperTime--;
		}
		if (g_sys_sts.inpaper_mgr.cutpaperTime <= 0) {
			g_sys_sts.inpaper_mgr.cutpaperstate = FALSE;
			IN_PAPER_2_ON;
			delay_ms(CUTPAPERTIME);
			IN_PAPER_2_OFF;
		}
	}
}

/*******************************************************************************
函数名称：Outcontrol_Handle
功    能：输出控制句柄
说    明：
参    数:
返    回: 
注    意：
*******************************************************************************/
void Outcontrol_Handle(void)
{
	if (g_sys_sts.inpaper_mgr.inpaperenable == TRUE) {
		IN_PAPER_1_ON;
		g_sys_sts.inpaper_mgr.inpaperenable = FALSE;
		g_sys_sts.inpaper_mgr.inpaperstate = TRUE;
		g_sys_sts.inpaper_mgr.inpaperTime = g_sys_cfg.out_cfg.InPaperTime;
	}
//	/* 正反纸关联停机信号 */
//	if ( g_sys_sts.col_sts.pnopaperstate == TRUE) { /* 正纸 */
//		if (g_sys_cfg.out_cfg.StopSigType == CLOSE) {
//			STOP_OPEN;
//		}
//		else {
//			STOP_CLOSE;
//		}
//	}
//	else {                                                     /* 反纸 */
//		if (g_sys_cfg.out_cfg.StopSigType == CLOSE) {
//			STOP_CLOSE;
//			delay_ms(1000);
//			STOP_OPEN;
//		}
//		else {
//			STOP_OPEN;
//			delay_ms(1000);
//			STOP_CLOSE; 				 
//		}
//		g_sys_sts.col_sts.pnopaperstate = TRUE;
//	}
}

/************************ (C) COPYRIGHT Hopewellgroup ***********END OF FILE***/

