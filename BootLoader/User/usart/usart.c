/**
  ******************************************************************************
  * @file           : usart.c
  * @brief          : usart1 & uart4 driver
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
#include <stdio.h>
#include <string.h>
#include "usart.h"

/* define */
#define UARTSKEW       		(5*sizeof(U16))			  	/* 数据头到数据的偏移          */
#define LOCALID        		(0x0000)         		    /* 本机ID 		      */
#define PCID           		(0x0001)         			/* PCID 			  */
#define PC_ISSUE_UPGRADE  	(0x0004) 	 				/* 配置上传           	  */

/* extern */
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart4;
extern SysCfg g_sys_cfg;
extern SysSts g_sys_sts;

/* others */
#pragma import(__use_no_semihosting)                              
struct __FILE										  /* 标准库需要的支持函数 */ 
{ 
	int handle; 
}; 

FILE __stdout;  
 
void _sys_exit(int x)                   /* 定义_sys_exit()以避免使用半主机模式    	     */
{ 
	x = x; 
}	

/*******************************************************************************
函数名称：fputc
功    能： 重定义fputc函数
说    明：
参    数: ch:打印的字符
		 f:文件指针
返    回:需要打印的字符
注    意：
*******************************************************************************/
int fputc(int ch, FILE *f)							 
{	
	/* 循环发送,直到发送完毕 */
	while(HAL_UART_Transmit(&huart4, (U8 *)&ch, 1,0xFF)!= HAL_OK);
	   
	return ch;
}

/*******************************************************************************
函数名称：usart1_init
功    能： 使能USART1_DMA
说    明：
参    数:
返    回:
注    意：
*******************************************************************************/
void usart1_init(void)
{
	HAL_UART_Receive_DMA(&huart1,g_sys_sts.uart_frm.rxbuf,1);
}

/*******************************************************************************
函数名称：HAL_UART_RxCpltCallback
功    能： 串口数据读取
说    明：
参    数: huart:串口句柄
返    回: 
注    意：
*******************************************************************************/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (USART1 == huart->Instance) { /* 配置串口 */
		Config_PcDataAnaly(g_sys_sts.uart_frm.rxbuf[0]);    
		HAL_UART_Receive_DMA(&huart1,g_sys_sts.uart_frm.rxbuf,1);
	}
}

