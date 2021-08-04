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
�������ƣ�led_init
��    �ܣ� led init
˵    ����
��    ��:
��    ��:
ע    �⣺
*******************************************************************************/
void led_init(void)
{
	/* PC6��1��Ĭ�ϳ�ʼ�����һ�е�һ�ŵ��� */
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_SET);
	/* PC8��1��Ĭ�ϳ�ʼ�����һ�е����ŵ��� */
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_SET);
}

/************************ (C) COPYRIGHT Hopewellgroup ***********END OF FILE***/

