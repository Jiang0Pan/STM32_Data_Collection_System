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
#define UARTSKEW       		(5*sizeof(U16))			  	/* ����ͷ�����ݵ�ƫ��          */
#define LOCALID        		(0x0000)         		    /* ����ID 		      */
#define PCID           		(0x0001)         			/* PCID 			  */
#define PC_ISSUE_UPGRADE  	(0x0004) 	 				/* �����ϴ�           	  */

/* extern */
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart4;
extern SysCfg g_sys_cfg;
extern SysSts g_sys_sts;

/* others */
#pragma import(__use_no_semihosting)                              
struct __FILE										  /* ��׼����Ҫ��֧�ֺ��� */ 
{ 
	int handle; 
}; 

FILE __stdout;  
 
void _sys_exit(int x)                   /* ����_sys_exit()�Ա���ʹ�ð�����ģʽ    	     */
{ 
	x = x; 
}	

/*******************************************************************************
�������ƣ�fputc
��    �ܣ� �ض���fputc����
˵    ����
��    ��: ch:��ӡ���ַ�
		 f:�ļ�ָ��
��    ��:��Ҫ��ӡ���ַ�
ע    �⣺
*******************************************************************************/
int fputc(int ch, FILE *f)							 
{	
	/* ѭ������,ֱ��������� */
	while(HAL_UART_Transmit(&huart4, (U8 *)&ch, 1,0xFF)!= HAL_OK);
	   
	return ch;
}

/*******************************************************************************
�������ƣ�usart1_init
��    �ܣ� ʹ��USART1_DMA
˵    ����
��    ��:
��    ��:
ע    �⣺
*******************************************************************************/
void usart1_init(void)
{
	HAL_UART_Receive_DMA(&huart1,g_sys_sts.uart_frm.rxbuf,1);
}

/*******************************************************************************
�������ƣ�HAL_UART_RxCpltCallback
��    �ܣ� �������ݶ�ȡ
˵    ����
��    ��: huart:���ھ��
��    ��: 
ע    �⣺
*******************************************************************************/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (USART1 == huart->Instance) { /* ���ô��� */
		Config_PcDataAnaly(g_sys_sts.uart_frm.rxbuf[0]);    
		HAL_UART_Receive_DMA(&huart1,g_sys_sts.uart_frm.rxbuf,1);
	}
}

/*******************************************************************************
�������ƣ�Config_PcDataAnaly
��    �ܣ��Ӵ��ڵõ����ݽ���
˵    ����
��    ��: data:���ܵ�������
��    �أ�
ע    �⣺
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
        g_sys_sts.uart_frm.RxSts = 0; /* ���ݿ�ʼ���� */
        datacount = 0;    /* ���ݼ������� */
        memset(&g_sys_sts.uart_frm.RxUartData,0,sizeof(UartUpData));/* ������� */
        g_sys_sts.uart_frm.RxUartData.header = UARTHEAD;		  
    }
	
    switch (g_sys_sts.uart_frm.RxSts) {
        case 0:     /*  ���տ�ʼ */
            g_sys_sts.uart_frm.RxSts = 1;
        	break;
			
        case 1:     /*  ԴID�� */
            g_sys_sts.uart_frm.RxUartData.devSAID += TempComDat << (datacount * 8);
            datacount++;
            if(datacount >= sizeof(U16)) {
                datacount = 0;
                g_sys_sts.uart_frm.RxSts = 2;
            }	
        	break;
			
        case 2:     /*  Ŀ��ID�� */
            g_sys_sts.uart_frm.RxUartData.devDAID +=  TempComDat << (datacount * 8);
            datacount++;
            if(datacount >= sizeof(U16)) {
                datacount = 0;
                g_sys_sts.uart_frm.RxSts = 3;
            }	
        	break;
		
        case 3:     /*  �������� */
            g_sys_sts.uart_frm.RxUartData.commend +=  TempComDat << (datacount * 8);
            datacount++;
			
            if (datacount >= sizeof(U16)) {
                datacount = 0;
                g_sys_sts.uart_frm.RxSts = 4;
                g_sys_sts.uart_frm.RxUartData.iLenght = 0;
            }	
        	break;
			
        case 4:     /*  �������ݳ��� */
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
			
        case 5:     /* �������� */
            g_sys_sts.uart_frm.RxUartData.databuf[datacount++] = data;
			
            if (datacount >= g_sys_sts.uart_frm.RxUartData.iLenght) {
                datacount = 0;
                g_sys_sts.uart_frm.RxSts = 6;
                g_sys_sts.uart_frm.RxUartTail.tailer = 0;
            }	
        	break;
			
        case 6:     /*  ����֡β */
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
�������ƣ�Config_uart_sendData(uint8_t* buf,uint16_t length)
��    �ܣ���������
˵    ����
��    ��: buf:���͵����� length:���͵����ݳ���
��    �أ�
ע    �⣺
*******************************************************************************/
static void Config_uart_sendData(U8* buf,U16 length)
{
    HAL_UART_Transmit_DMA(&huart1,buf,length);
}

/*******************************************************************************
�������ƣ�Config_uart_sendStructData
��    �ܣ����ͽṹ������
˵    ����
��    ��: UartUpData *RxUartData:��������ͷ�豸ԴID������ݳ��ȵ�
��    �أ�
ע    �⣺
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
�������ƣ�mp_send_data_nolen
��    �ܣ�����1��������
˵    ���� 
��    ��:buf������ָ��
        cnts:���ݳ��ȣ�û��ʹ��
��    ��:
ע    �⣺
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

