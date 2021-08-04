/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : init peripheral
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

#include "stm32f4xx_hal.h"
#include "system_config.h"
#include "usart.h"
#include "flash.h"
#include "iap.h"
#include "crc.h"
#include <string.h>
#include <stdio.h>

/* define */
#define  UARTHEAD            (0xFEEF)     				  /* 接收头数据            */
#define  UARTTAIL            (0xEAAE)     				  /* 接收尾数据            */
#define  FIRMWARE_VERSION    ("1.0.3")    				  /* boot版本           */

/* global variable */
U32  filelength = 0;      							  /* 文件长度 			  */
U16  calchkval = 0;									  /* 计算得到的校验值 */	
U16  chkval = 0;             						  /* 读出来的校验值   		  */
U16  Totalframe = 0;
U16  cntframe = 0;	
U16  stateiap = 0;	
U8   newmachine = 0;	
SysCfg g_sys_cfg;
SysSts g_sys_sts;

/* extern */
extern void system_stdinit(void);

int main(void)
{
	/*仅将环境变量从EEPROM改存到FLASH,其他数据不再保存*/
	stmflash_read(FLASH_ENV_ADDR,(U32*)&g_sys_cfg, sizeof(SysCfg));

	if(g_sys_cfg.u16upgradeflag != FLASH_UPGRADE_EN) /* 升级标志位没有置位 */
	{
		iap_load_app(FLASH_APP_ADDR);
	}
	
	__set_PRIMASK(0);
	__set_FAULTMASK(0);

	system_stdinit();
	system_init();
	
	printf("BootLoader_Version:%s\n",FIRMWARE_VERSION);
	
	while (1) {
		switch (stateiap) {
			case 0:	
				if (g_sys_sts.uart_frm.bComProc) {
					g_sys_sts.uart_frm.bComProc = 0;
					if ((g_sys_sts.uart_frm.RxUartData.databuf[0] == 'B') &&
							(g_sys_sts.uart_frm.RxUartData.databuf[1] == 'O') &&
							(g_sys_sts.uart_frm.RxUartData.databuf[2] == 'O') &&
							(g_sys_sts.uart_frm.RxUartData.databuf[3] == 'T') &&
							(g_sys_sts.uart_frm.RxUartData.databuf[4] == '?')) {
						mp_send_data_nolen("BOOT:OK");
						stateiap = 1;
						cntframe = 0;
						break;
					}
					else {

					}
				}
			case 1:                                      /* 接收总数据帧 	    	  */
			   if (g_sys_sts.uart_frm.bComProc) {
			   		g_sys_sts.uart_frm.bComProc = 0;
			   		if (modbus_action() == 1) {
						mp_send_data_nolen("OK\n");
						stateiap = 2;
						Totalframe = 
								((U16)g_sys_sts.uart_frm.RxUartData.databuf[3] << 8) +
								g_sys_sts.uart_frm.RxUartData.databuf[2];
			   		}					
			   }
			   break;
			case 2: 									/* 接收代码 		      */
				if (g_sys_sts.uart_frm.bComProc) {
					g_sys_sts.uart_frm.bComProc = 0;
					if (modbus_action() == 1) {
						if (cntframe == (((U16)g_sys_sts.uart_frm.RxUartData.databuf[3] << 8) +
								g_sys_sts.uart_frm.RxUartData.databuf[2]))  {
							mp_send_data_nolen("OK\n");	
							iap_write_appbin(&g_sys_sts.uart_frm.RxUartData.databuf[4],
									g_sys_sts.uart_frm.RxUartData.iLenght - 4);
							cntframe = ((U16)g_sys_sts.uart_frm.RxUartData.databuf[3] << 8) +
									g_sys_sts.uart_frm.RxUartData.databuf[2] + 1;
							if (cntframe == Totalframe) {
								g_sys_cfg.u16upgradeflag = 0;
                                stmflash_write(FLASH_ENV_ADDR,
		                                (U32*)&g_sys_cfg, 
		                                sizeof(SysCfg));/* 清除标志 */
								__set_PRIMASK(1);
								__set_FAULTMASK(1);
								NVIC_SystemReset();
							}
						}
					}
				}
				break;
			default:
				break;
		}
	}
}
	
/************************ (C) COPYRIGHT Hopewellgroup ***********END OF FILE***/

