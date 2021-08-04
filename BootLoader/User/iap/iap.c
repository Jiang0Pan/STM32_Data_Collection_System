/**
  ******************************************************************************
  * @file           : iap.c
  * @brief          : upgrade application
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

volatile SYS_FLAG_Struct SYSFLAG; /* 系统标志位 */


extern U16 Totalframe;
extern U16 cntframe;	
unsigned char appNVbuf[GET_NV_SIZE()] ;

Iaptemp_Struct Iapstruct;

/*******************************************************************************
函数名称：iap_write_appbin
功    能：将文件写入到FLASH中
说    明：
参    数:appbuf：写入的数据指针
        appsize：需要写入的数据大小
返    回:
注    意：
*******************************************************************************/
void iap_write_appbin(U8 *appbuf,U32 appsize)
{
	U16 i = 0;
	for (i=0; i<appsize; i++) {
		Iapstruct.buf[Iapstruct.cnt++] = appbuf[i];
		if (Iapstruct.cnt == GET_NV_SIZE()){
			iap_write_appbinreal(Iapstruct.buf,GET_NV_SIZE());
			Iapstruct.cnt = 0;
		}
	}

	if (cntframe == (Totalframe - 1)) {/* 最后一帧了，写进app空间，然后就要跳转了 */
		iap_write_appbinreal(Iapstruct.buf,Iapstruct.cnt);
        Iapstruct.cnt = 0;
	}
}

/*******************************************************************************
函数名称：iap_write_appbinreal
功    能：将数据写入到FLSAH中
说    明：
参    数:
返    回:
注    意：
*******************************************************************************/
void iap_write_appbinreal(U8 *appbuf,U32 appsize)/* 完整的一页或者不足一页 */
{

	static U16 totalpage = 0;
	static U16 i = 0;
	static U16 pagesize = GET_NV_SIZE();/* mcu一页多少字节 */
	static U32 appxaddrtemp = FLASH_APP_ADDR;
	totalpage = appsize/pagesize + 1;
	for (i = 0; i <(totalpage - 1); i++) {
		nv_writeapp(appxaddrtemp,appbuf + i * pagesize,pagesize);
		appxaddrtemp = appxaddrtemp + pagesize;
	}
	nv_writeapp(appxaddrtemp,appbuf + i * pagesize,appsize - i * pagesize);
	appxaddrtemp = appxaddrtemp + (appsize-i*pagesize);
}

/*******************************************************************************
函数名称：nv_writeapp
功    能：写入FLASH
说    明：WriteAddr因为是写app的bin文件，所有地址必须是页的开头地址 
         即1024 0x400的的整数倍
参    数: WriteAddr：写入的地址
		 pBuffer：数据指针
		 NumToWrite：GET_NV_SIZE() 128B  
返    回:
注    意：
*******************************************************************************/
void nv_writeapp(U32 WriteAddr,U8 *pBuffer,U16 NumToWrite)
{
	U16 lentemp = 0;
	if (NumToWrite % 4) {
		lentemp = (NumToWrite / 4) + 1;
	}
	else {
		lentemp = (NumToWrite / 4);
	}
	stmflash_write(WriteAddr,(U32 *)pBuffer,lentemp);
}
iapfun jump2app; 

/*******************************************************************************
函数名称：iap_load_app
功    能：跳转到应用程序段
说    明：
参    数:appxaddr:用户代码起始地址.
返    回:
注    意：
*******************************************************************************/
void iap_load_app(U32 appxaddr)
{
	/* 检查栈顶地址是否合法. */
	if (((*(VU32*)appxaddr) & 0x2FFE0000) == 0x20000000) {	
		__set_PRIMASK(1);
		__set_FAULTMASK(1);
		/* 用户代码区第二个字为程序开始地址(复位地址)	*/	
		jump2app = (iapfun)*(VU32*)(appxaddr + 4);	
		/* 初始化APP堆栈指针(用户代码区的第一个字用于存放栈顶地址) */
		MSR_MSP(*(VU32*)appxaddr);		
		/* 跳转到APP.*/
		jump2app();									
	}
}	

/*******************************************************************************
函数名称：MSR_MSP
功    能：设置栈顶地址
说    明：
参    数:addr:栈顶地址
返    回:
注    意：
*******************************************************************************/
__asm void MSR_MSP(U32 addr) 
{
	MSR MSP, r0 			/* set Main Stack value */
	BX r14
}


/************************ (C) COPYRIGHT Hopewellgroup ***********END OF FILE***/

