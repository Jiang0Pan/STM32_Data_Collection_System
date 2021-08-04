/**
  ******************************************************************************
  * @file           : outsidedata.c
  * @brief          : upload data control
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
#include "stdio.h"

char JsonDataBuf[JSONLENGTH];

/*******************************************************************************
函数名称：CommitCollectData_to_cJSON
功    能：采集数据转换为JSON数据格式
说    明：
参    数:cJsonRoom:cJSON数据缓冲区
        collectdata：采集数据
        datatype：数据类型
返    回: cJSON的字符串
注    意：
*******************************************************************************/
char * CommitCollectData_to_cJSON( char * cJsonRoom, 
		CommitCollectData collectdata,uint8_t datatype)
{
	char *result;
	cJSON *root,*item;

	root = cJSON_CreateObject();
	if (datatype == 0) {
		cJSON_AddItemToObject(root, COLLECTDATA, item=cJSON_CreateObject()); 
	}/* 采集器数据类型 */
	else if (datatype == 1) {
		cJSON_AddItemToObject(root, COLLECTCLEAR, 
				item=cJSON_CreateObject()); 
	}/* 采集器数据类型 */
	else {

	};

	/* 采集设备标识 */
	cJSON_AddStringToObject(item,MACHUUID,(const char*)collectdata.guid); 
	/* 设备GUID */
	cJSON_AddNumberToObject(item,UPINDEX,collectdata.dataindex); 
	/* 上传数据的INDEX */

	/* 设备时间统计 */
	cJSON_AddNumberToObject(item,MACHATIME,collectdata.alltime); 
	/* 设备总运行时间              */
	cJSON_AddNumberToObject(item,MACHBTIME,collectdata.boottime); 
	/* 设备正常运行时间 */
	cJSON_AddNumberToObject(item,MACHSTIME,collectdata.stoptime); 
	/* 设备停机时间 */
	cJSON_AddNumberToObject(item,MACHFTIME,collectdata.faulttime); 
	/* 设备故障时间 */

	/* 设备当前状态 */
	cJSON_AddNumberToObject(item,MACHSTA,collectdata.bootstopsig);
	cJSON_AddNumberToObject(item,MACHFAU,collectdata.faultsig);      
	cJSON_AddNumberToObject(item,MACHCOUNT,collectdata.pulsecount);
		
	/* 实时时间 */
	cJSON_AddStringToObject(item,MACHTIMEB,
			(const char*)(collectdata.timebegin));     
	cJSON_AddStringToObject(item,MACHTIMEC,
			(const char*)(collectdata.timecurrent));   			

	result = cJSON_Print(root);
	strcpy(cJsonRoom,(const char*)result);
		
	cJSON_Delete(root);
	myfree(result);

    /* 使用全局数组 */
	return cJsonRoom;
}

/*******************************************************************************
函数名称： CommitBladeData_to_cJSON
功    能：刀版数据转换为JSON数据格式
说    明：
参    数:cJsonRoom:cJSON数据缓冲区
        collectdata：采集数据
返    回: cJSON的字符串
注    意：
*******************************************************************************/
char * CommitBladeData_to_cJSON( char * cJsonRoom, CommitCollectData collectdata)
{
	char *result;
	cJSON *root,*item;

	root = cJSON_CreateObject();
	cJSON_AddItemToObject(root, BLADEDATA, item = cJSON_CreateObject()); 
	/* 采集器数据类型 */

	/* 采集设备标识 */
	cJSON_AddStringToObject(item,MACHUUID,(const char*)collectdata.guid); 
	/* 设备GUID */
	cJSON_AddNumberToObject(item,UPINDEX,collectdata.dataindex); 
	/* 上传数据的INDEX */
	
	/* 刀版脉冲 */
	cJSON_AddNumberToObject(item,MACHCOUNT,collectdata.bladepulsecount);
	/* 实时时间 */
	cJSON_AddStringToObject(item,MACHTIMEB,(const char*)(collectdata.timebegin));     
	cJSON_AddStringToObject(item,MACHTIMEC,(const char*)(collectdata.timecurrent));   			

	result=cJSON_Print(root);
	strcpy(cJsonRoom,(const char*)result);

	cJSON_Delete(root);
	myfree(result);

	/* 使用全局数组 */
	return cJsonRoom;
}

/*******************************************************************************
函数名称： CommitStopData_to_cJSON
功    能：封装上报停机信号
说    明：
参    数:cJsonRoom:cJSON数据缓冲区
        stopdata：停机数据
返    回: cJSON的字符串
注    意：
*******************************************************************************/
char * CommitStopData_to_cJSON( char * cJsonRoom, StopCtrCmdBack stopdata)
{
	char *result;
	cJSON *root,*item;

	root = cJSON_CreateObject();
	cJSON_AddItemToObject(root, CONMACHRS, item=cJSON_CreateObject()); 
	/* 采集器数据类型 */

	cJSON_AddStringToObject(item,UUID,(const char*)stopdata.uuid); 
	/* 设备GUID */
	cJSON_AddStringToObject(item,OPERATEID,(const char*)stopdata.operateid); 
	/* 停机操作ID号 */
	cJSON_AddNumberToObject(item,UINDEX,stopdata.uindex); 
	/* 操作序列号 */

	cJSON_AddStringToObject(item,OPTIME,(const char*)(stopdata.optime));
	/* 操作时间 */
	cJSON_AddNumberToObject(item,MSGSTATUS,stopdata.msgstatus);   
	/* 信号状态 */
	cJSON_AddStringToObject	(item,MSG,(const char*)(stopdata.msginfo));
	/* 操作时间 */

	result = cJSON_Print(root);
	strcpy(cJsonRoom,(const char*)result);
		
	cJSON_Delete(root);
	myfree(result);

    /* 使用全局数组 */
	return cJsonRoom;
}

