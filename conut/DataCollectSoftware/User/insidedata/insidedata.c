/**
  ******************************************************************************
  * @file           : insidedata.c
  * @brief          : ���ݴ����ۺϲ���
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

/*******************************************************************************
�������ƣ�InsideDataHandle(UartUpData *p_updata,UartUpData *p_backdata)
��    �ܣ��ڲ����ݴ���
˵    ����
��    ��: UartUpData *p_updata:�ϴ������� UartUpData *p_backdata:���ص�����
��    �أ���ȷ�����״̬.
ע    �⣺
*******************************************************************************/
u8 InsideDataHandle(UartUpData *p_updata,UartUpData *p_backdata)
{
    u8  state = FALSE;
    switch (p_updata->commend) { 
        case PC_ISSUE_DATA:               //PC�ϴ�������						
        break;	
				      
        case PC_ISSUE_INPAPER:            //PC�·���ֽ
            Config_InpaperEnable(p_updata);		 
        break;
				      
        case PC_ISSUE_CONFIG:             //PC�ϴ�ϵͳ����				
        break;
		
        case PC_ISSUE_UPGRADE:
            Config_uart_Upgrade();          //PC�·��������� 
        break;

        case PC_ISSUE_VERSION:            //PC�·���ȡ�汾��������Ϣ����
            Config_uart_GetVersion();      
        break;

        case CONFIG_TIME:                 //ͬ��ʱ��
            Config_TimeSynchronization(p_updata);
        break;

        case CONFIG_ID:                   //����ID��
            Config_SetIdNumber(p_updata);
        break;

        case CONFIG_CLEAR:                //��������
            Config_ClearNumber(p_updata);
        break;

        case CONFIG_GET:                  //��ȡ����		 
        break;

        case CONFIG_UPLOAD:               //�����ϴ�
        break;
				 
        case CONFIG_COLLECT:              //  �ɼ���������
            Config_Collect(p_updata);
        break;

        case CONFIG_NET:                  //  �ɼ�����������
            Config_Net(p_updata);
        break;

        case GETCONFIG_COLLECT:           //��ȡ�ɼ�����
            Get_Collect_Config();
        break;

        case GETCONFIG_NET:               //��ȡ��������
            Get_Net_Config();              
        break;

        case GETCONFIG_OUT:               //��ȡ�������
            Get_Out_Config();              
        break;

        case CONFIG_OUT:                  //  �������
            Config_Out(p_updata);
        break;

        case HEARTBEAT:                   //����
        break;		 

        default:                          //Ĭ��		 
        break;
				 
    }
    return state;		  
}

//�ɼ����ݴ��
/************************ (C) COPYRIGHT Hopewellgroup ***********END OF FILE***/

