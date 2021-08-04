/**
  ******************************************************************************
  * @file           : iap.h
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

#ifndef __IAP_H
#define __IAP_H

#define FLASH_APP_ADDR	(0x08010000)  /* 第一个应用程序起始地址(存放在FLASH)*/
#define	FLASH_ENV_ADDR	(0x080E0000)  /* 环境变量的起始地址 0x080E0000 */
#define	FLASH_UPGRADE_EN	(0x8888)  /* 升级标志: 0x8888          */

//#define FLASH_APP1_ADDR		(0x08020000)  	/* 第一个应用程序起始地址(存放在FLASH) */
//#define	NV_START_ADDR	    (0x0801FC00)    /* 升级标志位的存放起始地址                   */
#define	GET_NV_SIZE()		(128)           /* FLASH一块中一页的大小                  */
#define BOOT  				(4*1)			/* 升级标志位的存放偏移地址                   */										
typedef  void (*iapfun)(void);				/* 定义一个函数类型的参数.        		      */

typedef  U8 bool ;
typedef struct	
{
	U8 bootflag;
}SYS_FLAG_Struct;

typedef struct	
{
	U8 buf[GET_NV_SIZE()];
	U16  cnt;
}Iaptemp_Struct;

extern volatile SYS_FLAG_Struct SYSFLAG; /*系统标志位*/
extern Iaptemp_Struct Iapstruct;

/* function declaration */ 
void iap_load_app(U32 appxaddr);			/* 执行flash里面的app程序 */
void iap_write_appbin(U8 *appbuf,U32 applen);
void nv_writeapp(U32 WriteAddr,U8 *pBuffer,U16 NumToWrite);
void iap_write_appbinreal(U8 *appbuf,U32 appsize);/*写入的数据组合够一页再往里写*/
__asm void MSR_MSP(U32 addr);

#endif

/************************ (C) COPYRIGHT Hopewellgroup ***********END OF FILE***/

