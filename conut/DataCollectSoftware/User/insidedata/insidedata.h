/**
  ******************************************************************************
  * @file           : insidedata.h
  * @brief          : Header for insidedata.c file.
  *                   This file contains the common defines of the application.
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


#ifndef __UARTDATA_H
#define __UARTDATA_H

#include "stm32f4xx_hal.h"
#include "pl_type.h"

//����ͨѶ�ṹ��
#define UARTDATALENGTH 400    //�������ݳ���
#define UARTHEAD       0xFEEF //����ͷ����
#define UARTTAIL       0xEAAE //����β����

#define UARTSKEW       5*sizeof(U16)  //����ͷ�����ݵ�ƫ��
#define LOCALID        0x0000         //����ID
#define PCID           0x0001         //PCID

typedef enum __DATA_CMD__ { 
    PC_ISSUE_DATA     = 0x0001,     //PC�·��������
    PC_ISSUE_INPAPER  = 0x0002,     //PC�·���������
    PC_ISSUE_CONFIG   = 0x0003,     //PC�·���������
	PC_ISSUE_UPGRADE  = 0x0004, 	 //PC�·���������
	PC_ISSUE_VERSION  = 0x0005, 	 //PC�·���ȡ�汾������ʱ������
	PC_ISSUE_WIFISUS  = 0x0006, 	 //PC�·���ȡWIFI״̬

    //�������
    CONFIG_TIME       = 0x0101,      //  ͬ��ʱ��
    CONFIG_ID         = 0x0102,      //  ����ID��
    CONFIG_CLEAR      = 0x0103,      //  ��������
    CONFIG_GET        = 0x0104,      //  ��ȡ����
    CONFIG_UPLOAD     = 0x0105,      //  �����ϴ�
    CONFIG_COLLECT    = 0x0106,      //  �ɼ���������
    CONFIG_NET        = 0x0107,      //  �ɼ�����������
    GETCONFIG_COLLECT = 0x0108,      //  ��ȡ���òɼ�
    GETCONFIG_NET     = 0x0109,      //  ��ȡ��������
    GETCONFIG_OUT     = 0x010A,      //  ��ȡ�������
    CONFIG_OUT        = 0x010B,      //  �������
    PLATTOVISION      = 0x0201,      //  ƽ̨���Ӿ��·�
    VISIONTOPLAT      = 0x0202,      //  �Ӿ���ƽ̨�ϱ�
    HEARTBEAT         = 0x8001,       //����
} DATACMD; 

typedef enum __PC_CONTROL_CMD__ { 
    VIEW     = 0x0001,     //PC�·��������
    BOOT     = 0x0002,     //PC�·�����
    STOP     = 0x0003,     //PC�·�ͣ��
    INPAPER  = 0x0004,     //PC�·���ֽ     
} CONTROLCMD; 

//�Ӿ��ϱ�����
typedef struct
{
   S8   *data;
}PcIssueData;

//�Ӿ��ϱ�����
typedef struct {
    U16 controlcommend;     //����ο�CONTROLCMD
    S8   *data;	//INPAPER����һ���ֽ�Ϊ������VIEW:���ַ���
}PcIssueControl;

//�Ӿ���������
typedef struct {
   S8   *data;
}PcIssueConfig;

//����ͨѶ�ṹ��,���ڴ�������
typedef struct {
    U16 header;      //����ͷ
    U16 devSAID;     //�豸ԴID���ɿ��ƶ���������豸���豸�Զ����䣬����Ϊ0xFFFF
    U16 devDAID;     //�豸Ŀ��ID,�������ݸ��Ķ���
    U16 commend;     //����
    U16 iLenght;     //����������
    U8 databuf[UARTDATALENGTH];  //���ݻ�����
} UartUpData;//�����ϴ�����

//ϵͳ�ϱ�������Ϣ
typedef struct  
{
    U16 tailer;  //����β
}UartUpDatatail;


typedef struct   
{
    U8             rxbuf[UARTDATALENGTH];//��ȡ����
    UartUpData     RxUartData;      //��������
    UartUpDatatail RxUartTail;   //����֡β
    UartUpData     TxUartData;       //�������� 
    UartUpDatatail TxUarttail;   //����֡β
    UartUpData     uartBackData;      //��������
    //��������ʱ�Ĳ���ָ��
    U8 RxSts;    
    //���ݽ�����ɱ�־
    U8 bComProc;
} UartMgr;//usb�ϱ�����


typedef struct   
{
    U8    rxbuf[UARTDATALENGTH];//��ȡ����
    U8    JsonDatabuf[UARTDATALENGTH];//��ȡ����
    U8    RxUartData;               //��������
    U8    RxState;                  //���ݽ���״̬
    //��������ʱ�Ĳ���ָ��
    U16   RxStsLength;    
    U16   JsonStsLength;
    //���ݽ�����ɱ�־
    U8    bComProc;
} JsonUartMgr;//Json�ϱ�����



typedef struct  
{
    U8  S_Number[30];//���
    U8  S_Name[30];//�û���
}PlatToVision;
typedef struct  
{
    U32 NumberOfRejects;//��Ʒ��
    U32 NumberOfQuality;//��Ʒ��   
    U8  S_Number[30];//���
    U8  S_Name[30];//�û���
}VisionToPlat;


U8 InsideDataHandle(UartUpData *p_updata,UartUpData *p_backdata);

#endif

/************************ (C) COPYRIGHT Hopewellgroup ***********END OF FILE***/

