/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2019 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Bootsigin_Pin GPIO_PIN_0
#define Bootsigin_GPIO_Port GPIOA
#define Stopsigin_Pin GPIO_PIN_1
#define Stopsigin_GPIO_Port GPIOA
#define Faultsigin_Pin GPIO_PIN_2
#define Faultsigin_GPIO_Port GPIOA
#define Pulsesigin_Pin GPIO_PIN_3
#define Pulsesigin_GPIO_Port GPIOA
#define Pulsesigin_EXTI_IRQn EXTI3_IRQn
#define Inserpaper_1_Pin GPIO_PIN_4
#define Inserpaper_1_GPIO_Port GPIOA
#define Inserpaper_2_Pin GPIO_PIN_5
#define Inserpaper_2_GPIO_Port GPIOA
#define StopControl_Pin GPIO_PIN_6
#define StopControl_GPIO_Port GPIOA
#define Reservedpulse_Pin GPIO_PIN_7
#define Reservedpulse_GPIO_Port GPIOE
#define revpulsesig_Pin GPIO_PIN_9
#define revpulsesig_GPIO_Port GPIOE
#define revpulsesig_EXTI_IRQn EXTI9_5_IRQn
#define RevSigOut_1_Pin GPIO_PIN_10
#define RevSigOut_1_GPIO_Port GPIOE
#define EEPROM_WP_Pin GPIO_PIN_13
#define EEPROM_WP_GPIO_Port GPIOE
#define EEPROM_SCL_Pin GPIO_PIN_14
#define EEPROM_SCL_GPIO_Port GPIOE
#define EEPROM_SDA_Pin GPIO_PIN_15
#define EEPROM_SDA_GPIO_Port GPIOE
#define MCU_USR2_nReset_Pin GPIO_PIN_13
#define MCU_USR2_nReset_GPIO_Port GPIOD
#define LED1_Pin GPIO_PIN_6
#define LED1_GPIO_Port GPIOC
#define LED2_Pin GPIO_PIN_7
#define LED2_GPIO_Port GPIOC
#define LED3_Pin GPIO_PIN_8
#define LED3_GPIO_Port GPIOC
#define LED4_Pin GPIO_PIN_9
#define LED4_GPIO_Port GPIOC
#define MCU_USR1_nReset_Pin GPIO_PIN_7
#define MCU_USR1_nReset_GPIO_Port GPIOD
#define USR1_TX_Pin GPIO_PIN_9
#define USR1_TX_GPIO_Port GPIOG
#define USR1_RTS_Pin GPIO_PIN_13
#define USR1_RTS_GPIO_Port GPIOG
#define USR1_RX_Pin GPIO_PIN_14
#define USR1_RX_GPIO_Port GPIOG
#define USR1_CTS_Pin GPIO_PIN_15
#define USR1_CTS_GPIO_Port GPIOG
#define MCU_USR1_nReload_Pin GPIO_PIN_9
#define MCU_USR1_nReload_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
