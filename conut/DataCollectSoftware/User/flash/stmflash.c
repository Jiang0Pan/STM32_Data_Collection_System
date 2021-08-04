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
函数名称：STMFLASH_ReadWord(u32 faddr)
功    能：读取指定地址的半字(16位数据) 
说    明：
参    数: faddr:读地址 
返    回：对应数据.
注    意：
*******************************************************************************/
u32 STMFLASH_ReadWord(u32 faddr)
{
    return *(vu32*)faddr; 
}  

/*******************************************************************************
函数名称：STMFLASH_ReadWord(u32 faddr)
功    能：获取某个地址所在的flash扇区
说    明：
参    数: addr:flash地址
返    回：返回值:0~11,即addr所在的扇区
注    意：
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
函数名称：MEM_If_Init_FS()
功    能：写FLASH初始化
说    明：
参    数: 
返    回：
注    意：
*******************************************************************************/
void MEM_If_Init_FS(void)
{
 
    HAL_FLASH_Unlock(); 
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
                          FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
 
}

/*******************************************************************************
函数名称：MEM_If_Init_FS()
功    能：写完FLASH后锁定写
说    明：
参    数: 
返    回：
注    意：
*******************************************************************************/
void MEM_If_DeInit_FS(void) 
{
    HAL_FLASH_Lock();
}

/*******************************************************************************
函数名称：MEM_If_Erase_FS(u32 start_Add,u32 end_Add)
功    能：擦除FLASH
说    明：
参    数: start_Add:擦除首地址 end_Add:擦除结束地址
返    回：
注    意：
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
函数名称：STMFLASH_Write(u32 WriteAddr,u32 *pBuffer,u32 NumToWrite)	
功    能：从指定地址开始写入指定长度的数据
说    明：
参    数: WriteAddr:起始地址(此地址必须为4的倍数!!)
          pBuffer:数据指针
          NumToWrite:字(32位)数
返    回：
注    意：特别注意:因为STM32F4的扇区实在太大,没办法本地保存扇区数据,所以本函数
          写地址如果非0XFF,那么会先擦除整个扇区且不保存扇区数据.所以  写非0XFF
          的地址,将导致整个扇区数据丢失.建议写之前确保扇区里没有重要数据,最好是
          整个扇区先擦除了,然后慢慢往后写.该函数对OTP区域也有效!可以用来写OTP区!
          OTP区域地址范围:0X1FFF7800~0X1FFF7A0F
*******************************************************************************/
void STMFLASH_Write(u32 WriteAddr,u32 *pBuffer,u32 NumToWrite)	
{ 
    u32 addrx=0;
	u32 endaddr=0;	
  
	addrx=WriteAddr;				//写入的起始地址
	endaddr=WriteAddr+NumToWrite;	//写入的结束地址
	MEM_If_Erase_FS(addrx,endaddr);

	/*写数据*/
	while(WriteAddr<endaddr) {
		if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,WriteAddr,*pBuffer) != HAL_OK) break;
		WriteAddr+=4;
		pBuffer++;
	} 
	MEM_If_DeInit_FS();
} 

/*******************************************************************************
函数名称：STMFLASH_Read(u32 ReadAddr,u32 *pBuffer,u32 NumToRead)
功    能：从指定地址开始读出指定长度的数据
说    明：
参    数: ReadAddr:起始地址 pBuffer:数据指针 NumToRead:字(4位)数
返    回：
注    意：
*******************************************************************************/
void STMFLASH_Read(u32 ReadAddr,u32 *pBuffer,u32 NumToRead)   	
{
	u32 i;
	for(i=0;i<NumToRead;i++) {
		pBuffer[i]=STMFLASH_ReadWord(ReadAddr);//读取4个字节.
		ReadAddr+=4;//偏移4个字节.	
	}
}

/************************ (C) COPYRIGHT Hopewellgroup ***********END OF FILE***/

