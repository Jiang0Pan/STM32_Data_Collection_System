/**
  ******************************************************************************
  * @file           : wifiuart.c
  * @brief          : wifi config
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

extern UART_HandleTypeDef huart6;
extern DMA_HandleTypeDef hdma_usart6_tx;

/*******************************************************************************
�������ƣ�delay_ms
��    �ܣ���ʱ msΪ��λ
˵    ����
��    ��:time����ʱ��ms��
��    ��: 
ע    �⣺
*******************************************************************************/
void delay_ms(uint32_t time)
{
	uint32_t i = 8000 * 2 * time;
	while (i--)
		;
}

/*******************************************************************************
�������ƣ�WIFI_net_init
��    �ܣ�WIFIģ�������ʼ��
˵    ����
��    ��:
��    ��: 
ע    �⣺
*******************************************************************************/
void WIFI_net_init(void) 
{
	memcpy(g_sys_cfg.net_cfg.mode,WIFI_MODE,strlen(WIFI_MODE));
	memcpy(g_sys_cfg.net_cfg.name,WIFI_DEVNAME,strlen(WIFI_DEVNAME));
	memcpy(g_sys_cfg.net_cfg.wifiname,WIFI_NAME,strlen(WIFI_NAME));
	memcpy(g_sys_cfg.net_cfg.wifi_pwd,WIFI_PASSWORD,strlen(WIFI_PASSWORD));
	memcpy(g_sys_cfg.net_cfg.ser_ip,SERVER_IP,strlen(SERVER_IP));
	memcpy(g_sys_cfg.net_cfg.ser_port,SERVER_PORT,strlen(SERVER_PORT));
	memcpy(g_sys_cfg.net_cfg.ip,LOCAL_IP,strlen(LOCAL_IP));
	memcpy(g_sys_cfg.net_cfg.subnet_mask,LOCAL_MASK,strlen(LOCAL_MASK));
	memcpy(g_sys_cfg.net_cfg.gateway,LOCAL_GATEWAY,strlen(LOCAL_GATEWAY));
}

/*******************************************************************************
�������ƣ�WIFI_net_init
��    �ܣ�WIFIģ�鴮�ڳ�ʼ��
˵    ����
��    ��:
��    ��: 
ע    �⣺
*******************************************************************************/
void WIFI_uart_init(void)
{
	int delay = 0;
	uint8_t ledstate = 0;  

	/* �˴���delay_msû�в���osDelay��ԭ������Ϊ��ʼ������Ϊ��freertos��ʼ����֮ǰ
	��osDelay��Ҫ�ٵ���֮����Ч
	*/    
	WIFI_RESET_ON;
	delay_ms(WIFIRESETTIME);
	WIFI_RESET_OFF;
	delay_ms(WIFIRESETTIME/2);
	for (delay = WIFIWAITCONNECT; delay > 0; delay --) {
		delay_ms(500);

		if (ledstate == 0) {
			LED2_ON;
			ledstate = 1;
		}
		else {
			LED2_OFF;
			ledstate = 0;
		}
	}
	/* Wi-Fi����״̬�ĵ�û�����������˴�ֻ�ǳ�ʱģ�� Wi-Fi ��ʾ�ƣ�
	ʵ���Ͽ���û���ã�����ʱ��Ҳ����û���źţ��˴���ʱ���ھ���ֵ��
	*/
	LED2_ON;
}

/*******************************************************************************
�������ƣ�WIFI_uart_sendData
��    �ܣ�WIFI��������
˵    ����
��    ��:buf��Ҫ�������ݵ��׵�ַ
		length��Ҫ�������ݵĳ���
��    ��: 
ע    �⣺
*******************************************************************************/
void WIFI_uart_sendData(char* buf,uint16_t length)
{
    HAL_UART_Transmit_DMA(&huart6,(uint8_t *)buf,length);
}

/*******************************************************************************
�������ƣ�WIFI_uart_receiveData
��    �ܣ�WIFI��������
˵    ����
��    ��:data�����յ����ݣ�һ���ֽڣ�
��    ��: 
ע    �⣺
*******************************************************************************/
void WIFI_uart_receiveData(uint8_t data)
{ 
	if (g_sys_sts.wifi_cfg_mgr.RxStsLength < UARTDATALENGTH-1) {
		g_sys_sts.wifi_cfg_mgr.rxbuf[g_sys_sts.wifi_cfg_mgr.RxStsLength++] = data;	
		g_sys_sts.wifi_cfg_mgr.RxState = 1;/* �������ڽ��� */
	}
	else {
		g_sys_sts.wifi_cfg_mgr.RxState = 2; /* ���ݳ��Ƚ����쳣 */
	}
}

