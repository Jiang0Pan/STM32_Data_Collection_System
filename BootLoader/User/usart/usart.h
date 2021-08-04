/**
  ******************************************************************************
  * @file           : usart.h
  * @brief          : usart1 & uart4 driver
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

#ifndef __USART_H
#define __USART_H

/* function declaration */ 
void usart1_init(void);
void USART1_IRQHandler(void);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);\
static void Config_PcDataAnaly(U8 data);
static void Config_uart_sendData(U8* buf,U16 length);
static void Config_uart_sendStructData(UartUpData *RxUartData);
void mp_send_data_nolen(U8* buf);

#endif

/************************ (C) COPYRIGHT Hopewellgroup ***********END OF FILE***/

