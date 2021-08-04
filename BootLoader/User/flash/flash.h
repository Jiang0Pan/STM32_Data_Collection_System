/**
  ******************************************************************************
  * @file           : flash.c
  * @brief          : flash driver
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

#ifndef __FLASH_H
#define __FLASH_H


/* define */
#define STM32_FLASH_BASE 0x08000000 	/* STM32 FLASH����ʼ��ַ */
#define FLASH_WAITETIME  50000          /* FLASH�ȴ���ʱʱ��           */

/* FLASH ��������ʼ��ַ */
#define ADDR_FLASH_SECTOR_0     ((U32)0x08000000) 	/* ����0��ʼ��ַ, 16 Kbytes  */  
#define ADDR_FLASH_SECTOR_1     ((U32)0x08004000) 	/* ����1��ʼ��ַ, 16 Kbytes  */  
#define ADDR_FLASH_SECTOR_2     ((U32)0x08008000) 	/* ����2��ʼ��ַ, 16 Kbytes  */  
#define ADDR_FLASH_SECTOR_3     ((U32)0x0800C000) 	/* ����3��ʼ��ַ, 16 Kbytes  */   
#define ADDR_FLASH_SECTOR_4     ((U32)0x08010000) 	/* ����4��ʼ��ַ, 64 Kbytes  */    
#define ADDR_FLASH_SECTOR_5     ((U32)0x08020000) 	/* ����5��ʼ��ַ, 128 Kbytes */    
#define ADDR_FLASH_SECTOR_6     ((U32)0x08040000) 	/* ����6��ʼ��ַ, 128 Kbytes */   
#define ADDR_FLASH_SECTOR_7     ((U32)0x08060000) 	/* ����7��ʼ��ַ, 128 Kbytes */   
#define ADDR_FLASH_SECTOR_8     ((U32)0x08080000) 	/* ����8��ʼ��ַ, 128 Kbytes */   
#define ADDR_FLASH_SECTOR_9     ((U32)0x080A0000) 	/* ����9��ʼ��ַ, 128 Kbytes */   
#define ADDR_FLASH_SECTOR_10    ((U32)0x080C0000) 	/* ����10��ʼ��ַ,128 Kbytes */   
#define ADDR_FLASH_SECTOR_11    ((U32)0x080E0000) 	/* ����11��ʼ��ַ,128 Kbytes */  

/* function declaration */ 
U32  stmflash_readword(U32 faddr); /* ������  */
void stmflash_write(U32 WriteAddr,U32 *pBuffer,U32 NumToWrite);
void stmflash_read(U32 ReadAddr,U32 *pBuffer,U32 NumToRead);
U8   stm32flash_getflashsector(U32 addr);


#endif


/************************ (C) COPYRIGHT Hopewellgroup ***********END OF FILE***/

