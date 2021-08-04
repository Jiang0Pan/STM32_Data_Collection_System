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
�������ƣ�system_init
��    �ܣ� ϵͳ��ʼ��
˵    ����
��    ��:
��    ��:
ע    �⣺
*******************************************************************************/
void system_init(void)
{
	usart1_init();
	led_init();	
}

/************************ (C) COPYRIGHT Hopewellgroup ***********END OF FILE***/

