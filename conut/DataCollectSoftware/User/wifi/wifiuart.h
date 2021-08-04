/**
  ******************************************************************************
  * @file           : wifiuart.h
  * @brief          : wifi config
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

#ifndef __WIFI_UART_H
#define __WIFI_UART_H

#include "stm32f4xx_hal.h"

#define MCU_USR1_nReset_Pin GPIO_PIN_7
#define MCU_USR1_nReset_GPIO_Port GPIOD

#define WIFI_DEVNAME     "USR_216"
#define WIFI_MODE        "AP"
#define WIFI_NAME        "USR_216"
#define WIFI_PASSWORD    "12345678"
#define SERVER_IP        "192.168.123.123"
#define SERVER_PORT      "8899"
#define LOCAL_IP         "192.168.123.124"
#define LOCAL_MASK       "255.255.255.0"
#define LOCAL_GATEWAY    "192.168.123.1"



/* 复位延时 */
#define WIFIRESETTIME  1000
#define WIFIWAITCONNECT  120 /* WIFI等待连接20S,500MS一次 */
/* WIFI通讯口 */

/* 配置管理结构体 */
typedef struct
{
  uint8_t configenable;  /* 是否为参数配置部分 */
	
}WifiMgr; /* 配置管理 */
/* 电压选择定义 */
#define WIFI_RESET_ON   HAL_GPIO_WritePin(MCU_USR1_nReset_GPIO_Port, MCU_USR1_nReset_Pin, GPIO_PIN_RESET);
#define WIFI_RESET_OFF  HAL_GPIO_WritePin(MCU_USR1_nReset_GPIO_Port, MCU_USR1_nReset_Pin, GPIO_PIN_SET);

void delay_ms(uint32_t time);
void WIFI_uart_sendData(char* buf,uint16_t length);
void WIFI_Uart_Handle(void);/* 处理串口接收数据 */
void WIFI_uart_init(void);
void WIFI_net_init(void);/* wifi网络初始化 */
void WIFI_uart_receiveData(uint8_t data);/* WIFI数据接收 */
void WIFI_uart_receiveTimeout(void);
#endif


/************************ (C) COPYRIGHT Hopewellgroup ***********END OF FILE***/

