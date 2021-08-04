/**
  ******************************************************************************
  * @file           : config.c
  * @brief          : drive of configure device by PC
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
#include "outsidedata.h"

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart6;
extern U8 commit[];

void Config_Init(void)
{
    char data[VERSION_MAX]={0};
    sprintf(data,"Commit: %s,FW: %s\n",commit, Firmware_Version);
    HAL_UART_Transmit_DMA(&huart4,(uint8_t *)data,strlen(data));	
}

/*******************************************************************************
函数名称：Config_PcDataAnaly(uint8_t data)
功    能：从串口得到数据解析
说    明：
参    数: data:接受到的数据
返    回：
注    意：
*******************************************************************************/
void Config_PcDataAnaly(u8 data)
{
    static unsigned char RecDataBuf[2];
    uint32_t TempComDat = 0;
    static uint16_t datacount = 0;
    RecDataBuf[0]=RecDataBuf[1];
    RecDataBuf[1]=data;
    TempComDat = data;
	
    if(RecDataBuf[0]==0xEF&&RecDataBuf[1]==0xFE) {
        g_sys_sts.uart_frm.RxSts = 0;//数据开始接收
        datacount = 0;    //数据计数长度
        memset(&g_sys_sts.uart_frm.RxUartData,0,sizeof(UartUpData));//数据清空
        g_sys_sts.uart_frm.RxUartData.header = UARTHEAD;		  
    }
	
    switch (g_sys_sts.uart_frm.RxSts) {
        case 0:     // 接收开始
            g_sys_sts.uart_frm.RxSts = 1;
        break;
			
        case 1:     // 源ID号
            g_sys_sts.uart_frm.RxUartData.devSAID +=  TempComDat<<(datacount*8);
            datacount++;
            if(datacount >= sizeof(u16)) {
                datacount = 0;
                g_sys_sts.uart_frm.RxSts= 2;
            }	
        break;
			
        case 2:     // 目标ID号
            g_sys_sts.uart_frm.RxUartData.devDAID +=  TempComDat<<(datacount*8);
            datacount++;
            if(datacount >= sizeof(u16)) {
                datacount = 0;
                g_sys_sts.uart_frm.RxSts= 3;
            }	
        break;
		
        case 3:     // 接收命令
            g_sys_sts.uart_frm.RxUartData.commend +=  TempComDat<<(datacount*8);
            datacount++;
			
            if (datacount >= sizeof(u16)) {
                datacount = 0;
                g_sys_sts.uart_frm.RxSts= 4;
                g_sys_sts.uart_frm.RxUartData.iLenght=0;
            }	
        break;
			
        case 4:     // 接收数据长度
            g_sys_sts.uart_frm.RxUartData.iLenght +=  TempComDat<<(datacount*8);
            datacount++;
			
            if (datacount >= sizeof(u16)) {
                datacount = 0;
				
                if (g_sys_sts.uart_frm.RxUartData.iLenght > 0) {
                    g_sys_sts.uart_frm.RxSts= 5;
                }

				else {
                    g_sys_sts.uart_frm.RxUartTail.tailer = 0; 
                    g_sys_sts.uart_frm.RxSts= 6;
                }
            }	
        break;
			
        case 5:     //接收数据
            g_sys_sts.uart_frm.RxUartData.databuf[datacount++] = data;
			
            if (datacount >= g_sys_sts.uart_frm.RxUartData.iLenght) {
                datacount = 0;
                g_sys_sts.uart_frm.RxSts = 6;
                g_sys_sts.uart_frm.RxUartTail.tailer = 0;
            }	
        break;
			
        case 6:     // 接收帧尾
            g_sys_sts.uart_frm.RxUartTail.tailer +=  TempComDat<<(datacount*8);
            datacount++;
			
            if (datacount >= sizeof(u16)) {
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
void Config_uart_sendData(u8* buf,u16 length)
{
    HAL_UART_Transmit_DMA(&huart1,buf,length);
}

/*******************************************************************************
函数名称：Config_uart_sendStructData(UartUpData *RxUartData)
功    能：发送结构体数组
说    明：
参    数: UartUpData *RxUartData:包含数据头设备源ID命令及数据长度等
返    回：
注    意：
*******************************************************************************/
void Config_uart_sendStructData(UartUpData *RxUartData)
{
    g_sys_sts.uart_frm.TxUarttail.tailer = UARTTAIL;
    memcpy((uint8_t *)(&RxUartData->databuf[RxUartData->iLenght]),(uint8_t *)&g_sys_sts.uart_frm.TxUarttail.tailer,sizeof(UartUpDatatail));
    Config_uart_sendData((uint8_t *)RxUartData,RxUartData->iLenght + UARTSKEW+sizeof(UartUpDatatail));	  
}

/*******************************************************************************
函数名称：Config_Uart_Handle()
功    能：处理Config串口数据
说    明：
参    数: 
返    回：
注    意：
*******************************************************************************/
void Config_Uart_Handle(void)
{ 
    if(g_sys_sts.uart_frm.bComProc == 1) {
        g_sys_sts.uart_frm.bComProc = 0;
		
        /*频率展示LED*/
        /*处理上位机发送过来的参数*/
        if(InsideDataHandle(&g_sys_sts.uart_frm.RxUartData,&g_sys_sts.uart_frm.uartBackData)) {
            Config_uart_sendStructData(&g_sys_sts.uart_frm.uartBackData);
        }
    }
}

/*******************************************************************************
函数名称：Config_uart_Upgrade()
功    能：串口升级
说    明： 
参    数: 
返    回：
注    意：
*******************************************************************************/
void Config_uart_Upgrade(void)
{
	UpGradeAsk upgrade;
	g_sys_cfg.u16upgradeflag = UPGRAGE_FLAG;
    STMFLASH_Write(FLASH_ENV_ADDR,(u32*)&g_sys_cfg, sizeof(SysCfg));
	
	g_sys_sts.uart_frm.TxUartData.header = UARTHEAD;
	g_sys_sts.uart_frm.TxUartData.devDAID = 0;
	g_sys_sts.uart_frm.TxUartData.devSAID = LOCALID;
	g_sys_sts.uart_frm.TxUartData.commend = PC_ISSUE_UPGRADE;
	g_sys_sts.uart_frm.TxUartData.iLenght = sizeof(UpGradeAsk);

	sprintf(upgrade.gradeask,"%s",APPASK);
	memcpy(g_sys_sts.uart_frm.TxUartData.databuf,(char *)&upgrade.gradeask,g_sys_sts.uart_frm.TxUartData.iLenght);
	Config_uart_sendStructData(&g_sys_sts.uart_frm.TxUartData);
	HAL_Delay(50);
    __set_PRIMASK(1);
    __set_FAULTMASK(1);
    NVIC_SystemReset(); // 软件系统复位
}
    
/*******************************************************************************
函数名称：Config_uart_GetVersion()
功    能：上传版本
说    明：
参    数: 
返    回：
注    意：
*******************************************************************************/
void Config_uart_GetVersion(void)
{
	VersionCompier data ;
	sprintf(data.version,"%s",Firmware_Version);
	
    u16  Year;
    u8   Month;
    u8   Day;
    u8   i;
    const unsigned char code_DataStr[COPMIER_MAX] = __DATE__;    
    const unsigned char code_TimeStr[COPMIER_MAX] = __TIME__;
    const char *pMonth[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
	
    for (i = 0; i < 12; i++) {
        if (memcmp(code_DataStr, pMonth[i], 3) == 0) {
            Month = i + 1;
            i = 12;
        }
    }
	
    Year = (u16)atoi(code_DataStr + 7);
    Day = (u8)atoi(code_DataStr + 4);		
    sprintf(data.compier,"%d-%d-%d %s",Year,Month,Day,code_TimeStr);

	g_sys_sts.uart_frm.TxUartData.header = UARTHEAD;
	g_sys_sts.uart_frm.TxUartData.devDAID = 0;
	g_sys_sts.uart_frm.TxUartData.devSAID = LOCALID;
	g_sys_sts.uart_frm.TxUartData.commend = PC_ISSUE_VERSION;
	g_sys_sts.uart_frm.TxUartData.iLenght = sizeof(VersionCompier);

	memcpy(g_sys_sts.uart_frm.TxUartData.databuf,(char *)&data,g_sys_sts.uart_frm.TxUartData.iLenght);
	Config_uart_sendStructData(&g_sys_sts.uart_frm.TxUartData);
}	

/*******************************************************************************
函数名称：Config_TimeSynchronization(UartUpData *RxUartData)
功    能：时间同步
说    明：
参    数: UartUpData *RxUartData:包含数据头设备源ID命令及数据长度等
返    回：
注    意：
*******************************************************************************/
void  Config_TimeSynchronization(UartUpData *RxUartData)
{
    RTC_TimeTypeDef sTime;
    RTC_DateTypeDef DateToUpdate;
	
    RTC_DateType *timeddata = (RTC_DateType *)RxUartData->databuf;
	
    //设置时间
    sTime.Hours = timeddata->Hours;
    sTime.Minutes = timeddata->Minutes;
    sTime.Seconds = timeddata->Seconds;
    sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    sTime.StoreOperation = RTC_STOREOPERATION_RESET;
    HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	
	
    //设置日期
    DateToUpdate.WeekDay = timeddata->WeekDay;
    DateToUpdate.Month = timeddata->Month;
    DateToUpdate.Date = timeddata->Date;
    DateToUpdate.Year = timeddata->Year;  
	 
    HAL_RTC_SetDate(&hrtc,&DateToUpdate,RTC_FORMAT_BIN);

}

/*******************************************************************************
函数名称：Config_InpaperEnable(UartUpData *RxUartData)
功    能：插纸机使能
说    明：
参    数: UartUpData *RxUartData:包含数据头设备源ID命令及数据长度等
返    回：
注    意：
*******************************************************************************/
void  Config_InpaperEnable(UartUpData *RxUartData)
{
    g_sys_sts.inpaper_mgr.inpaperenable = TRUE;
}

/*******************************************************************************
函数名称：Config_SetIdNumber(UartUpData *RxUartData)
功    能：ID号设置
说    明：
参    数: UartUpData *RxUartData:包含数据头设备源ID命令及数据长度等
返    回：
注    意：
*******************************************************************************/
void  Config_SetIdNumber(UartUpData *RxUartData)
{
    // 使用STM32 Unique ID作为作为 UUID，不需要额外配置。
}

/*******************************************************************************
函数名称：Config_ClearNumber(UartUpData *RxUartData)
功    能：计数清零
说    明：
参    数: UartUpData *RxUartData:包含数据头设备源ID命令及数据长度等
返    回：
注    意：
*******************************************************************************/
void  Config_ClearNumber(UartUpData *RxUartData)
{
    Collect_init(); //采集重新初始化
}

/*******************************************************************************
函数名称：Config_Collect(UartUpData *RxUartData)
功    能：采集参数配置
说    明：
参    数: UartUpData *RxUartData:包含数据头设备源ID命令及数据长度等
返    回：
注    意：
*******************************************************************************/
void  Config_Collect(UartUpData *RxUartData)
{
    memcpy(&g_sys_cfg.col_cfg ,(CollectCfg *)RxUartData->databuf,sizeof(CollectCfg));
    //写入到FALSH
    g_sys_cfg.enable = 1;
    /*仅将环境变量从EEPROM改存到FLASH,其他数据不再保存*/
    STMFLASH_Write(FLASH_ENV_ADDR,(u32*)&g_sys_cfg, sizeof(SysCfg));
}

/*******************************************************************************
函数名称：Get_Collect_Config()
功    能：获取采集配置
说    明：
参    数: 
返    回：
注    意：
*******************************************************************************/
void  Get_Collect_Config(void)
{
    g_sys_sts.uart_frm.TxUartData.header = UARTHEAD;
    g_sys_sts.uart_frm.TxUartData.devDAID = 0;
    g_sys_sts.uart_frm.TxUartData.devSAID = LOCALID;
    g_sys_sts.uart_frm.TxUartData.commend = GETCONFIG_COLLECT;
    g_sys_sts.uart_frm.TxUartData.iLenght = sizeof(CollectCfg);
	 
    memcpy(g_sys_sts.uart_frm.TxUartData.databuf,(uint8_t *)&g_sys_cfg.col_cfg,g_sys_sts.uart_frm.TxUartData.iLenght);
    Config_uart_sendStructData(&g_sys_sts.uart_frm.TxUartData);

}

/*******************************************************************************
函数名称：Get_Net_Config()
功    能：获取网络
说    明：
参    数: 
返    回：
注    意：
*****************************************************	**************************/
void  Get_Net_Config(void)
{
    g_sys_sts.uart_frm.TxUartData.header = UARTHEAD;
    g_sys_sts.uart_frm.TxUartData.devDAID = 0;
    g_sys_sts.uart_frm.TxUartData.devSAID = LOCALID;
    g_sys_sts.uart_frm.TxUartData.commend = GETCONFIG_NET;
    g_sys_sts.uart_frm.TxUartData.iLenght = sizeof(NetCfg);
	
    memcpy(g_sys_sts.uart_frm.TxUartData.databuf,(uint8_t *)&g_sys_cfg.net_cfg,g_sys_sts.uart_frm.TxUartData.iLenght);
    Config_uart_sendStructData(&g_sys_sts.uart_frm.TxUartData);

}

/*******************************************************************************
函数名称：Config_Net(UartUpData *RxUartData)
功    能：网络设置
说    明：
参    数: UartUpData *RxUartData:包含数据头设备源ID命令及数据长度等
返    回：
注    意：
*******************************************************************************/
void  Config_Net(UartUpData *RxUartData)
{
    char databuf[100];
    g_sys_sts.wifi_mgr.configenable = 1;
    NetCfg* netconfig = (NetCfg *)RxUartData->databuf;
    memset(&g_sys_cfg.net_cfg,0,sizeof(NetCfg));
    memcpy(&g_sys_cfg.net_cfg ,RxUartData->databuf,sizeof(NetCfg));
	
    g_sys_cfg.enable = 1;
    /*仅将环境变量从EEPROM改存到FLASH,其他数据不再保存*/
    STMFLASH_Write(FLASH_ENV_ADDR,(u32*)&g_sys_cfg, sizeof(SysCfg));
//
//	/*恢复出厂设置*/
//    do {
//        delay_ms(200);
//        WIFI_uart_sendData(CMDSTART1,strlen(CMDSTART1));
//        memset(g_sys_sts.wifi_cfg_mgr.rxbuf,0,UARTDATALENGTH);
//        HAL_UART_Receive_DMA(&huart6,g_sys_sts.wifi_cfg_mgr.rxbuf,1);
//        delay_ms(200);
//    } while (g_sys_sts.wifi_cfg_mgr.rxbuf[0] != 'a');
//		 
//    do{ 
//        delay_ms(200);
//        WIFI_uart_sendData(CMDSTART2,strlen(CMDSTART2));
//        memset(g_sys_sts.wifi_cfg_mgr.rxbuf,0,UARTDATALENGTH);
//        HAL_UART_Receive_DMA(&huart6,g_sys_sts.wifi_cfg_mgr.rxbuf,3);
//        delay_ms(200);
//    } while (strcmp((const char*)g_sys_sts.wifi_cfg_mgr.rxbuf,"+ok") != 0);
//
//	do{ 
//        delay_ms(200);
//        WIFI_uart_sendData(FUNCTION,strlen(FUNCTION));
//        memset(g_sys_sts.wifi_cfg_mgr.rxbuf,0,UARTDATALENGTH);
//        HAL_UART_Receive_DMA(&huart6,g_sys_sts.wifi_cfg_mgr.rxbuf,3);
//        delay_ms(200);
//    } while (strcmp((const char*)g_sys_sts.wifi_cfg_mgr.rxbuf,"k") != 0);	
	
    /*配置网络*/
    do {
        delay_ms(200);
        WIFI_uart_sendData(CMDSTART1,strlen(CMDSTART1));
        memset(g_sys_sts.wifi_cfg_mgr.rxbuf,0,UARTDATALENGTH);
        HAL_UART_Receive_DMA(&huart6,g_sys_sts.wifi_cfg_mgr.rxbuf,1);
        delay_ms(200);
    } while (g_sys_sts.wifi_cfg_mgr.rxbuf[0] != 'a');
		 
    do{ 
        delay_ms(200);
        WIFI_uart_sendData(CMDSTART2,strlen(CMDSTART2));
        memset(g_sys_sts.wifi_cfg_mgr.rxbuf,0,UARTDATALENGTH);
        HAL_UART_Receive_DMA(&huart6,g_sys_sts.wifi_cfg_mgr.rxbuf,3);
        delay_ms(200);
    } while (strcmp((const char*)g_sys_sts.wifi_cfg_mgr.rxbuf,"+ok") != 0);

    memset(databuf,0,100);
    sprintf(databuf,MODECMD,netconfig->mode);	
    do{	
        delay_ms(200);		
        WIFI_uart_sendData(databuf,strlen(databuf));
        memset(g_sys_sts.wifi_cfg_mgr.rxbuf,0,UARTDATALENGTH);
        HAL_UART_Receive_DMA(&huart6,g_sys_sts.wifi_cfg_mgr.rxbuf,100);
        delay_ms(500);
    } while (strchr((const char*)g_sys_sts.wifi_cfg_mgr.rxbuf,'k') == NULL);

	if (strncmp((const char*)netconfig->mode,"AP",2) == 0 ) {
		memset(databuf,0,100);
	    sprintf(databuf,APNAME,netconfig->apname);
		 
	    do{
	        delay_ms(200);
	        WIFI_uart_sendData(databuf,strlen(databuf));
	        memset(g_sys_sts.wifi_cfg_mgr.rxbuf,0,UARTDATALENGTH);
	        HAL_UART_Receive_DMA(&huart6,g_sys_sts.wifi_cfg_mgr.rxbuf,100);
	        delay_ms(500);
	    } while (strchr((const char*)g_sys_sts.wifi_cfg_mgr.rxbuf,'k') == NULL);
		
		memset(databuf,0,100);
	    sprintf(databuf,APPASSWORD,netconfig->appasswd);
		 
	    do{
	        delay_ms(200);
	        WIFI_uart_sendData(databuf,strlen(databuf));
	        memset(g_sys_sts.wifi_cfg_mgr.rxbuf,0,UARTDATALENGTH);
	        HAL_UART_Receive_DMA(&huart6,g_sys_sts.wifi_cfg_mgr.rxbuf,100);
	        delay_ms(500);
	    } while (strchr((const char*)g_sys_sts.wifi_cfg_mgr.rxbuf,'k') == NULL);	
	}

	if ((strncmp((const char*)netconfig->mode,"STA",3) == 0) || 
		(strncmp((const char*)netconfig->mode,"APSTA",5) == 0)) {
	    memset(databuf,0,100);
	    sprintf(databuf,ROUTERCMD,netconfig->wifiname,netconfig->wifi_pwd);
		 
	    do{
	        delay_ms(200);
	        WIFI_uart_sendData(databuf,strlen(databuf));
	        memset(g_sys_sts.wifi_cfg_mgr.rxbuf,0,UARTDATALENGTH);
	        HAL_UART_Receive_DMA(&huart6,g_sys_sts.wifi_cfg_mgr.rxbuf,100);
	        delay_ms(500);
	    } while (strchr((const char*)g_sys_sts.wifi_cfg_mgr.rxbuf,'k') == NULL);		
	}

    memset(databuf,0,100);
    sprintf(databuf,NETCMD,netconfig->ser_port,netconfig->ser_ip);
	 	 
    do{
        delay_ms(200);
        WIFI_uart_sendData(databuf,strlen(databuf));
        memset(g_sys_sts.wifi_cfg_mgr.rxbuf,0,UARTDATALENGTH);
        HAL_UART_Receive_DMA(&huart6,g_sys_sts.wifi_cfg_mgr.rxbuf,100); 
        delay_ms(500);
	 } while (strchr((const char*)g_sys_sts.wifi_cfg_mgr.rxbuf,'k') == NULL);
	 
    memset(databuf,0,100);
	 
    /* dhcp */
	if (strncmp((const char*)netconfig->dhcp_en,"DHCP",4) == 0) { //netconfig->dhcp_en) {
		sprintf(databuf,NETAUTOCONFIG);
        printf("come DHCP?\r\n");
	}
    /*static ip */
	else if (strncmp((const char*)netconfig->dhcp_en,"STATIC",6) == 0) {
		sprintf(databuf,NETIPCONFIG,netconfig->ip,netconfig->subnet_mask,netconfig->gateway);
        printf("come static?\r\n");
	}
//    if ((strlen((const char *)netconfig->ip))&&(strlen((const char *)netconfig->subnet_mask))&(strlen((const char *)netconfig->gateway))) {
//        sprintf(databuf,NETIPCONFIG,netconfig->ip,netconfig->subnet_mask,netconfig->gateway);
//        printf("come static?\r\n");
//    }
//	
//    else {
//        sprintf(databuf,NETAUTOCONFIG);
//        printf("come DHCP?\r\n");
//    }
	
    do {
        delay_ms(200);
        WIFI_uart_sendData(databuf,strlen(databuf));
        memset(g_sys_sts.wifi_cfg_mgr.rxbuf,0,UARTDATALENGTH);
        HAL_UART_Receive_DMA(&huart6,g_sys_sts.wifi_cfg_mgr.rxbuf,100); 
        delay_ms(500);
    } while (strchr((const char*)g_sys_sts.wifi_cfg_mgr.rxbuf,'k') == NULL);
	 printf("restart wifi\r\n");
    //重启WiFi模块
	delay_ms(200);
    WIFI_uart_sendData(RESTART,strlen(RESTART));
	delay_ms(200);
    // memset(g_sys_sts.wifi_cfg_mgr.rxDatabuf,0,UARTDATALENGTH);
    // HAL_UART_Receive_DMA(&huart6,g_sys_sts.wifi_cfg_mgr.rxDatabuf,100);
    // delay_ms(1000);

	 
    // __set_FAULTMASK(1);
    //NVIC_SystemReset();
		
}

/*******************************************************************************
函数名称：Get_Out_Config()
功    能：获取输出配置
说    明：
参    数: 
返    回：
注    意：
*******************************************************************************/
void  Get_Out_Config(void)
{
    g_sys_sts.uart_frm.TxUartData.header = UARTHEAD;
    g_sys_sts.uart_frm.TxUartData.devDAID = 0;
    g_sys_sts.uart_frm.TxUartData.devSAID = LOCALID;
    g_sys_sts.uart_frm.TxUartData.commend = GETCONFIG_OUT;
    g_sys_sts.uart_frm.TxUartData.iLenght = sizeof(OutCfg);
	
    memcpy(g_sys_sts.uart_frm.TxUartData.databuf,(uint8_t *)&g_sys_cfg.out_cfg,g_sys_sts.uart_frm.TxUartData.iLenght);
    Config_uart_sendStructData(&g_sys_sts.uart_frm.TxUartData);
}

/*******************************************************************************
函数名称：Config_Out(UartUpData *RxUartData)
功    能：输出配置
说    明：
参    数: UartUpData *RxUartData:包含数据头设备源ID命令及数据长度等
返    回：
注    意：
*******************************************************************************/
void  Config_Out(UartUpData *RxUartData)
{
    g_sys_sts.wifi_mgr.configenable = 1;
    memset(&g_sys_cfg.out_cfg,0,sizeof(OutCfg));
    memcpy(&g_sys_cfg.out_cfg ,RxUartData->databuf,sizeof(OutCfg));
	
    g_sys_cfg.enable = 1;
    /*仅将环境变量从EEPROM改存到FLASH,其他数据不再保存*/
    STMFLASH_Write(FLASH_ENV_ADDR,(u32*)&g_sys_cfg, sizeof(SysCfg));
}

/*******************************************************************************
函数名称：Config_Uart_PackPcData(CommitCollectData *datainfo)
功    能：提交给配置软件机当前参数信息
说    明：
参    数: CommitCollectData *datainfo:盒子的GUID号 机器总时间 停机 脉冲数等
返    回：
注    意：
*******************************************************************************/
void Config_Uart_PackPcData(CommitCollectData *datainfo)
{
    g_sys_sts.uart_frm.TxUartData.header = UARTHEAD;
    g_sys_sts.uart_frm.TxUartData.devDAID = 0;
    g_sys_sts.uart_frm.TxUartData.devSAID = LOCALID;
    g_sys_sts.uart_frm.TxUartData.commend = CONFIG_UPLOAD;
    g_sys_sts.uart_frm.TxUartData.iLenght = sizeof(CommitCollectData);
    memcpy(g_sys_sts.uart_frm.TxUartData.databuf,(uint8_t *)datainfo,g_sys_sts.uart_frm.TxUartData.iLenght);
    Config_uart_sendStructData(&g_sys_sts.uart_frm.TxUartData);
}

/*******************************************************************************
函数名称：Config_UploadMonitorState(void)
功    能：上报当前检测状态
说    明：
参    数:  
返    回：
注    意：
*******************************************************************************/
void  Config_UploadMonitorState(void)
{
    if (g_sys_sts.cfg_mgr.upload_en == TRUE) { 
        g_sys_sts.cfg_mgr.upload_en = FALSE;
        memcpy(g_sys_sts.commit_data.guid , g_sys_cfg.dev_cfg.guid,sizeof(int8_t)*UUIDBUFLENGTH);                      //提交的盒子guid号
        //数据打包
        g_sys_sts.commit_data.alltime   = g_sys_sts.col_info.all_tm;     //机器总时间
        g_sys_sts.commit_data.boottime  = g_sys_sts.col_info.bt_tm;        //正常开机总时间
        g_sys_sts.commit_data.stoptime  = g_sys_sts.col_info.st_tm;        //停机总时间
        g_sys_sts.commit_data.faulttime = g_sys_sts.col_info.ft_tm;       //故障总时间
				
        g_sys_sts.commit_data.bootstopsig = ((g_sys_sts.col_sts.bootsig !=0) && (g_sys_sts.col_sts.mainpulsesig!=0));       //设备状态开机或者停机
        g_sys_sts.commit_data.faultsig = g_sys_sts.col_sts.faultsig;	            //错误信号状态	
        g_sys_sts.commit_data.pulsecount = g_sys_sts.col_sts.mainpulsecount;     //脉冲数
			 
        //2019年11月30日   新增功能刀版脉冲
        g_sys_sts.commit_data.bladepulsesig = g_sys_sts.col_sts.bladepulsesig;     //刀版状态
        g_sys_sts.commit_data.bladepulsecount = g_sys_sts.col_sts.bladepulsecount;     //刀版脉冲

		g_sys_sts.commit_data.revpulsesig = g_sys_sts.col_sts.revpulsesig;
		g_sys_sts.commit_data.revpulsecount = g_sys_sts.col_sts.revpulsecount;

		sprintf((char*)g_sys_sts.commit_data.bladepulsetime,"%02d:%02d:%02d",
															g_sys_sts.col_sts.bladepulsetime.Hours,
															g_sys_sts.col_sts.bladepulsetime.Minutes,
															g_sys_sts.col_sts.bladepulsetime.Seconds);
			 
        HAL_RTC_GetDate(&hrtc, &g_sys_sts.col_info.cur_tm.sdate, RTC_FORMAT_BIN);
        delay_ms(1);
        HAL_RTC_GetTime(&hrtc, &g_sys_sts.col_info.cur_tm.stime, RTC_FORMAT_BIN);
        sprintf((char*)g_sys_sts.commit_data.timecurrent,"20%02d-%02d-%02d %02d:%02d:%02d",
															g_sys_sts.col_info.cur_tm.sdate.Year,
															g_sys_sts.col_info.cur_tm.sdate.Month,
															g_sys_sts.col_info.cur_tm.sdate.Date,
															g_sys_sts.col_info.cur_tm.stime.Hours,
															g_sys_sts.col_info.cur_tm.stime.Minutes,
															g_sys_sts.col_info.cur_tm.stime.Seconds);
				
        g_sys_sts.commit_data.pnopaperstate = g_sys_sts.col_sts.pnopaperstate;
        Config_Uart_PackPcData(&g_sys_sts.commit_data);
    }  
}


/*******************************************************************************
函数名称：Config_UploadTimeCount(void)
功    能：配置信息采集数据定时上报
说    明：
参    数:  
返    回：
注    意：
*******************************************************************************/
void  Config_UploadTimeCount(void)
{
    static int time_count = 0;
    time_count ++;
    if (time_count >= CONFIGUPLOADTIME) {
        time_count = 0;			
        g_sys_sts.cfg_mgr.upload_en = TRUE;
    }      
}

/************************ (C) COPYRIGHT Hopewellgroup ***********END OF FILE***/

