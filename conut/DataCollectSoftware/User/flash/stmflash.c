/**
  ******************************************************************************
  * @file           : stmflash.c
  * @brief          : handle of flash
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

#include "system_config.h"
 
/*******************************************************************************
�������ƣ�STMFLASH_ReadWord(u32 faddr)
��    �ܣ���ȡָ����ַ�İ���(16λ����) 
˵    ����
��    ��: faddr:����ַ 
��    �أ���Ӧ����.
ע    �⣺
*******************************************************************************/
u32 STMFLASH_ReadWord(u32 faddr)
{
    return *(vu32*)faddr; 
}  

/*******************************************************************************
�������ƣ�STMFLASH_ReadWord(u32 faddr)
��    �ܣ���ȡĳ����ַ���ڵ�flash����
˵    ����
��    ��: addr:flash��ַ
��    �أ�����ֵ:0~11,��addr���ڵ�����
ע    �⣺
*******************************************************************************/
static u32 GetSector(u32 Address)
{
    uint32_t sector = 0;
  
    if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
    {
        sector = FLASH_SECTOR_0;  
    }
    else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
    {
        sector = FLASH_SECTOR_1;  
    }
    else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
    {
        sector = FLASH_SECTOR_2;  
    }
    else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
    {
        sector = FLASH_SECTOR_3;  
    }
    else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
    {
        sector = FLASH_SECTOR_4;  
    }
    else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
    {
        sector = FLASH_SECTOR_5;  
    }
    else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
    {
        sector = FLASH_SECTOR_6;  
    }
    else if((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7))
    {
        sector = FLASH_SECTOR_7;  
    }
    else if((Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8))
    {
        sector = FLASH_SECTOR_8;  
    }
    else if((Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9))
    {
        sector = FLASH_SECTOR_9;  
    }
    else if((Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10))
    {
        sector = FLASH_SECTOR_10;  
    }
    else
    {
        sector = FLASH_SECTOR_11;  
    }
 
  return sector;
}

/*******************************************************************************
�������ƣ�MEM_If_Init_FS()
��    �ܣ�дFLASH��ʼ��
˵    ����
��    ��: 
��    �أ�
ע    �⣺
*******************************************************************************/
void MEM_If_Init_FS(void)
{
 
    HAL_FLASH_Unlock(); 
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
                          FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
 
}

/*******************************************************************************
�������ƣ�MEM_If_Init_FS()
��    �ܣ�д��FLASH������д
˵    ����
��    ��: 
��    �أ�
ע    �⣺
*******************************************************************************/
void MEM_If_DeInit_FS(void) 
{
    HAL_FLASH_Lock();
}

/*******************************************************************************
�������ƣ�MEM_If_Erase_FS(u32 start_Add,u32 end_Add)
��    �ܣ�����FLASH
˵    ����
��    ��: start_Add:�����׵�ַ end_Add:����������ַ
��    �أ�
ע    �⣺
*******************************************************************************/
u16 MEM_If_Erase_FS(u32 start_Add,u32 end_Add)
{
    /* USER CODE BEGIN 3 */
    u32 UserStartSector;
    u32 SectorError;
    FLASH_EraseInitTypeDef pEraseInit;
 
    /* Unlock the Flash to enable the flash control register access *************/
    MEM_If_Init_FS();
 
    /* Get the sector where start the user flash area */
    UserStartSector = GetSector(start_Add);
 
    pEraseInit.TypeErase = TYPEERASE_SECTORS;
    pEraseInit.Sector = UserStartSector;
    pEraseInit.NbSectors = GetSector(end_Add)-UserStartSector+1 ;
    pEraseInit.VoltageRange = VOLTAGE_RANGE_3;
 
    if (HAL_FLASHEx_Erase(&pEraseInit, &SectorError) != HAL_OK) {
        /* Error occurred while page erase */
        return (0);
    }
 
    return 1;
    /* USER CODE END 3 */
}

/*******************************************************************************
�������ƣ�STMFLASH_Write(u32 WriteAddr,u32 *pBuffer,u32 NumToWrite)	
��    �ܣ���ָ����ַ��ʼд��ָ�����ȵ�����
˵    ����
��    ��: WriteAddr:��ʼ��ַ(�˵�ַ����Ϊ4�ı���!!)
          pBuffer:����ָ��
          NumToWrite:��(32λ)��
��    �أ�
ע    �⣺�ر�ע��:��ΪSTM32F4������ʵ��̫��,û�취���ر�����������,���Ա�����
          д��ַ�����0XFF,��ô���Ȳ������������Ҳ�������������.����  д��0XFF
          �ĵ�ַ,�����������������ݶ�ʧ.����д֮ǰȷ��������û����Ҫ����,�����
          ���������Ȳ�����,Ȼ����������д.�ú�����OTP����Ҳ��Ч!��������дOTP��!
          OTP�����ַ��Χ:0X1FFF7800~0X1FFF7A0F
*******************************************************************************/
void STMFLASH_Write(u32 WriteAddr,u32 *pBuffer,u32 NumToWrite)	
{ 
    u32 addrx=0;
	u32 endaddr=0;	
  
	addrx=WriteAddr;				//д�����ʼ��ַ
	endaddr=WriteAddr+NumToWrite;	//д��Ľ�����ַ
	MEM_If_Erase_FS(addrx,endaddr);

	/*д����*/
	while(WriteAddr<endaddr) {
		if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,WriteAddr,*pBuffer) != HAL_OK) break;
		WriteAddr+=4;
		pBuffer++;
	} 
	MEM_If_DeInit_FS();
} 

/*******************************************************************************
�������ƣ�STMFLASH_Read(u32 ReadAddr,u32 *pBuffer,u32 NumToRead)
��    �ܣ���ָ����ַ��ʼ����ָ�����ȵ�����
˵    ����
��    ��: ReadAddr:��ʼ��ַ pBuffer:����ָ�� NumToRead:��(4λ)��
��    �أ�
ע    �⣺
*******************************************************************************/
void STMFLASH_Read(u32 ReadAddr,u32 *pBuffer,u32 NumToRead)   	
{
	u32 i;
	for(i=0;i<NumToRead;i++) {
		pBuffer[i]=STMFLASH_ReadWord(ReadAddr);//��ȡ4���ֽ�.
		ReadAddr+=4;//ƫ��4���ֽ�.	
	}
}

/************************ (C) COPYRIGHT Hopewellgroup ***********END OF FILE***/