/*******************************************************************************
函数名称：CJSON_to_StopCtrCmd
功    能：解析JSON数据(下发停机配置数据)
说    明：
参    数:json_string:cJSON数据缓冲区
        stopctr：解析后的停机命令
返    回: 返回值:停机命令
注    意：
*******************************************************************************/
uint8_t  CJSON_to_StopCtrCmd( char * json_string,StopCtrCmd * stopctr)
{
	cJSON *item;
	cJSON *root = cJSON_Parse(json_string);  /* 获取JSON端点 */

	if (root == NULL) {
		return 0; /* JSON解析失败 */
	}
	cJSON *object = cJSON_GetObjectItem(root,CONMACH); 
	if (object == NULL) {
		cJSON_Delete(root);
		return 0; /* JSON解析失败 */
	}

	item = cJSON_GetObjectItem(object,UUID); /* 取UUID */
	if (item == NULL) {
		cJSON_Delete(root);
		return 0; /* JSON解析失败 */
	}
	memcpy(stopctr->uuid,item->valuestring,strlen(item->valuestring));

	item = cJSON_GetObjectItem(object,STATUS); /* 取状态 */
	if (item == NULL) {
		cJSON_Delete(root);
		return 0; /* JSON解析失败 */
	}
	stopctr->status = item->valueint;

	item = cJSON_GetObjectItem(object,MODE); /* 取模式 */
	if (item == NULL) {
		cJSON_Delete(root);
		return 0; /* JSON解析失败 */
	}
	stopctr->mode=item->valueint;

	item = cJSON_GetObjectItem(object,OPERATEID); /* 取执行流水号 */
	if (item == NULL) {
		cJSON_Delete(root);
		return 0; /* JSON解析失败 */
	}
	memcpy(stopctr->operateid,item->valuestring,strlen(item->valuestring));

	item = cJSON_GetObjectItem(object,OPERATETIME); /* 接收到信号后延迟时间 */
	if (item == NULL) {
		cJSON_Delete(root);
		return 0; /* JSON解析失败 */
	}
	stopctr->operatetime=item->valueint;

	cJSON_Delete(root);

	return 1;
	  
}
/* 解析JSON数据(下发配置ID) */
/* 解析JSON数据(同步时间) */

/*******************************************************************************
函数名称：CommitCollectData_Upload
功    能：采集数据打包上传
说    明：
参    数:
返    回: 
注    意：
*******************************************************************************/
void CommitCollectData_Upload(void)
{
	/* WIFI数据发送 */
	memset(JsonDataBuf,0,JSONLENGTH);
	CommitCollectData_to_cJSON(JsonDataBuf,g_sys_sts.commit_data,0);
	WIFI_uart_sendData(JsonDataBuf,strlen(JsonDataBuf));
	/* 串口数据发送       */
}

/*******************************************************************************
函数名称：CommitBladeData_Upload
功    能：刀版数据打包上传
说    明：
参    数:
返    回: 
注    意：
*******************************************************************************/
void CommitBladeData_Upload(void)
{
	/* WIFI数据发送 */
	memset(JsonDataBuf,0,JSONLENGTH);
	CommitBladeData_to_cJSON(JsonDataBuf,g_sys_sts.commit_data);
	WIFI_uart_sendData(JsonDataBuf,strlen(JsonDataBuf));
	/* Config_uart_sendData(JsonDataBuf,strlen(JsonDataBuf)); */
	/* 串口数据发送       */
}

/*******************************************************************************
函数名称：CommitStopData_Upload
功    能：停机数据打包上传
说    明：
参    数:
返    回: 
注    意：
*******************************************************************************/
void CommitStopData_Upload(void)
{  
	/* WIFI数据发送 */
	memset(JsonDataBuf,0,JSONLENGTH);
	CommitStopData_to_cJSON(JsonDataBuf,g_sys_sts.stopCtrCmdBack);
	WIFI_uart_sendData(JsonDataBuf,strlen(JsonDataBuf));
}

/*******************************************************************************
函数名称：CommitClearData_Upload
功    能：清零数据打包上传
说    明：
参    数:
返    回: 
注    意：
*******************************************************************************/
void CommitClearData_Upload(void)
{  
	/* WIFI数据发送 */
	memset(JsonDataBuf,0,JSONLENGTH);
	CommitCollectData_to_cJSON(JsonDataBuf,g_sys_sts.commit_data,1);
	WIFI_uart_sendData(JsonDataBuf,strlen(JsonDataBuf));
}

/************************ (C) COPYRIGHT Hopewellgroup ***********END OF FILE***/

