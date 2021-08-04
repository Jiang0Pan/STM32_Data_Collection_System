/**************************************************************************//**
 * @file     system_config.h
 * @brief    ϵͳ����ͷ�ļ�������GPIO��ȫ�ֱ����ȵ�����
 * @version  V1.0
 * @date     20/09/2019
 * @Author   ��ΰ
 * @note
 *
 ******************************************************************************/
#ifndef __SYSTEM_CONFIG_H
#define __SYSTEM_CONFIG_H

#include "stm32f4xx_hal.h"
#include "stdio.h"


//Ӳ������
#include "interruptservice.h"
#include "wifiuart.h"


//�ɼ��ӿ�ͷ�ļ�
#include "collect.h"

//���ýӿ�ͷ�ļ�
#include "config.h"

//�ֽ�ͨѶЭ��
#include "insidedata.h"


//JSONЭ��ͷ�ļ�
#include "cJSON.h"
#include "malloc.h"
#include "outsidedata.h"

//FLASH
#include "stmflash.h"

//�ⲿ����
#include "outcontrol.h"
#include "pl_type.h"


//��ʼ������
#define  DEVID             "1234567890"
/*
	�汾��Ϣ: A.B.C
	A: ��ʾ�ش�汾�������漰�ܶ๦�ܻ�����������
	B: ģ�鹦�����ƻ�����ӡ�
	C: ����bug�޸ġ�
*/
#define  Firmware_Version  "1.0.1"
#define  APPASK            "APP:OK"

//����
#define  VERSION_MAX       (100)           //�汾��Ϣ�����С
#define  COPMIER_MAX       (20)            //������Ϣ�����С
#define  UPGRAGE_FLAG      (0x8888)        //��λ������־λ
#define  BOOTFLAG_ADDR     (0x0800FC04)    //������־λ��ŵĵ�ַ
#define  FLASH_ENV_ADDR    (0x080E0000)    //���128K����������

//BOOL״̬����
typedef enum _STATE_ { 
    FALSE = 0,          //��Ч
    TRUE = 1,            //��Ч
}STATE;


#pragma pack (1) //�ڴ�һ�ֽڶ���
/* ϵͳ���� */
typedef struct _SystemConfig_ {
    U16           enable;   /* ʹ������ */
    U16          u16upgradeflag;   /* ����flag, 0x8888 ��λ������ */
    DevCfg       dev_cfg;        /* �豸���� */
    CollectCfg   col_cfg;   /* �ɼ��ź����� */
    NetCfg       net_cfg;        /* ��������  */
    OutCfg       out_cfg;        /* ������� */
}SysCfg;
#pragma pack ()


/* ϵͳ״̬ */
typedef struct _SystemState_
{
	 //�źŲɼ�����
	 CollectData        col_info; /* �ɼ��ź��м�״̬ */
	 CollectSigState    col_sts;  /* �ɼ��ź�����״̬  */
	 CommitCollectData  commit_data;       /* �ɼ�����ʱ�ύ���� */
	 RealTimeStore      realtime_data;     /* ʵʱ���ݴ洢 */
	 //������Ʋ���
	 InPaperMgr         inpaper_mgr;       /* ��ֽ���� */
	 //ͣ������
	 StopCtrCmd         stopCtrCmd;        /* ͣ������ */
	 StopCtrCmdBack     stopCtrCmdBack;    /* ͣ������� */
	 StopMgr            stop_mgr;          /* ͣ�����Ʋ���,��Ҫ������ʱ���� */
	 //ָʾ�Ʋ���
	 //ͨѶ����
	 JsonUartMgr wifi_cfg_mgr;
	 WifiMgr  wifi_mgr;
	 //���ò���
	 UartMgr   uart_frm;/* uart֡�ṹ */
	 ConfigMgr cfg_mgr;
	
}SysSts;

extern void system_init(void);
extern SysCfg g_sys_cfg;
extern SysSts g_sys_sts;
extern RTC_HandleTypeDef hrtc;

#endif
