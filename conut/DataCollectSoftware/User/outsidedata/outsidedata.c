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
�������ƣ�CommitCollectData_to_cJSON
��    �ܣ��ɼ�����ת��ΪJSON���ݸ�ʽ
˵    ����
��    ��:cJsonRoom:cJSON���ݻ�����
        collectdata���ɼ�����
        datatype����������
��    ��: cJSON���ַ���
ע    �⣺
*******************************************************************************/
char * CommitCollectData_to_cJSON( char * cJsonRoom, 
		CommitCollectData collectdata,uint8_t datatype)
{
	char *result;
	cJSON *root,*item;

	root = cJSON_CreateObject();
	if (datatype == 0) {
		cJSON_AddItemToObject(root, COLLECTDATA, item=cJSON_CreateObject()); 
	}/* �ɼ����������� */
	else if (datatype == 1) {
		cJSON_AddItemToObject(root, COLLECTCLEAR, 
				item=cJSON_CreateObject()); 
	}/* �ɼ����������� */
	else {

	};

	/* �ɼ��豸��ʶ */
	cJSON_AddStringToObject(item,MACHUUID,(const char*)collectdata.guid); 
	/* �豸GUID */
	cJSON_AddNumberToObject(item,UPINDEX,collectdata.dataindex); 
	/* �ϴ����ݵ�INDEX */

	/* �豸ʱ��ͳ�� */
	cJSON_AddNumberToObject(item,MACHATIME,collectdata.alltime); 
	/* �豸������ʱ��              */
	cJSON_AddNumberToObject(item,MACHBTIME,collectdata.boottime); 
	/* �豸��������ʱ�� */
	cJSON_AddNumberToObject(item,MACHSTIME,collectdata.stoptime); 
	/* �豸ͣ��ʱ�� */
	cJSON_AddNumberToObject(item,MACHFTIME,collectdata.faulttime); 
	/* �豸����ʱ�� */

	/* �豸��ǰ״̬ */
	cJSON_AddNumberToObject(item,MACHSTA,collectdata.bootstopsig);
	cJSON_AddNumberToObject(item,MACHFAU,collectdata.faultsig);      
	cJSON_AddNumberToObject(item,MACHCOUNT,collectdata.pulsecount);
		
	/* ʵʱʱ�� */
	cJSON_AddStringToObject(item,MACHTIMEB,
			(const char*)(collectdata.timebegin));     
	cJSON_AddStringToObject(item,MACHTIMEC,
			(const char*)(collectdata.timecurrent));   			

	result = cJSON_Print(root);
	strcpy(cJsonRoom,(const char*)result);
		
	cJSON_Delete(root);
	myfree(result);

    /* ʹ��ȫ������ */
	return cJsonRoom;
}

/*******************************************************************************
�������ƣ� CommitBladeData_to_cJSON
��    �ܣ���������ת��ΪJSON���ݸ�ʽ
˵    ����
��    ��:cJsonRoom:cJSON���ݻ�����
        collectdata���ɼ�����
��    ��: cJSON���ַ���
ע    �⣺
*******************************************************************************/
char * CommitBladeData_to_cJSON( char * cJsonRoom, CommitCollectData collectdata)
{
	char *result;
	cJSON *root,*item;

	root = cJSON_CreateObject();
	cJSON_AddItemToObject(root, BLADEDATA, item = cJSON_CreateObject()); 
	/* �ɼ����������� */

	/* �ɼ��豸��ʶ */
	cJSON_AddStringToObject(item,MACHUUID,(const char*)collectdata.guid); 
	/* �豸GUID */
	cJSON_AddNumberToObject(item,UPINDEX,collectdata.dataindex); 
	/* �ϴ����ݵ�INDEX */
	
	/* �������� */
	cJSON_AddNumberToObject(item,MACHCOUNT,collectdata.bladepulsecount);
	/* ʵʱʱ�� */
	cJSON_AddStringToObject(item,MACHTIMEB,(const char*)(collectdata.timebegin));     
	cJSON_AddStringToObject(item,MACHTIMEC,(const char*)(collectdata.timecurrent));   			

	result=cJSON_Print(root);
	strcpy(cJsonRoom,(const char*)result);

	cJSON_Delete(root);
	myfree(result);

	/* ʹ��ȫ������ */
	return cJsonRoom;
}

/*******************************************************************************
�������ƣ� CommitStopData_to_cJSON
��    �ܣ���װ�ϱ�ͣ���ź�
˵    ����
��    ��:cJsonRoom:cJSON���ݻ�����
        stopdata��ͣ������
��    ��: cJSON���ַ���
ע    �⣺
*******************************************************************************/
char * CommitStopData_to_cJSON( char * cJsonRoom, StopCtrCmdBack stopdata)
{
	char *result;
	cJSON *root,*item;

	root = cJSON_CreateObject();
	cJSON_AddItemToObject(root, CONMACHRS, item=cJSON_CreateObject()); 
	/* �ɼ����������� */

	cJSON_AddStringToObject(item,UUID,(const char*)stopdata.uuid); 
	/* �豸GUID */
	cJSON_AddStringToObject(item,OPERATEID,(const char*)stopdata.operateid); 
	/* ͣ������ID�� */
	cJSON_AddNumberToObject(item,UINDEX,stopdata.uindex); 
	/* �������к� */

	cJSON_AddStringToObject(item,OPTIME,(const char*)(stopdata.optime));
	/* ����ʱ�� */
	cJSON_AddNumberToObject(item,MSGSTATUS,stopdata.msgstatus);   
	/* �ź�״̬ */
	cJSON_AddStringToObject	(item,MSG,(const char*)(stopdata.msginfo));
	/* ����ʱ�� */

	result = cJSON_Print(root);
	strcpy(cJsonRoom,(const char*)result);
		
	cJSON_Delete(root);
	myfree(result);

    /* ʹ��ȫ������ */
	return cJsonRoom;
}

