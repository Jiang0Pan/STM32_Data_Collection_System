/**
  ******************************************************************************
  * @file           : interruptservice.c
  * @brief          : interrupt service 
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

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart6;

/*******************************************************************************
�������ƣ�TIM_PeriodElapsedCallback
��    �ܣ� ��ʱ���ص�����
˵    ����
��    ��: htim:��ʱ��
��    ��: 
ע    �⣺
*******************************************************************************/
void TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM2) {/* �ɼ����ֶ�ʱ��,�����ж��źſ�Ⱥ��źż�� */
//		Collect_TimerHandle();
	}
	else if (htim->Instance == TIM3) { /* 1S��ʱ���� */
		Collect_StateTimeAggregation();
		Collect_UploadTimeCount();
		Config_UploadTimeCount();
		/* �ж�������� */
		if(Collect_Judge_ClearData(g_sys_sts.col_info.pwr_on_tm) == TRUE) {
			g_sys_sts.col_info.cl_en = TRUE;/* �ϴ��������		*/
		}
	}
	/* 1MS��ʱ���� */
	else if (htim->Instance == TIM4) { 
		Outcontrol_Inpaper_TimeCount();
		Outcontrol_Cutpaper_TimeCount();
		WIFI_uart_receiveTimeout();
	}
	else {
		
	}
}

/*******************************************************************************
�������ƣ�HAL_GPIO_EXTI_Callback
��    �ܣ� �ⲿ�ж�
˵    ����
��    ��: GPIO_Pin:GPIO����
��    ��: 
ע    �⣺
*******************************************************************************/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	
	if (GPIO_Pin == MAINPULSESIG_GPIO) { /* ���Ź���ж� */
		Collect_ExitHandle(MAINPULSESIG);
	}
	else if(GPIO_Pin == RESERVEDPULSE_GPIO) { /* ֽ����ж� */
	 	Collect_ExitHandle(RESERVEDPULSESIG);          
	}
	else if (GPIO_Pin==REVPULSESIG_GPIO) { /* �ʼ��ж� */
		Collect_ExitHandle(REVPULSESIG);
	}
	/* �������ܣ�20191130���� */
	else if (GPIO_Pin==BLADEPULSESIG_GPIO) { /* ��ģ�ж� */
		Collect_ExitHandle(BLADEPULSESIG);
	}
	else {

	}		 
}

/*******************************************************************************
�������ƣ�HAL_UART_RxCpltCallback
��    �ܣ� �������ݶ�ȡ
˵    ����
��    ��: huart:����
��    ��: 
ע    �⣺
*******************************************************************************/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (USART1 == huart->Instance) { /* ���ô��� */
		Config_PcDataAnaly(g_sys_sts.uart_frm.rxbuf[0]);    
		HAL_UART_Receive_DMA(&huart1,g_sys_sts.uart_frm.rxbuf,1);
	}
	else if (USART6 == huart->Instance) { /* WIFI���� */
		/* ����� Wi-Fiģ��ͨ�ſ���,��������������ȡ�ߣ�
		���������ط�ͨ��һ�������⡣
		����configenable������ʹ�á�*/ 
		if (g_sys_sts.wifi_mgr.configenable == 0) {
			WIFI_uart_receiveData(g_sys_sts.wifi_cfg_mgr.RxUartData);
			HAL_UART_Receive_DMA(&huart6,&g_sys_sts.wifi_cfg_mgr.RxUartData,1);	  
		}
	} 
}

/************************ (C) COPYRIGHT Hopewellgroup ***********END OF FILE***/

