/**
  ******************************************************************************
  * @file           : modulebus.h
  * @brief          : moduledus mangement
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

#ifndef __AD5761_H
#define __AD5761_H

#include <stm32f4xx.h>
#include <stdint.h>

#define ADCONFIG      0x00F0

#define ALLRESET      0x0F   /* 配置寄存器 */
#define CONFIGREG     0x04   /* 配置寄存器 */
#define DATACONFIG    0x03   /* 数据寄存器      */

/* SIG通道1 */
#define AD5761_1_ALERT  GPIOG,GPIO_Pin_2 
#define AD5761_1_CLEAR  GPIOG,GPIO_Pin_3
#define AD5761_1_RESET  GPIOG,GPIO_Pin_4
#define AD5761_1_LDAC   GPIOG,GPIO_Pin_5

#define AD5761_1_SCLK   GPIOB,GPIO_Pin_13
#define AD5761_1_SYNC   GPIOB,GPIO_Pin_12
#define AD5761_1_SDI    GPIOB,GPIO_Pin_14  /* 定义方向为AD5761_1方向        */
#define AD5761_1_SDO    GPIOB,GPIO_Pin_15

#define AD5761_1_CLK_HIGH()  GPIO_SetBits(AD5761_1_SCLK)
#define AD5761_1_CLK_LOW()   GPIO_ResetBits(AD5761_1_SCLK)

#define AD5761_1_SDI_HIGH()  GPIO_SetBits(AD5761_1_SDI)
#define AD5761_1_SDI_LOW()   GPIO_ResetBits(AD5761_1_SDI)

#define AD5761_1_SDO_IN()    GPIO_ReadOutputDataBit(AD5761_1_SDO)

#define AD5761_1_SYNC_HIGH()  GPIO_SetBits(AD5761_1_SYNC)
#define AD5761_1_SYNC_LOW()   GPIO_ResetBits(AD5761_1_SYNC)

#define AD5761_1_CLEAR_HIGH()  GPIO_SetBits(AD5761_1_CLEAR)
#define AD5761_1_CLEAR_LOW()   GPIO_ResetBits(AD5761_1_CLEAR)

#define AD5761_1_RESET_HIGH()  GPIO_SetBits(AD5761_1_RESET)
#define AD5761_1_RESET_LOW()   GPIO_ResetBits(AD5761_1_RESET)

#define AD5761_1_LDAC_HIGH()  GPIO_SetBits(AD5761_1_LDAC)
#define AD5761_1_LDAC_LOW()   GPIO_ResetBits(AD5761_1_LDAC)

/* SIG通道2   */
#define AD5761_2_ALERT  GPIOG,GPIO_Pin_6 
#define AD5761_2_CLEAR  GPIOG,GPIO_Pin_7
#define AD5761_2_RESET  GPIOG,GPIO_Pin_8
#define AD5761_2_LDAC   GPIOC,GPIO_Pin_6

#define AD5761_2_SYNC   GPIOA,GPIO_Pin_4
#define AD5761_2_SCLK   GPIOA,GPIO_Pin_5
#define AD5761_2_SDI    GPIOA,GPIO_Pin_6  /* 定义方向为AD5761_2方向        */
#define AD5761_2_SDO    GPIOA,GPIO_Pin_7

#define AD5761_2_CLK_HIGH()  GPIO_SetBits(AD5761_2_SCLK)
#define AD5761_2_CLK_LOW()   GPIO_ResetBits(AD5761_2_SCLK)

#define AD5761_2_SDI_HIGH()  GPIO_SetBits(AD5761_2_SDI)
#define AD5761_2_SDI_LOW()   GPIO_ResetBits(AD5761_2_SDI)

#define AD5761_2_SDO_IN()    GPIO_ReadOutputDataBit(AD5761_2_SDO)

#define AD5761_2_SYNC_HIGH()  GPIO_SetBits(AD5761_2_SYNC)
#define AD5761_2_SYNC_LOW()   GPIO_ResetBits(AD5761_2_SYNC)

#define AD5761_2_CLEAR_HIGH()  GPIO_SetBits(AD5761_2_CLEAR)
#define AD5761_2_CLEAR_LOW()   GPIO_ResetBits(AD5761_2_CLEAR)

#define AD5761_2_RESET_HIGH()  GPIO_SetBits(AD5761_2_RESET)
#define AD5761_2_RESET_LOW()   GPIO_ResetBits(AD5761_2_RESET)

#define AD5761_2_LDAC_HIGH()  GPIO_SetBits(AD5761_2_LDAC)
#define AD5761_2_LDAC_LOW()   GPIO_ResetBits(AD5761_2_LDAC)

void  AD5761_1_reset(void);
void  AD5761_1_init(void); /* AD5761_1初始化        */
void  AD5761_1_config(void);
void  AD5761_1_write(u8 addr,u16 data); /* 写入参数 */
u16   AD5761_1_read(u8 addr); /* 读取参数 */

void  AD5761_2_reset(void);
void  AD5761_2_init(void);/* AD5761_2初始化        */
void  AD5761_2_config(void);
void  AD5761_2_write(u8 addr,u16 data);/* 写入参数 */
u16   AD5761_2_read(u8 addr);/* 读取参数 */

void SigOut_Handle(void);/* 信号输出处理 */
	
#endif

/************************ (C) COPYRIGHT Hopewellgroup ***********END OF FILE***/

