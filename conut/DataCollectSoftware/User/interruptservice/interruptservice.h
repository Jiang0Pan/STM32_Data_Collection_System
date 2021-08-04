/**
  ******************************************************************************
  * @file           : interruptservice.h
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

#ifndef __INTERRUPTSERVICE_H
#define __INTERRUPTSERVICE_H

#include "stm32f4xx_hal.h"

void TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

#endif

/************************ (C) COPYRIGHT Hopewellgroup ***********END OF FILE***/

