/**
  ******************************************************************************
  * @file           : led.c
  * @brief          : led control
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
#include "led.h"

/*******************************************************************************
函数名称：led_init
功    能： led init
说    明：
参    数:
返    回:
注    意：
*******************************************************************************/
void led_init(void)
{
	/* PC6置1，默认初始化后第一列第一排灯亮 */
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_SET);
	/* PC8置1，默认初始化后第一列第三排灯亮 */
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_SET);
}

/************************ (C) COPYRIGHT Hopewellgroup ***********END OF FILE***/

