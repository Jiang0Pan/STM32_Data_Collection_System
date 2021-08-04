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
#define STM32_FLASH_BASE 0x08000000 	/* STM32 FLASH的起始地址 */
#define FLASH_WAITETIME  50000          /* FLASH等待超时时间           */

/* FLASH 扇区的起始地址 */
#define ADDR_FLASH_SECTOR_0     ((U32)0x08000000) 	/* 扇区0起始地址, 16 Kbytes  */  
#define ADDR_FLASH_SECTOR_1     ((U32)0x08004000) 	/* 扇区1起始地址, 16 Kbytes  */  
#define ADDR_FLASH_SECTOR_2     ((U32)0x08008000) 	/* 扇区2起始地址, 16 Kbytes  */  
#define ADDR_FLASH_SECTOR_3     ((U32)0x0800C000) 	/* 扇区3起始地址, 16 Kbytes  */   
#define ADDR_FLASH_SECTOR_4     ((U32)0x08010000) 	/* 扇区4起始地址, 64 Kbytes  */    
#define ADDR_FLASH_SECTOR_5     ((U32)0x08020000) 	/* 扇区5起始地址, 128 Kbytes */    
#define ADDR_FLASH_SECTOR_6     ((U32)0x08040000) 	/* 扇区6起始地址, 128 Kbytes */   
#define ADDR_FLASH_SECTOR_7     ((U32)0x08060000) 	/* 扇区7起始地址, 128 Kbytes */   
#define ADDR_FLASH_SECTOR_8     ((U32)0x08080000) 	/* 扇区8起始地址, 128 Kbytes */   
#define ADDR_FLASH_SECTOR_9     ((U32)0x080A0000) 	/* 扇区9起始地址, 128 Kbytes */   
#define ADDR_FLASH_SECTOR_10    ((U32)0x080C0000) 	/* 扇区10起始地址,128 Kbytes */   
#define ADDR_FLASH_SECTOR_11    ((U32)0x080E0000) 	/* 扇区11起始地址,128 Kbytes */  

/* function declaration */ 
U32  stmflash_readword(U32 faddr); /* 读出字  */
void stmflash_write(U32 WriteAddr,U32 *pBuffer,U32 NumToWrite);
void stmflash_read(U32 ReadAddr,U32 *pBuffer,U32 NumToRead);
U8   stm32flash_getflashsector(U32 addr);


#endif


/************************ (C) COPYRIGHT Hopewellgroup ***********END OF FILE***/

