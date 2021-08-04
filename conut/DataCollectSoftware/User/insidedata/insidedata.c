/**
  ******************************************************************************
  * @file           : insidedata.c
  * @brief          : 数据处理综合部分
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
函数名称：InsideDataHandle(UartUpData *p_updata,UartUpData *p_backdata)
功    能：内部数据处理
说    明：
参    数: UartUpData *p_updata:上传的数据 UartUpData *p_backdata:返回的数据
返    回：正确或错误状态.
注    意：
*******************************************************************************/
u8 InsideDataHandle(UartUpData *p_updata,UartUpData *p_backdata)
{
    u8  state = FALSE;
    switch (p_updata->commend) { 
        case PC_ISSUE_DATA:               //PC上传的数据						
        break;	
				      
        case PC_ISSUE_INPAPER:            //PC下发插纸
            Config_InpaperEnable(p_updata);		 
        break;
				      
        case PC_ISSUE_CONFIG:             //PC上传系统配置				
        break;
		
        case PC_ISSUE_UPGRADE:
            Config_uart_Upgrade();          //PC下发升级命令 
        break;

        case PC_ISSUE_VERSION:            //PC下发获取版本、编译信息命令
            Config_uart_GetVersion();      
        break;

        case CONFIG_TIME:                 //同步时间
            Config_TimeSynchronization(p_updata);
        break;

        case CONFIG_ID:                   //配置ID号
            Config_SetIdNumber(p_updata);
        break;

        case CONFIG_CLEAR:                //计数清零
            Config_ClearNumber(p_updata);
        break;

        case CONFIG_GET:                  //获取配置		 
        break;

        case CONFIG_UPLOAD:               //配置上传
        break;
				 
        case CONFIG_COLLECT:              //  采集参数配置
            Config_Collect(p_updata);
        break;

        case CONFIG_NET:                  //  采集器网络配置
            Config_Net(p_updata);
        break;

        case GETCONFIG_COLLECT:           //获取采集配置
            Get_Collect_Config();
        break;

        case GETCONFIG_NET:               //获取网络配置
            Get_Net_Config();              
        break;

        case GETCONFIG_OUT:               //获取输出配置
            Get_Out_Config();              
        break;

        case CONFIG_OUT:                  //  配置输出
            Config_Out(p_updata);
        break;

        case HEARTBEAT:                   //心跳
        break;		 

        default:                          //默认		 
        break;
				 
    }
    return state;		  
}

//采集数据打包
/************************ (C) COPYRIGHT Hopewellgroup ***********END OF FILE***/

