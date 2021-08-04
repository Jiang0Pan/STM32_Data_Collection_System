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
�������ƣ�Outcontrol_config
��    �ܣ���������ϵ�����
˵    ����
��    ��:
��    ��: 
ע    �⣺
*******************************************************************************/
void  Outcontrol_config(void)
{
	g_sys_cfg.out_cfg.InPaperTime = INPAPERTIME;
}

/*******************************************************************************
�������ƣ�Outcontrol_config
��    �ܣ�������Ƴ�ʼ��
˵    ����
��    ��:
��    ��: 
ע    �⣺
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
	/*��Դָʾ���ϵ���ƣ����Ƕϵ磬�����ٲ���*/
	LED2_OFF;
	LED3_OFF;
	LED4_OFF;
}

/*******************************************************************************
�������ƣ�Outcontrol_Inpaper_TimeCount
��    �ܣ������ʱ���,1MS���һ��
˵    ������ֽ
��    ��:
��    ��: 
ע    �⣺
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
�������ƣ�Outcontrol_Stop
��    �ܣ�ͣ���������
˵    ����
��    ��:state:ͣ���źŲ�����1Ϊͣ����0Ϊ����
��    ��: 
ע    �⣺
*******************************************************************************/
void Outcontrol_Stop(uint8_t state)
{
	if (state== FALSE) { /* ���� */
		if (g_sys_cfg.out_cfg.StopSigType == CLOSE) {
			STOP_OPEN;
		}
		else {
			STOP_CLOSE;
		}
	}
    else {                                                      /* ͣ�� */
		if (g_sys_cfg.out_cfg.StopSigType == CLOSE) {
			STOP_CLOSE;
		}
		else {
			STOP_OPEN;
		}
    }
}

/*******************************************************************************
�������ƣ�Outcontrol_Cutpaper_TimeCount
��    �ܣ������ʱ���,1MS���һ��
˵    ������ֽ
��    ��:
��    ��: 
ע    �⣺
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
�������ƣ�Outcontrol_Handle
��    �ܣ�������ƾ��
˵    ����
��    ��:
��    ��: 
ע    �⣺
*******************************************************************************/
void Outcontrol_Handle(void)
{
	if (g_sys_sts.inpaper_mgr.inpaperenable == TRUE) {
		IN_PAPER_1_ON;
		g_sys_sts.inpaper_mgr.inpaperenable = FALSE;
		g_sys_sts.inpaper_mgr.inpaperstate = TRUE;
		g_sys_sts.inpaper_mgr.inpaperTime = g_sys_cfg.out_cfg.InPaperTime;
	}
//	/* ����ֽ����ͣ���ź� */
//	if ( g_sys_sts.col_sts.pnopaperstate == TRUE) { /* ��ֽ */
//		if (g_sys_cfg.out_cfg.StopSigType == CLOSE) {
//			STOP_OPEN;
//		}
//		else {
//			STOP_CLOSE;
//		}
//	}
//	else {                                                     /* ��ֽ */
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

