/**
  ******************************************************************************
  * @file           : wificonfig.h
  * @brief          : Header for wificonfig.c file.
  *                   This file contains the common defines of the application.
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

#ifndef __STMFLASH_H__
#define __STMFLASH_H__
#include "stm32f4xx_hal.h" 

typedef uint32_t  u32;
typedef uint8_t   u8;
typedef uint16_t  u16;   
typedef __IO uint32_t  vu32;


#define FLASH_SAVE_ADDR_CONFIG  0X0800CC00 	//设置配置存储位置
//FLASH起始地址
#define STM32_FLASH_BASE        0x08000000 	//STM32 FLASH的起始地址
 
#define FLASH_Sector_0     ((uint16_t)0x0000) /*!< Sector Number 0   */
#define FLASH_Sector_1     ((uint16_t)0x0008) /*!< Sector Number 1   */
#define FLASH_Sector_2     ((uint16_t)0x0010) /*!< Sector Number 2   */
#define FLASH_Sector_3     ((uint16_t)0x0018) /*!< Sector Number 3   */
#define FLASH_Sector_4     ((uint16_t)0x0020) /*!< Sector Number 4   */
#define FLASH_Sector_5     ((uint16_t)0x0028) /*!< Sector Number 5   */
#define FLASH_Sector_6     ((uint16_t)0x0030) /*!< Sector Number 6   */
#define FLASH_Sector_7     ((uint16_t)0x0038) /*!< Sector Number 7   */
#define FLASH_Sector_8     ((uint16_t)0x0040) /*!< Sector Number 8   */
#define FLASH_Sector_9     ((uint16_t)0x0048) /*!< Sector Number 9   */
#define FLASH_Sector_10    ((uint16_t)0x0050) /*!< Sector Number 10  */
#define FLASH_Sector_11    ((uint16_t)0x0058) /*!< Sector Number 11  */
#define FLASH_Sector_12    ((uint16_t)0x0080) /*!< Sector Number 12  */
#define FLASH_Sector_13    ((uint16_t)0x0088) /*!< Sector Number 13  */
#define FLASH_Sector_14    ((uint16_t)0x0090) /*!< Sector Number 14  */
#define FLASH_Sector_15    ((uint16_t)0x0098) /*!< Sector Number 15  */
#define FLASH_Sector_16    ((uint16_t)0x00A0) /*!< Sector Number 16  */
#define FLASH_Sector_17    ((uint16_t)0x00A8) /*!< Sector Number 17  */
#define FLASH_Sector_18    ((uint16_t)0x00B0) /*!< Sector Number 18  */
#define FLASH_Sector_19    ((uint16_t)0x00B8) /*!< Sector Number 19  */
#define FLASH_Sector_20    ((uint16_t)0x00C0) /*!< Sector Number 20  */
#define FLASH_Sector_21    ((uint16_t)0x00C8) /*!< Sector Number 21  */
#define FLASH_Sector_22    ((uint16_t)0x00D0) /*!< Sector Number 22  */
#define FLASH_Sector_23    ((uint16_t)0x00D8) /*!< Sector Number 23  */
//FLASH 扇区的起始地址
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base @ of Sector 0, 16 Kbyte */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) /* Base @ of Sector 1, 16 Kbyte */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) /* Base @ of Sector 2, 16 Kbyte */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) /* Base @ of Sector 3, 16 Kbyte */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) /* Base @ of Sector 4, 64 Kbyte */
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) /* Base @ of Sector 5, 128 Kbyte */
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) /* Base @ of Sector 6, 128 Kbyte */
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) /* Base @ of Sector 7, 128 Kbyte */
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000) /* Base @ of Sector 8, 128 Kbyte */
#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000) /* Base @ of Sector 9, 128 Kbyte */
#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000) /* Base @ of Sector 10, 128 Kbyte */
#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000) /* Base @ of Sector 11, 128 Kbyte */


u32 STMFLASH_ReadWord(u32 faddr);		  	//读出字  
void STMFLASH_Write(u32 WriteAddr,u32 *pBuffer,u32 NumToWrite);		//从指定地址开始写入指定长度的数据
void STMFLASH_Read(u32 ReadAddr,u32 *pBuffer,u32 NumToRead);   		//从指定地址开始读出指定长度的数据
void Flash_WriteData(u32 addr,u16 *data,u16 Size);
u8 Flash_ReadData(u32 addr,u16 *data,u16 Size);
#endif

/************************ (C) COPYRIGHT Hopewellgroup ***********END OF FILE***/

