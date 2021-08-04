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

#ifndef __OUTCONTROL_H
#define __OUTCONTROL_H

#include "stm32f4xx_hal.h"
/* 插纸时间设置 */
#define INPAPERTIME    5
#define CUTPAPERTIME   200

/* 端口宏定义 */
#define IN_PAPER1_Pin  GPIO_PIN_4
#define IN_PAPER1_GPIO_Port GPIOA

#define IN_PAPER2_Pin  GPIO_PIN_5
#define IN_PAPER2_GPIO_Port GPIOA

#define STOP_Pin       GPIO_PIN_6
#define STOP_GPIO_Port      GPIOA

#define LED1_Pin       GPIO_PIN_6
#define LED1_GPIO_Port      GPIOC
#define LED2_Pin       GPIO_PIN_7
#define LED2_GPIO_Port      GPIOC
#define LED3_Pin       GPIO_PIN_8
#define LED3_GPIO_Port      GPIOC
#define LED4_Pin       GPIO_PIN_9
#define LED4_GPIO_Port      GPIOC

/* 插纸机-1 */
#define IN_PAPER_1_ON   HAL_GPIO_WritePin(IN_PAPER1_GPIO_Port, IN_PAPER1_Pin, GPIO_PIN_SET)
#define IN_PAPER_1_OFF  HAL_GPIO_WritePin(IN_PAPER1_GPIO_Port, IN_PAPER1_Pin, GPIO_PIN_RESET)

/* 插纸机-2 */
#define IN_PAPER_2_ON   HAL_GPIO_WritePin(IN_PAPER2_GPIO_Port, IN_PAPER2_Pin, GPIO_PIN_SET)
#define IN_PAPER_2_OFF  HAL_GPIO_WritePin(IN_PAPER2_GPIO_Port, IN_PAPER2_Pin, GPIO_PIN_RESET)

/* 停机信号 */
#define STOP_CLOSE         HAL_GPIO_WritePin(STOP_GPIO_Port, STOP_Pin, GPIO_PIN_SET)
#define STOP_OPEN          HAL_GPIO_WritePin(STOP_GPIO_Port, STOP_Pin, GPIO_PIN_RESET)

/* LED灯信号 */
#define LED1_ON           HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET)
#define LED1_OFF          HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET)

#define LED2_ON           HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET)
#define LED2_OFF          HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET)

#define LED3_ON           HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET)
#define LED3_OFF          HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET)

#define LED4_ON           HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_SET)
#define LED4_OFF          HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_RESET)

/* 配置管理结构体 */
typedef struct
{
	uint8_t inpaperenable;  /* 插纸使能 */
	uint8_t inpaperstate;   /* 插纸状态,1为插纸,0为未插纸 */
	uint8_t cutpaperstate;  /* 剪纸状态,1为剪纸,0为不剪纸 */
	uint16_t inpaperTime;    /* 插纸时间 */
	uint16_t cutpaperTime;    /* 剪纸时间 */	
}InPaperMgr; /* 配置管理	 */

/* 配置管理结构体 */
typedef struct
{
	uint8_t  delayisend;   /* 执行完成 */
	uint8_t  state;   /* 1为开机，0为停机 */
	uint8_t  mode;    /* 0:及时执行,1为延时执行 */
	uint16_t delayTime; /* 延迟操作时间，单位MS */
}StopMgr; /* 配置管理	 */

void Outcontrol_Init(void);
void Outcontrol_config(void);
void Outcontrol_Inpaper_TimeCount(void); /* 插纸 */
void Outcontrol_Cutpaper_TimeCount(void); /* 剪纸 */
void Outcontrol_Handle(void);
void Outcontrol_Stop(uint8_t state);
#endif

/************************ (C) COPYRIGHT Hopewellgroup ***********END OF FILE***/

