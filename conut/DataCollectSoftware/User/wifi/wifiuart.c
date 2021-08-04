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
函数名称：delay_ms
功    能：延时 ms为单位
说    明：
参    数:time：延时的ms数
返    回: 
注    意：
*******************************************************************************/
void delay_ms(uint32_t time)
{
	uint32_t i = 8000 * 2 * time;
	while (i--)
		;
}

/*******************************************************************************
函数名称：WIFI_net_init
功    能：WIFI模块网络初始化
说    明：
参    数:
返    回: 
注    意：
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
函数名称：WIFI_net_init
功    能：WIFI模块串口初始化
说    明：
参    数:
返    回: 
注    意：
*******************************************************************************/
void WIFI_uart_init(void)
{
	int delay = 0;
	uint8_t ledstate = 0;  

	/* 此处的delay_ms没有采用osDelay的原因是因为初始化的行为在freertos开始调度之前
	而osDelay需要再调度之后生效
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
	/* Wi-Fi连接状态的灯没有连出来，此处只是超时模拟 Wi-Fi 提示灯，
	实际上可能没有用，即超时了也可能没有信号，此处超时属于经验值。
	*/
	LED2_ON;
}

/*******************************************************************************
函数名称：WIFI_uart_sendData
功    能：WIFI发送数据
说    明：
参    数:buf：要发送数据的首地址
		length：要发送数据的长度
返    回: 
注    意：
*******************************************************************************/
void WIFI_uart_sendData(char* buf,uint16_t length)
{
    HAL_UART_Transmit_DMA(&huart6,(uint8_t *)buf,length);
}

/*******************************************************************************
函数名称：WIFI_uart_receiveData
功    能：WIFI接收数据
说    明：
参    数:data：接收的数据（一个字节）
返    回: 
注    意：
*******************************************************************************/
void WIFI_uart_receiveData(uint8_t data)
{ 
	if (g_sys_sts.wifi_cfg_mgr.RxStsLength < UARTDATALENGTH-1) {
		g_sys_sts.wifi_cfg_mgr.rxbuf[g_sys_sts.wifi_cfg_mgr.RxStsLength++] = data;	
		g_sys_sts.wifi_cfg_mgr.RxState = 1;/* 数据正在接收 */
	}
	else {
		g_sys_sts.wifi_cfg_mgr.RxState = 2; /* 数据长度接收异常 */
	}
}

/*******************************************************************************
函数名称：WIFI_uart_receiveTimeout
功    能：WIFI接收数据（带超时功能）
说    明：
参    数:
返    回: 
注    意：
*******************************************************************************/
void  WIFI_uart_receiveTimeout()
{
	static uint16_t  count = 0;
	if (g_sys_sts.wifi_cfg_mgr.RxState != 1 && 
			g_sys_sts.wifi_cfg_mgr.RxStsLength != 0) { /* 数据接收完成 */
		if (g_sys_sts.wifi_cfg_mgr.bComProc != TRUE) {/* 前一包数据正在处理 */
			memcpy(g_sys_sts.wifi_cfg_mgr.JsonDatabuf,
					g_sys_sts.wifi_cfg_mgr.rxbuf,UARTDATALENGTH);
			g_sys_sts.wifi_cfg_mgr.JsonStsLength = 
					g_sys_sts.wifi_cfg_mgr.RxStsLength;
			memset(g_sys_sts.wifi_cfg_mgr.rxbuf,0,UARTDATALENGTH);
			g_sys_sts.wifi_cfg_mgr.RxStsLength = 0;
			g_sys_sts.wifi_cfg_mgr.bComProc = TRUE;
		}

	}
	else if(g_sys_sts.wifi_cfg_mgr.RxState == 2) {/* 本次接收数据超长 */
		count++;
		if (count > 1000) { /* 1S后才可重新开启接收 */
			count = 0;
			memset(g_sys_sts.wifi_cfg_mgr.rxbuf,0,UARTDATALENGTH);
			g_sys_sts.wifi_cfg_mgr.RxStsLength = 0;
			g_sys_sts.wifi_cfg_mgr.RxState = 0; /* 数据长度接收异常 */
		}
	}
	else {
		g_sys_sts.wifi_cfg_mgr.RxState = 0;
	}
	/* 时间延迟执行 */
	if (g_sys_sts.stop_mgr.mode == 1 && g_sys_sts.stop_mgr.delayTime > 0) { 
		g_sys_sts.stop_mgr.delayTime --;
		if (g_sys_sts.stop_mgr.delayTime == 0) {
			g_sys_sts.stop_mgr.delayisend = 1;
		}
	}
}
/* 操作成功上报 */
void WIFI_Uart_StopHandle(uint8_t state)
{
	if (state == 1) {
		Outcontrol_Stop(0);
	}
	else {
		Outcontrol_Stop(1);
	}
	/*  */
	/* 拷贝UUID */
	memcpy(g_sys_sts.stopCtrCmdBack.uuid, g_sys_cfg.dev_cfg.guid,
			sizeof(int8_t)*UUIDBUFLENGTH);
	/* 控制端的执行流水号 */
	memcpy(g_sys_sts.stopCtrCmdBack.operateid, 
			g_sys_sts.stopCtrCmd.operateid,sizeof(int8_t)*UUIDBUFLENGTH);
	/* 操作序列号 */
	g_sys_sts.stopCtrCmdBack.uindex ++;
	/* 执行操作时间 */
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
	/* 0操作成功，1操作失败 */
	g_sys_sts.stopCtrCmdBack.msgstatus = 1;
	/* 消息信息 */
	sprintf((char *)g_sys_sts.stopCtrCmdBack.msginfo,SUCCESS);	

	CommitStopData_Upload();
}

