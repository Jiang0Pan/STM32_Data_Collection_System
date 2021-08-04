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
�������ƣ�stmflash_readword
��    �ܣ���ȡflash��Ӧ��ַ�����ֽ�����
˵    ����
��    ��: faddr:flash��ַ
��    �أ�����flash������
ע    �⣺
*******************************************************************************/
U32 stmflash_readword(U32 faddr)
{
	return *(VU32*)faddr; 
}

/*******************************************************************************
�������ƣ�stm32flash_getflashsector
��    �ܣ���ȡĳ����ַ���ڵ�flash����
˵    ����
��    ��: addr:flash��ַ
��    �أ�����ֵ:0~11,��addr���ڵ�����
ע    �⣺
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
�������ƣ�stmflash_write
��    �ܣ���ָ����ַ��ʼд��ָ�����ȵ�����
˵    ����
��    ��:WriteAddr:��ʼ��ַ(�˵�ַ����Ϊ4�ı���!!)
	    pBuffer:����ָ��
        NumToWrite:��(32λ)��(����Ҫд���32λ���ݵĸ���.) 
��    �أ�����ֵ:0~11,��addr���ڵ�����
ע    �⣺��ΪSTM32F4������ʵ��̫��,û�취���ر�����������,���Ա�����
         д��ַ�����0XFF,��ô���Ȳ������������Ҳ�������������.����
         д��0XFF�ĵ�ַ,�����������������ݶ�ʧ.����д֮ǰȷ��������
         û����Ҫ����,��������������Ȳ�����,Ȼ����������д.
         �ú�����OTP����Ҳ��Ч!��������дOTP��!
         OTP�����ַ��Χ:0X1FFF7800~0X1FFF7A0F
         (ע�⣺���16�ֽڣ�����OTP���ݿ�����������д����)
*******************************************************************************/
void stmflash_write(U32 WriteAddr,U32 *pBuffer,U32 NumToWrite)	
{ 
	FLASH_EraseInitTypeDef FlashEraseInit;
	HAL_StatusTypeDef FlashStatus = HAL_OK;
	U32 SectorError = 0;
	U32 addrx = 0;
	U32 endaddr = 0;	
	if((WriteAddr < STM32_FLASH_BASE) || (WriteAddr % 4)) {
		return;	//�Ƿ���ַ
	}
	HAL_FLASH_Unlock();            	 		/* ����			   */
	addrx = WriteAddr;				 		/* д�����ʼ��ַ         */
	endaddr = WriteAddr + NumToWrite * 4;	/* д��Ľ�����ַ  */
    
	if (addrx < 0X1FFF0000) {
		/* ɨ��һ���ϰ�.(�Է�FFFFFFFF�ĵط�,�Ȳ���)               */
		while (addrx < endaddr) {
			 /* �з�0XFFFFFFFF�ĵط�,Ҫ�����������  */
			 if (stmflash_readword(addrx) != 0XFFFFFFFF) { 
			 	/* �������ͣ���������          */
				FlashEraseInit.TypeErase = FLASH_TYPEERASE_SECTORS; 
				/* Ҫ����������       */
				FlashEraseInit.Sector = stm32flash_getflashsector(addrx); 
			    /* һ��ֻ����һ������          */
				FlashEraseInit.NbSectors = 1;   
				/* ��ѹ��Χ��VCC=2.7~3.6V֮��!! */
				FlashEraseInit.VoltageRange = FLASH_VOLTAGE_RANGE_3;      
				if(HAL_FLASHEx_Erase(&FlashEraseInit,&SectorError) != HAL_OK) {
					break;/* ���������� */	
				}
			 }
			 else {
				addrx+=4;
			 }
			 FLASH_WaitForLastOperation(FLASH_WAITETIME); /* �ȴ��ϴβ������ */
	    }
	}
	/* �ȴ��ϴβ������ */
	FlashStatus = FLASH_WaitForLastOperation(FLASH_WAITETIME);
	if (FlashStatus == HAL_OK) {
		 while (WriteAddr<endaddr) {/* д���� */
			if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,WriteAddr,*pBuffer) != 
					HAL_OK) { /* д������ */
				break;	/* д���쳣 */
			}
			WriteAddr += 4;
			pBuffer++;
		}  
	}
	HAL_FLASH_Lock();           /* ���� */
} 

/*******************************************************************************
�������ƣ�stmflash_read 
��    �ܣ���ָ����ַ��ʼ����ָ�����ȵ�����
˵    ����
��    ��: ReadAddr:��ʼ��ַ
	     pBuffer:����ָ��
         NumToRead:��(32λ)��
��    �أ�
ע    �⣺
*******************************************************************************/
void stmflash_read(U32 ReadAddr,U32 *pBuffer,U32 NumToRead)   	
{
	U32 i;
	for (i=0; i<NumToRead; i++) {
		pBuffer[i] = stmflash_readword(ReadAddr);/* ��ȡ4���ֽ�. */
		ReadAddr += 4;/* ƫ��4���ֽ�. */	
	}
}


/************************ (C) COPYRIGHT Hopewellgroup ***********END OF FILE***/

