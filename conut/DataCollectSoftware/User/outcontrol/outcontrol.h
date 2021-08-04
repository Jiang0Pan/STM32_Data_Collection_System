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
/* ��ֽʱ������ */
#define INPAPERTIME    5
#define CUTPAPERTIME   200

/* �˿ں궨�� */
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

/* ��ֽ��-1 */
#define IN_PAPER_1_ON   HAL_GPIO_WritePin(IN_PAPER1_GPIO_Port, IN_PAPER1_Pin, GPIO_PIN_SET)
#define IN_PAPER_1_OFF  HAL_GPIO_WritePin(IN_PAPER1_GPIO_Port, IN_PAPER1_Pin, GPIO_PIN_RESET)

/* ��ֽ��-2 */
#define IN_PAPER_2_ON   HAL_GPIO_WritePin(IN_PAPER2_GPIO_Port, IN_PAPER2_Pin, GPIO_PIN_SET)
#define IN_PAPER_2_OFF  HAL_GPIO_WritePin(IN_PAPER2_GPIO_Port, IN_PAPER2_Pin, GPIO_PIN_RESET)

/* ͣ���ź� */
#define STOP_CLOSE         HAL_GPIO_WritePin(STOP_GPIO_Port, STOP_Pin, GPIO_PIN_SET)
#define STOP_OPEN          HAL_GPIO_WritePin(STOP_GPIO_Port, STOP_Pin, GPIO_PIN_RESET)

/* LED���ź� */
#define LED1_ON           HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET)
#define LED1_OFF          HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET)

#define LED2_ON           HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET)
#define LED2_OFF          HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET)

#define LED3_ON           HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET)
#define LED3_OFF          HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET)

#define LED4_ON           HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_SET)
#define LED4_OFF          HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_RESET)

/* ���ù���ṹ�� */
typedef struct
{
	uint8_t inpaperenable;  /* ��ֽʹ�� */
	uint8_t inpaperstate;   /* ��ֽ״̬,1Ϊ��ֽ,0Ϊδ��ֽ */
	uint8_t cutpaperstate;  /* ��ֽ״̬,1Ϊ��ֽ,0Ϊ����ֽ */
	uint16_t inpaperTime;    /* ��ֽʱ�� */
	uint16_t cutpaperTime;    /* ��ֽʱ�� */	
}InPaperMgr; /* ���ù���	 */

/* ���ù���ṹ�� */
typedef struct
{
	uint8_t  delayisend;   /* ִ����� */
	uint8_t  state;   /* 1Ϊ������0Ϊͣ�� */
	uint8_t  mode;    /* 0:��ʱִ��,1Ϊ��ʱִ�� */
	uint16_t delayTime; /* �ӳٲ���ʱ�䣬��λMS */
}StopMgr; /* ���ù���	 */

void Outcontrol_Init(void);
void Outcontrol_config(void);
void Outcontrol_Inpaper_TimeCount(void); /* ��ֽ */
void Outcontrol_Cutpaper_TimeCount(void); /* ��ֽ */
void Outcontrol_Handle(void);
void Outcontrol_Stop(uint8_t state);
#endif

/************************ (C) COPYRIGHT Hopewellgroup ***********END OF FILE***/