/*******************************************************************************
函数名称：WIFI_Uart_StopErrorHandle
功    能：WIFI接收数据（超时错误处理）
说    明：
参    数:
返    回: 
注    意：
*******************************************************************************/
void WIFI_Uart_StopErrorHandle()
{
	/* 拷贝UUID */
	memcpy(g_sys_sts.stopCtrCmdBack.uuid, g_sys_cfg.dev_cfg.guid,
			sizeof(int8_t)*UUIDBUFLENGTH);
	/* 控制端的执行流水号 */
	memcpy(g_sys_sts.stopCtrCmdBack.operateid, 
			g_sys_sts.stopCtrCmd.operateid,sizeof(int8_t)*UUIDBUFLENGTH);
	/* 操作序列号 */
	g_sys_sts.stopCtrCmdBack.uindex ++;
	/* 执行操作时间 */
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
	/* 0操作成功，1操作失败 */
	g_sys_sts.stopCtrCmdBack.msgstatus = 0;
	/* 消息信息 */
	/* memcpy((char *)g_sys_sts.stopCtrCmdBack.msginfo,msg,UUIDBUFLENGTH);	 */

	CommitStopData_Upload();
}

/*******************************************************************************
函数名称：WIFI_Uart_Handle
功    能：处理WIFi串口数据
说    明：
参    数:
返    回: 
注    意：
*******************************************************************************/
void WIFI_Uart_Handle(void)
{  
	if (g_sys_sts.wifi_cfg_mgr.bComProc == 1) {
		if (CJSON_to_StopCtrCmd((char *)g_sys_sts.wifi_cfg_mgr.JsonDatabuf,
				&g_sys_sts.stopCtrCmd)) {
			/* 判断是否为本机ID号 */
			if (strcmp((const char *)g_sys_sts.stopCtrCmd.uuid,
					(const char *)g_sys_cfg.dev_cfg.guid) == 0) {
			     if (g_sys_sts.stopCtrCmd.mode == 0) { /* 及时执行 */
					WIFI_Uart_StopHandle(g_sys_sts.stopCtrCmd.status);
					memset(&g_sys_sts.stopCtrCmd,0,sizeof(StopCtrCmd));   
				 }
			     else {
					g_sys_sts.stop_mgr.delayisend = 0;
					g_sys_sts.stop_mgr.mode = 1; /* 延时执行 */ 
					g_sys_sts.stop_mgr.state = g_sys_sts.stopCtrCmd.status; /* 状态 */
					g_sys_sts.stop_mgr.delayTime = 
							g_sys_sts.stopCtrCmd.operatetime; /* 延迟时间 */
				 }

			 }
			 else {
				sprintf((char *)g_sys_sts.stopCtrCmdBack.msginfo,UUIDERROR);
				WIFI_Uart_StopErrorHandle();  
			 }
		/* 处理停机信号 */
		}
		else {  /* 格式解析错误 */   
			sprintf((char *)g_sys_sts.stopCtrCmdBack.msginfo,DATAFORMATERROR);
			WIFI_Uart_StopErrorHandle();
		}					
		g_sys_sts.wifi_cfg_mgr.bComProc = 0;
	}
	if (g_sys_sts.stop_mgr.delayisend == 1) { /* 延时已经完成 */
		WIFI_Uart_StopHandle(g_sys_sts.stop_mgr.state);
		memset(&g_sys_sts.stopCtrCmd,0,sizeof(StopCtrCmd)); 
		g_sys_sts.stop_mgr.delayisend = 0;
	}
}

/************************ (C) COPYRIGHT Hopewellgroup ***********END OF FILE***/

