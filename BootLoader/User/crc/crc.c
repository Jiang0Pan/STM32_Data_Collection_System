/**
  ******************************************************************************
  * @file           : crc.c
  * @brief          : crc by soft
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2ZERO2ZERO hopewellgroup.
  * All rights reserved.
  *
  * This software is licensed by hopewellgroup under Ultimate Liberty license
  *
  ******************************************************************************
  */

/* include fils */
#include "stm32f4xx_hal.h"
#include "system_config.h"
#include "usart.h"
#include "crc.h"

/* define */
/* define */
#define  UARTHEAD            (0xFEEF)     				  /* 接收头数据            */
#define  UARTTAIL            (0xEAAE)     				  /* 接收尾数据            */
#define  BootLoader_Version  ("2.0.0")    				  /* boot版本           */

#define  DEFINE_VAL 		 (0xFFFF)
#define  NO_TOP_BIT 	     (0x7FFF)
#define  CRC_VAL    	     (0xA001)
#define  MAX_BITS   		 (8)

extern U16      chkval;
extern U16      calchkval;									  /* 计算得到的校验值 */	
extern SysSts 	g_sys_sts;

/*******************************************************************************
函数名称：mc_check_crc16
功    能： 进行软件CRC校验
说    明：
参    数:lpByte：需要校验的数据指针
        iLen：数据长度
返    回:CRC校验值
注    意：
*******************************************************************************/
U16 mc_check_crc16(U8 *lpByte,U16 iLen)
{
	U16 iCrc = ZERO;
	U16 i = ZERO;
	U16 n = ZERO;
	
	if (iLen > ZERO) {
		iCrc = DEFINE_VAL;
		for (n = ZERO; n < iLen; n++) {
			iCrc = (U16)(iCrc ^ lpByte[n]);
			for (i = ZERO; i < MAX_BITS; i++) {
				U16 TT;
				TT = (U16)(iCrc & 1);
				iCrc = (U16)(iCrc >> 1);
				iCrc = (U16)(iCrc & NO_TOP_BIT);
				if (TT == 1) {
					iCrc = (U16)(iCrc ^ CRC_VAL);
				}
				iCrc = (U16)(iCrc & DEFINE_VAL);
			}
		}
	}
	return iCrc;
}

/*******************************************************************************
函数名称：modbus_action(void)
功    能： 接收后的数据处理
说    明：
参    数:
返    回: 1代表正在接收数据，2复位，3代表不处理，0代表失败
注    意：
*******************************************************************************/
U8 modbus_action(void)
{
	U8 flag = 0;   /* 0校验没过  1升级数据  2升级命令,3不知道什么数据，过滤此帧 */
	chkval = ((U16)g_sys_sts.uart_frm.RxUartData.databuf[1] 
			<< 8) + g_sys_sts.uart_frm.RxUartData.databuf[0];
	calchkval = mc_check_crc16((U8 *)&g_sys_sts.uart_frm.RxUartData.databuf[2],
			g_sys_sts.uart_frm.RxUartData.iLenght-2);/* 先校验	  */
	if (chkval != calchkval) {
		mp_send_data_nolen((U8 *)"Error\n")	;	
		return 0;
	} 															  /* 校验错误 */
	if ((UARTHEAD == g_sys_sts.uart_frm.RxUartData.header) && 
			(UARTTAIL == g_sys_sts.uart_frm.RxUartTail.tailer)) {
		switch (g_sys_sts.uart_frm.RxUartData.commend) {
			case 0x0000:
				flag = 2;
				__set_PRIMASK(1);
				__set_FAULTMASK(1);
			    NVIC_SystemReset();								 /* 复位 	      */
				break;
			case 0x0004:
				flag=1;
				break;
			default:
				flag=3;
				break;
		}
	}
	else { 
		flag=3;  
	}
	return flag;
}

/************************ (C) COPYRIGHT Hopewellgroup ***********END OF FILE***/

