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

#define FLASH_APP_ADDR	(0x08010000)  /* ��һ��Ӧ�ó�����ʼ��ַ(�����FLASH)*/
#define	FLASH_ENV_ADDR	(0x080E0000)  /* ������������ʼ��ַ 0x080E0000 */
#define	FLASH_UPGRADE_EN	(0x8888)  /* ������־: 0x8888          */

//#define FLASH_APP1_ADDR		(0x08020000)  	/* ��һ��Ӧ�ó�����ʼ��ַ(�����FLASH) */
//#define	NV_START_ADDR	    (0x0801FC00)    /* ������־λ�Ĵ����ʼ��ַ                   */
#define	GET_NV_SIZE()		(128)           /* FLASHһ����һҳ�Ĵ�С                  */
#define BOOT  				(4*1)			/* ������־λ�Ĵ��ƫ�Ƶ�ַ                   */										
typedef  void (*iapfun)(void);				/* ����һ���������͵Ĳ���.        		      */

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

extern volatile SYS_FLAG_Struct SYSFLAG; /*ϵͳ��־λ*/
extern Iaptemp_Struct Iapstruct;

/* function declaration */ 
void iap_load_app(U32 appxaddr);			/* ִ��flash�����app���� */
void iap_write_appbin(U8 *appbuf,U32 applen);
void nv_writeapp(U32 WriteAddr,U8 *pBuffer,U16 NumToWrite);
void iap_write_appbinreal(U8 *appbuf,U32 appsize);/*д���������Ϲ�һҳ������д*/
__asm void MSR_MSP(U32 addr);

#endif

/************************ (C) COPYRIGHT Hopewellgroup ***********END OF FILE***/

