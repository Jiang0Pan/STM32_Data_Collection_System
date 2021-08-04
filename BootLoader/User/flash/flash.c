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
/* include fils */
#include "stm32f4xx_hal.h"
#include "system_config.h"
#include "flash.h"
#include "iap.h"
	
/* define */

/*******************************************************************************
函数名称：stmflash_readword
功    能：读取flash相应地址的四字节数据
说    明：
参    数: faddr:flash地址
返    回：返回flash的数据
注    意：
*******************************************************************************/
U32 stmflash_readword(U32 faddr)
{
	return *(VU32*)faddr; 
}

/*******************************************************************************
函数名称：stm32flash_getflashsector
功    能：获取某个地址所在的flash扇区
说    明：
参    数: addr:flash地址
返    回：返回值:0~11,即addr所在的扇区
注    意：
*******************************************************************************/
U8   stm32flash_getflashsector(U32 addr)
{
	if(addr<ADDR_FLASH_SECTOR_1) {
		return FLASH_SECTOR_0;
	}
	else if (addr<ADDR_FLASH_SECTOR_2) { 
		return FLASH_SECTOR_1;
	}
	else if(addr<ADDR_FLASH_SECTOR_3) {
		return FLASH_SECTOR_2;
	}
	else if (addr<ADDR_FLASH_SECTOR_4) {
		return FLASH_SECTOR_3;
	}
	else if (addr<ADDR_FLASH_SECTOR_5) {
		return FLASH_SECTOR_4;
	}
	else if (addr<ADDR_FLASH_SECTOR_6) {
		return FLASH_SECTOR_5;
	}
	else if (addr<ADDR_FLASH_SECTOR_7) {
		return FLASH_SECTOR_6;
	}
	else if (addr<ADDR_FLASH_SECTOR_8) { 
		return FLASH_SECTOR_7;
	}
	else if (addr<ADDR_FLASH_SECTOR_9) {
		return FLASH_SECTOR_8;
	}
	else if (addr<ADDR_FLASH_SECTOR_10) {
		return FLASH_SECTOR_9;
	}
	else if (addr<ADDR_FLASH_SECTOR_11) {
		return FLASH_SECTOR_10;
	}
	return FLASH_SECTOR_11;	
}

/*******************************************************************************
函数名称：stmflash_write
功    能：从指定地址开始写入指定长度的数据
说    明：
参    数:WriteAddr:起始地址(此地址必须为4的倍数!!)
	    pBuffer:数据指针
        NumToWrite:字(32位)数(就是要写入的32位数据的个数.) 
返    回：返回值:0~11,即addr所在的扇区
注    意：因为STM32F4的扇区实在太大,没办法本地保存扇区数据,所以本函数
         写地址如果非0XFF,那么会先擦除整个扇区且不保存扇区数据.所以
         写非0XFF的地址,将导致整个扇区数据丢失.建议写之前确保扇区里
         没有重要数据,最好是整个扇区先擦除了,然后慢慢往后写.
         该函数对OTP区域也有效!可以用来写OTP区!
         OTP区域地址范围:0X1FFF7800~0X1FFF7A0F
         (注意：最后16字节，用于OTP数据块锁定，别乱写！！)
*******************************************************************************/
void stmflash_write(U32 WriteAddr,U32 *pBuffer,U32 NumToWrite)	
{ 
	FLASH_EraseInitTypeDef FlashEraseInit;
	HAL_StatusTypeDef FlashStatus = HAL_OK;
	U32 SectorError = 0;
	U32 addrx = 0;
	U32 endaddr = 0;	
	if((WriteAddr < STM32_FLASH_BASE) || (WriteAddr % 4)) {
		return;	//非法地址
	}
	HAL_FLASH_Unlock();            	 		/* 解锁			   */
	addrx = WriteAddr;				 		/* 写入的起始地址         */
	endaddr = WriteAddr + NumToWrite * 4;	/* 写入的结束地址  */
    
	if (addrx < 0X1FFF0000) {
		/* 扫清一切障碍.(对非FFFFFFFF的地方,先擦除)               */
		while (addrx < endaddr) {
			 /* 有非0XFFFFFFFF的地方,要擦除这个扇区  */
			 if (stmflash_readword(addrx) != 0XFFFFFFFF) { 
			 	/* 擦除类型，扇区擦除          */
				FlashEraseInit.TypeErase = FLASH_TYPEERASE_SECTORS; 
				/* 要擦除的扇区       */
				FlashEraseInit.Sector = stm32flash_getflashsector(addrx); 
			    /* 一次只擦除一个扇区          */
				FlashEraseInit.NbSectors = 1;   
				/* 电压范围，VCC=2.7~3.6V之间!! */
				FlashEraseInit.VoltageRange = FLASH_VOLTAGE_RANGE_3;      
				if(HAL_FLASHEx_Erase(&FlashEraseInit,&SectorError) != HAL_OK) {
					break;/* 发生错误了 */	
				}
			 }
			 else {
				addrx+=4;
			 }
			 FLASH_WaitForLastOperation(FLASH_WAITETIME); /* 等待上次操作完成 */
	    }
	}
	/* 等待上次操作完成 */
	FlashStatus = FLASH_WaitForLastOperation(FLASH_WAITETIME);
	if (FlashStatus == HAL_OK) {
		 while (WriteAddr<endaddr) {/* 写数据 */
			if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,WriteAddr,*pBuffer) != 
					HAL_OK) { /* 写入数据 */
				break;	/* 写入异常 */
			}
			WriteAddr += 4;
			pBuffer++;
		}  
	}
	HAL_FLASH_Lock();           /* 上锁 */
} 

/*******************************************************************************
函数名称：stmflash_read 
功    能：从指定地址开始读出指定长度的数据
说    明：
参    数: ReadAddr:起始地址
	     pBuffer:数据指针
         NumToRead:字(32位)数
返    回：
注    意：
*******************************************************************************/
void stmflash_read(U32 ReadAddr,U32 *pBuffer,U32 NumToRead)   	
{
	U32 i;
	for (i=0; i<NumToRead; i++) {
		pBuffer[i] = stmflash_readword(ReadAddr);/* 读取4个字节. */
		ReadAddr += 4;/* 偏移4个字节. */	
	}
}


/************************ (C) COPYRIGHT Hopewellgroup ***********END OF FILE***/

