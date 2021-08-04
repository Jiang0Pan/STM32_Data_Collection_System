/**
  ******************************************************************************
  * @file           : system_config.c
  * @brief          : system config
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

/* include fils */
#include "stm32f4xx_hal.h"
#include "system_config.h"
#include "led.h"
#include "usart.h"

/*******************************************************************************
函数名称：system_init
功    能： 系统初始化
说    明：
参    数:
返    回:
注    意：
*******************************************************************************/
void system_init(void)
{
	usart1_init();
	led_init();	
}

/************************ (C) COPYRIGHT Hopewellgroup ***********END OF FILE***/