/*******************************************************************************
�������ƣ�WIFI_uart_receiveTimeout
��    �ܣ�WIFI�������ݣ�����ʱ���ܣ�
˵    ����
��    ��:
��    ��: 
ע    �⣺
*******************************************************************************/
void  WIFI_uart_receiveTimeout()
{
	static uint16_t  count = 0;
	if (g_sys_sts.wifi_cfg_mgr.RxState != 1 && 
			g_sys_sts.wifi_cfg_mgr.RxStsLength != 0) { /* ���ݽ������ */
		if (g_sys_sts.wifi_cfg_mgr.bComProc != TRUE) {/* ǰһ���������ڴ��� */
			memcpy(g_sys_sts.wifi_cfg_mgr.JsonDatabuf,
					g_sys_sts.wifi_cfg_mgr.rxbuf,UARTDATALENGTH);
			g_sys_sts.wifi_cfg_mgr.JsonStsLength = 
					g_sys_sts.wifi_cfg_mgr.RxStsLength;
			memset(g_sys_sts.wifi_cfg_mgr.rxbuf,0,UARTDATALENGTH);
			g_sys_sts.wifi_cfg_mgr.RxStsLength = 0;
			g_sys_sts.wifi_cfg_mgr.bComProc = TRUE;
		}

	}
	else if(g_sys_sts.wifi_cfg_mgr.RxState == 2) {/* ���ν������ݳ��� */
		count++;
		if (count > 1000) { /* 1S��ſ����¿������� */
			count = 0;
			memset(g_sys_sts.wifi_cfg_mgr.rxbuf,0,UARTDATALENGTH);
			g_sys_sts.wifi_cfg_mgr.RxStsLength = 0;
			g_sys_sts.wifi_cfg_mgr.RxState = 0; /* ���ݳ��Ƚ����쳣 */
		}
	}
	else {
		g_sys_sts.wifi_cfg_mgr.RxState = 0;
	}
	/* ʱ���ӳ�ִ�� */
	if (g_sys_sts.stop_mgr.mode == 1 && g_sys_sts.stop_mgr.delayTime > 0) { 
		g_sys_sts.stop_mgr.delayTime --;
		if (g_sys_sts.stop_mgr.delayTime == 0) {
			g_sys_sts.stop_mgr.delayisend = 1;
		}
	}
}
/* �����ɹ��ϱ� */
void WIFI_Uart_StopHandle(uint8_t state)
{
	if (state == 1) {
		Outcontrol_Stop(0);
	}
	else {
		Outcontrol_Stop(1);
	}
	/*  */
	/* ����UUID */
	memcpy(g_sys_sts.stopCtrCmdBack.uuid, g_sys_cfg.dev_cfg.guid,
			sizeof(int8_t)*UUIDBUFLENGTH);
	/* ���ƶ˵�ִ����ˮ�� */
	memcpy(g_sys_sts.stopCtrCmdBack.operateid, 
			g_sys_sts.stopCtrCmd.operateid,sizeof(int8_t)*UUIDBUFLENGTH);
	/* �������к� */
	g_sys_sts.stopCtrCmdBack.uindex ++;
	/* ִ�в���ʱ�� */
	HAL_RTC_GetDate(&hrtc, &g_sys_sts.col_info.cur_tm.sdate, RTC_FORMAT_BIN);
	delay_ms(1);
	HAL_RTC_GetTime(&hrtc, &g_sys_sts.col_info.cur_tm.stime, RTC_FORMAT_BIN);
	sprintf((char*)g_sys_sts.stopCtrCmdBack.optime,
			"20%02d-%02d-%02d %02d:%02d:%02d",
			g_sys_sts.col_info.cur_tm.sdate.Year,
			g_sys_sts.col_info.cur_tm.sdate.Month,
			g_sys_sts.col_info.cur_tm.sdate.Date,
			g_sys_sts.col_info.cur_tm.stime.Hours,
			g_sys_sts.col_info.cur_tm.stime.Minutes,
			g_sys_sts.col_info.cur_tm.stime.Seconds);
	/* 0�����ɹ���1����ʧ�� */
	g_sys_sts.stopCtrCmdBack.msgstatus = 1;
	/* ��Ϣ��Ϣ */
	sprintf((char *)g_sys_sts.stopCtrCmdBack.msginfo,SUCCESS);	

	CommitStopData_Upload();
}

