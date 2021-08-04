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

volatile SYS_FLAG_Struct SYSFLAG; /* ϵͳ��־λ */


extern U16 Totalframe;
extern U16 cntframe;	
unsigned char appNVbuf[GET_NV_SIZE()] ;

Iaptemp_Struct Iapstruct;

/*******************************************************************************
�������ƣ�iap_write_appbin
��    �ܣ����ļ�д�뵽FLASH��
˵    ����
��    ��:appbuf��д�������ָ��
        appsize����Ҫд������ݴ�С
��    ��:
ע    �⣺
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

	if (cntframe == (Totalframe - 1)) {/* ���һ֡�ˣ�д��app�ռ䣬Ȼ���Ҫ��ת�� */
		iap_write_appbinreal(Iapstruct.buf,Iapstruct.cnt);
        Iapstruct.cnt = 0;
	}
}

/*******************************************************************************
�������ƣ�iap_write_appbinreal
��    �ܣ�������д�뵽FLSAH��
˵    ����
��    ��:
��    ��:
ע    �⣺
*******************************************************************************/
void iap_write_appbinreal(U8 *appbuf,U32 appsize)/* ������һҳ���߲���һҳ */
{

	static U16 totalpage = 0;
	static U16 i = 0;
	static U16 pagesize = GET_NV_SIZE();/* mcuһҳ�����ֽ� */
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
�������ƣ�nv_writeapp
��    �ܣ�д��FLASH
˵    ����WriteAddr��Ϊ��дapp��bin�ļ������е�ַ������ҳ�Ŀ�ͷ��ַ 
         ��1024 0x400�ĵ�������
��    ��: WriteAddr��д��ĵ�ַ
		 pBuffer������ָ��
		 NumToWrite��GET_NV_SIZE() 128B  
��    ��:
ע    �⣺
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
�������ƣ�iap_load_app
��    �ܣ���ת��Ӧ�ó����
˵    ����
��    ��:appxaddr:�û�������ʼ��ַ.
��    ��:
ע    �⣺
*******************************************************************************/
void iap_load_app(U32 appxaddr)
{
	/* ���ջ����ַ�Ƿ�Ϸ�. */
	if (((*(VU32*)appxaddr) & 0x2FFE0000) == 0x20000000) {	
		__set_PRIMASK(1);
		__set_FAULTMASK(1);
		/* �û��������ڶ�����Ϊ����ʼ��ַ(��λ��ַ)	*/	
		jump2app = (iapfun)*(VU32*)(appxaddr + 4);	
		/* ��ʼ��APP��ջָ��(�û��������ĵ�һ�������ڴ��ջ����ַ) */
		MSR_MSP(*(VU32*)appxaddr);		
		/* ��ת��APP.*/
		jump2app();									
	}
}	

/*******************************************************************************
�������ƣ�MSR_MSP
��    �ܣ�����ջ����ַ
˵    ����
��    ��:addr:ջ����ַ
��    ��:
ע    �⣺
*******************************************************************************/
__asm void MSR_MSP(U32 addr) 
{
	MSR MSP, r0 			/* set Main Stack value */
	BX r14
}


/************************ (C) COPYRIGHT Hopewellgroup ***********END OF FILE***/