/*******************************************************************************
函数名称：Config_PcDataAnaly
功    能：从串口得到数据解析
说    明：
参    数: data:接受到的数据
返    回：
注    意：
*******************************************************************************/
static void Config_PcDataAnaly(U8 data)
{
    static U8 RecDataBuf[2];
    U32 TempComDat = 0;
    static U16 datacount = 0;
    RecDataBuf[0] = RecDataBuf[1];
    RecDataBuf[1] = data;
    TempComDat = data;
	
    if((g_sys_sts.uart_frm.RxSts != 5) && (RecDataBuf[0] == 0xEF) 
			&& (RecDataBuf[1] == 0xFE)) {
        g_sys_sts.uart_frm.RxSts = 0; /* 数据开始接收 */
        datacount = 0;    /* 数据计数长度 */
        memset(&g_sys_sts.uart_frm.RxUartData,0,sizeof(UartUpData));/* 数据清空 */
        g_sys_sts.uart_frm.RxUartData.header = UARTHEAD;		  
    }
	
    switch (g_sys_sts.uart_frm.RxSts) {
        case 0:     /*  接收开始 */
            g_sys_sts.uart_frm.RxSts = 1;
        	break;
			
        case 1:     /*  源ID号 */
            g_sys_sts.uart_frm.RxUartData.devSAID += TempComDat << (datacount * 8);
            datacount++;
            if(datacount >= sizeof(U16)) {
                datacount = 0;
                g_sys_sts.uart_frm.RxSts = 2;
            }	
        	break;
			
        case 2:     /*  目标ID号 */
            g_sys_sts.uart_frm.RxUartData.devDAID +=  TempComDat << (datacount * 8);
            datacount++;
            if(datacount >= sizeof(U16)) {
                datacount = 0;
                g_sys_sts.uart_frm.RxSts = 3;
            }	
        	break;
		
        case 3:     /*  接收命令 */
            g_sys_sts.uart_frm.RxUartData.commend +=  TempComDat << (datacount * 8);
            datacount++;
			
            if (datacount >= sizeof(U16)) {
                datacount = 0;
                g_sys_sts.uart_frm.RxSts = 4;
                g_sys_sts.uart_frm.RxUartData.iLenght = 0;
            }	
        	break;
			
        case 4:     /*  接收数据长度 */
            g_sys_sts.uart_frm.RxUartData.iLenght +=  TempComDat << (datacount * 8);
            datacount++;
			
            if (datacount >= sizeof(U16)) {
                datacount = 0;
				
                if (g_sys_sts.uart_frm.RxUartData.iLenght > 0) {
                    g_sys_sts.uart_frm.RxSts = 5;
                }

				else {
                    g_sys_sts.uart_frm.RxUartTail.tailer = 0; 
                    g_sys_sts.uart_frm.RxSts = 6;
                }
            }	
        	break;
			
        case 5:     /* 接收数据 */
            g_sys_sts.uart_frm.RxUartData.databuf[datacount++] = data;
			
            if (datacount >= g_sys_sts.uart_frm.RxUartData.iLenght) {
                datacount = 0;
                g_sys_sts.uart_frm.RxSts = 6;
                g_sys_sts.uart_frm.RxUartTail.tailer = 0;
            }	
        	break;
			
        case 6:     /*  接收帧尾 */
            g_sys_sts.uart_frm.RxUartTail.tailer += TempComDat << (datacount * 8);
            datacount++;
			
            if (datacount >= sizeof(U16)) {
                datacount = 0;
                g_sys_sts.uart_frm.RxSts = 255;
				
                if(g_sys_sts.uart_frm.RxUartTail.tailer == UARTTAIL) {  
                    g_sys_sts.uart_frm.bComProc = 1;  
                }
            }	
        	break;	
				
        default:
            g_sys_sts.uart_frm.RxSts = 255;
        	break;
    }
}

/*******************************************************************************
函数名称：Config_uart_sendData(uint8_t* buf,uint16_t length)
功    能：发送数据
说    明：
参    数: buf:发送的数据 length:发送的数据长度
返    回：
注    意：
*******************************************************************************/
static void Config_uart_sendData(U8* buf,U16 length)
{
    HAL_UART_Transmit_DMA(&huart1,buf,length);
}

/*******************************************************************************
函数名称：Config_uart_sendStructData
功    能：发送结构体数组
说    明：
参    数: UartUpData *RxUartData:包含数据头设备源ID命令及数据长度等
返    回：
注    意：
*******************************************************************************/
static void Config_uart_sendStructData(UartUpData *RxUartData)
{
    g_sys_sts.uart_frm.TxUarttail.tailer = UARTTAIL;
    memcpy((U8 *)(&RxUartData->databuf[RxUartData->iLenght]),
			(U8 *)&g_sys_sts.uart_frm.TxUarttail.tailer,
			sizeof(UartUpDatatail));
    Config_uart_sendData((U8 *)RxUartData,RxUartData->iLenght + 
			UARTSKEW+sizeof(UartUpDatatail));	  
}

/*******************************************************************************
函数名称：mp_send_data_nolen
功    能：串口1发送数据
说    明： 
参    数:buf：数据指针
        cnts:数据长度，没有使用
返    回:
注    意：
*******************************************************************************/
void mp_send_data_nolen(U8* buf)		
{
	g_sys_sts.uart_frm.TxUartData.header = UARTHEAD;
	g_sys_sts.uart_frm.TxUartData.devDAID = 0;
	g_sys_sts.uart_frm.TxUartData.devSAID = LOCALID;
	g_sys_sts.uart_frm.TxUartData.commend = PC_ISSUE_UPGRADE;
	g_sys_sts.uart_frm.TxUartData.iLenght = strlen((const char *)buf);

	memcpy(g_sys_sts.uart_frm.TxUartData.databuf,buf,
			g_sys_sts.uart_frm.TxUartData.iLenght);
	Config_uart_sendStructData(&g_sys_sts.uart_frm.TxUartData);
}

/************************ (C) COPYRIGHT Hopewellgroup ***********END OF FILE***/