/*******************************************************************************
�������ƣ�WIFI_Uart_StopErrorHandle
��    �ܣ�WIFI�������ݣ���ʱ������
˵    ����
��    ��:
��    ��: 
ע    �⣺
*******************************************************************************/
void WIFI_Uart_StopErrorHandle()
{
	/* ����UUID */
	memcpy(g_sys_sts.stopCtrCmdBack.uuid, g_sys_cfg.dev_cfg.guid,
			sizeof(int8_t)*UUIDBUFLENGTH);
	/* ���ƶ˵�ִ����ˮ�� */
	memcpy(g_sys_sts.stopCtrCmdBack.operateid, 
			g_sys_sts.stopCtrCmd.operateid,sizeof(int8_t)*UUIDBUFLENGTH);
	/* �������к� */
	g_sys_sts.stopCtrCmdBack.uindex ++;
	/* ִ�в���ʱ�� */
	HAL_RTC_GetDate(&hrtc, &g_sys_sts.col_info.cur_tm.sdate, RTC_FORMAT_BIN);
	delay_ms(1);
	HAL_RTC_GetTime(&hrtc, &g_sys_sts.col_info.cur_tm.stime, RTC_FORMAT_BIN);
	sprintf((char*)g_sys_sts.stopCtrCmdBack.optime,
			"20%02d-%02d-%02d %02d:%02d:%02d",
			g_sys_sts.col_info.cur_tm.sdate.Year,
			g_sys_sts.col_info.cur_tm.sdate.Month,
			g_sys_sts.col_info.cur_tm.sdate.Date,
			g_sys_sts.col_info.cur_tm.stime.Hours,
			g_sys_sts.col_info.cur_tm.stime.Minutes,
			g_sys_sts.col_info.cur_tm.stime.Seconds);
	/* 0�����ɹ���1����ʧ�� */
	g_sys_sts.stopCtrCmdBack.msgstatus = 0;
	/* ��Ϣ��Ϣ */
	/* memcpy((char *)g_sys_sts.stopCtrCmdBack.msginfo,msg,UUIDBUFLENGTH);	 */

	CommitStopData_Upload();
}

/*******************************************************************************
�������ƣ�WIFI_Uart_Handle
��    �ܣ�����WIFi��������
˵    ����
��    ��:
��    ��: 
ע    �⣺
*******************************************************************************/
void WIFI_Uart_Handle(void)
{  
	if (g_sys_sts.wifi_cfg_mgr.bComProc == 1) {
		if (CJSON_to_StopCtrCmd((char *)g_sys_sts.wifi_cfg_mgr.JsonDatabuf,
				&g_sys_sts.stopCtrCmd)) {
			/* �ж��Ƿ�Ϊ����ID�� */
			if (strcmp((const char *)g_sys_sts.stopCtrCmd.uuid,
					(const char *)g_sys_cfg.dev_cfg.guid) == 0) {
			     if (g_sys_sts.stopCtrCmd.mode == 0) { /* ��ʱִ�� */
					WIFI_Uart_StopHandle(g_sys_sts.stopCtrCmd.status);
					memset(&g_sys_sts.stopCtrCmd,0,sizeof(StopCtrCmd));   
				 }
			     else {
					g_sys_sts.stop_mgr.delayisend = 0;
					g_sys_sts.stop_mgr.mode = 1; /* ��ʱִ�� */ 
					g_sys_sts.stop_mgr.state = g_sys_sts.stopCtrCmd.status; /* ״̬ */
					g_sys_sts.stop_mgr.delayTime = 
							g_sys_sts.stopCtrCmd.operatetime; /* �ӳ�ʱ�� */
				 }

			 }
			 else {
				sprintf((char *)g_sys_sts.stopCtrCmdBack.msginfo,UUIDERROR);
				WIFI_Uart_StopErrorHandle();  
			 }
		/* ����ͣ���ź� */
		}
		else {  /* ��ʽ�������� */   
			sprintf((char *)g_sys_sts.stopCtrCmdBack.msginfo,DATAFORMATERROR);
			WIFI_Uart_StopErrorHandle();
		}					
		g_sys_sts.wifi_cfg_mgr.bComProc = 0;
	}
	if (g_sys_sts.stop_mgr.delayisend == 1) { /* ��ʱ�Ѿ���� */
		WIFI_Uart_StopHandle(g_sys_sts.stop_mgr.state);
		memset(&g_sys_sts.stopCtrCmd,0,sizeof(StopCtrCmd)); 
		g_sys_sts.stop_mgr.delayisend = 0;
	}
}

/************************ (C) COPYRIGHT Hopewellgroup ***********END OF FILE***/

