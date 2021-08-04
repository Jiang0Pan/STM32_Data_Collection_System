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
函数名称：TIM_PeriodElapsedCallback
功    能： 定时器回调服务
说    明：
参    数: htim:定时器
返    回: 
注    意：
*******************************************************************************/
void TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM2) {/* 采集部分定时器,用于判断信号宽度和信号间隔 */
//		Collect_TimerHandle();
	}
	else if (htim->Instance == TIM3) { /* 1S定时触发 */
		Collect_StateTimeAggregation();
		Collect_UploadTimeCount();
		Config_UploadTimeCount();
		/* 判断清除数据 */
		if(Collect_Judge_ClearData(g_sys_sts.col_info.pwr_on_tm) == TRUE) {
			g_sys_sts.col_info.cl_en = TRUE;/* 上传清除数据		*/
		}
	}
	/* 1MS定时触发 */
	else if (htim->Instance == TIM4) { 
		Outcontrol_Inpaper_TimeCount();
		Outcontrol_Cutpaper_TimeCount();
		WIFI_uart_receiveTimeout();
	}
	else {
		
	}
}

/*******************************************************************************
函数名称：HAL_GPIO_EXTI_Callback
功    能： 外部中断
说    明：
参    数: GPIO_Pin:GPIO引脚
返    回: 
注    意：
*******************************************************************************/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	
	if (GPIO_Pin == MAINPULSESIG_GPIO) { /* 牙排光电中断 */
		Collect_ExitHandle(MAINPULSESIG);
	}
	else if(GPIO_Pin == RESERVEDPULSE_GPIO) { /* 纸光电中断 */
	 	Collect_ExitHandle(RESERVEDPULSESIG);          
	}
	else if (GPIO_Pin==REVPULSESIG_GPIO) { /* 质检中断 */
		Collect_ExitHandle(REVPULSESIG);
	}
	/* 新增功能，20191130增加 */
	else if (GPIO_Pin==BLADEPULSESIG_GPIO) { /* 换模中断 */
		Collect_ExitHandle(BLADEPULSESIG);
	}
	else {

	}		 
}

/*******************************************************************************
函数名称：HAL_UART_RxCpltCallback
功    能： 串口数据读取
说    明：
参    数: huart:串口
返    回: 
注    意：
*******************************************************************************/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (USART1 == huart->Instance) { /* 配置串口 */
		Config_PcDataAnaly(g_sys_sts.uart_frm.rxbuf[0]);    
		HAL_UART_Receive_DMA(&huart1,g_sys_sts.uart_frm.rxbuf,1);
	}
	else if (USART6 == huart->Instance) { /* WIFI串口 */
		/* 如果对 Wi-Fi模块通信控制,留意这里会把数据取走，
		导致其他地方通信一致有问题。
		留意configenable变量的使用。*/ 
		if (g_sys_sts.wifi_mgr.configenable == 0) {
			WIFI_uart_receiveData(g_sys_sts.wifi_cfg_mgr.RxUartData);
			HAL_UART_Receive_DMA(&huart6,&g_sys_sts.wifi_cfg_mgr.RxUartData,1);	  
		}
	} 
}

/************************ (C) COPYRIGHT Hopewellgroup ***********END OF FILE***/