/*******************************************************************************
�������ƣ�CJSON_to_StopCtrCmd
��    �ܣ�����JSON����(�·�ͣ����������)
˵    ����
��    ��:json_string:cJSON���ݻ�����
        stopctr���������ͣ������
��    ��: ����ֵ:ͣ������
ע    �⣺
*******************************************************************************/
uint8_t  CJSON_to_StopCtrCmd( char * json_string,StopCtrCmd * stopctr)
{
	cJSON *item;
	cJSON *root = cJSON_Parse(json_string);  /* ��ȡJSON�˵� */

	if (root == NULL) {
		return 0; /* JSON����ʧ�� */
	}
	cJSON *object = cJSON_GetObjectItem(root,CONMACH); 
	if (object == NULL) {
		cJSON_Delete(root);
		return 0; /* JSON����ʧ�� */
	}

	item = cJSON_GetObjectItem(object,UUID); /* ȡUUID */
	if (item == NULL) {
		cJSON_Delete(root);
		return 0; /* JSON����ʧ�� */
	}
	memcpy(stopctr->uuid,item->valuestring,strlen(item->valuestring));

	item = cJSON_GetObjectItem(object,STATUS); /* ȡ״̬ */
	if (item == NULL) {
		cJSON_Delete(root);
		return 0; /* JSON����ʧ�� */
	}
	stopctr->status = item->valueint;

	item = cJSON_GetObjectItem(object,MODE); /* ȡģʽ */
	if (item == NULL) {
		cJSON_Delete(root);
		return 0; /* JSON����ʧ�� */
	}
	stopctr->mode=item->valueint;

	item = cJSON_GetObjectItem(object,OPERATEID); /* ȡִ����ˮ�� */
	if (item == NULL) {
		cJSON_Delete(root);
		return 0; /* JSON����ʧ�� */
	}
	memcpy(stopctr->operateid,item->valuestring,strlen(item->valuestring));

	item = cJSON_GetObjectItem(object,OPERATETIME); /* ���յ��źź��ӳ�ʱ�� */
	if (item == NULL) {
		cJSON_Delete(root);
		return 0; /* JSON����ʧ�� */
	}
	stopctr->operatetime=item->valueint;

	cJSON_Delete(root);

	return 1;
	  
}
/* ����JSON����(�·�����ID) */
/* ����JSON����(ͬ��ʱ��) */

/*******************************************************************************
�������ƣ�CommitCollectData_Upload
��    �ܣ��ɼ����ݴ���ϴ�
˵    ����
��    ��:
��    ��: 
ע    �⣺
*******************************************************************************/
void CommitCollectData_Upload(void)
{
	/* WIFI���ݷ��� */
	memset(JsonDataBuf,0,JSONLENGTH);
	CommitCollectData_to_cJSON(JsonDataBuf,g_sys_sts.commit_data,0);
	WIFI_uart_sendData(JsonDataBuf,strlen(JsonDataBuf));
	/* �������ݷ���       */
}

/*******************************************************************************
�������ƣ�CommitBladeData_Upload
��    �ܣ��������ݴ���ϴ�
˵    ����
��    ��:
��    ��: 
ע    �⣺
*******************************************************************************/
void CommitBladeData_Upload(void)
{
	/* WIFI���ݷ��� */
	memset(JsonDataBuf,0,JSONLENGTH);
	CommitBladeData_to_cJSON(JsonDataBuf,g_sys_sts.commit_data);
	WIFI_uart_sendData(JsonDataBuf,strlen(JsonDataBuf));
	/* Config_uart_sendData(JsonDataBuf,strlen(JsonDataBuf)); */
	/* �������ݷ���       */
}

/*******************************************************************************
�������ƣ�CommitStopData_Upload
��    �ܣ�ͣ�����ݴ���ϴ�
˵    ����
��    ��:
��    ��: 
ע    �⣺
*******************************************************************************/
void CommitStopData_Upload(void)
{  
	/* WIFI���ݷ��� */
	memset(JsonDataBuf,0,JSONLENGTH);
	CommitStopData_to_cJSON(JsonDataBuf,g_sys_sts.stopCtrCmdBack);
	WIFI_uart_sendData(JsonDataBuf,strlen(JsonDataBuf));
}

/*******************************************************************************
�������ƣ�CommitClearData_Upload
��    �ܣ��������ݴ���ϴ�
˵    ����
��    ��:
��    ��: 
ע    �⣺
*******************************************************************************/
void CommitClearData_Upload(void)
{  
	/* WIFI���ݷ��� */
	memset(JsonDataBuf,0,JSONLENGTH);
	CommitCollectData_to_cJSON(JsonDataBuf,g_sys_sts.commit_data,1);
	WIFI_uart_sendData(JsonDataBuf,strlen(JsonDataBuf));
}

/************************ (C) COPYRIGHT Hopewellgroup ***********END OF FILE***/

