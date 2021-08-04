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
�������ƣ�Config_PcDataAnaly(uint8_t data)
��    �ܣ��Ӵ��ڵõ����ݽ���
˵    ����
��    ��: data:���ܵ�������
��    �أ�
ע    �⣺
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
        g_sys_sts.uart_frm.RxSts = 0;//���ݿ�ʼ����
        datacount = 0;    //���ݼ�������
        memset(&g_sys_sts.uart_frm.RxUartData,0,sizeof(UartUpData));//�������
        g_sys_sts.uart_frm.RxUartData.header = UARTHEAD;		  
    }
	
    switch (g_sys_sts.uart_frm.RxSts) {
        case 0:     // ���տ�ʼ
            g_sys_sts.uart_frm.RxSts = 1;
        break;
			
        case 1:     // ԴID��
            g_sys_sts.uart_frm.RxUartData.devSAID +=  TempComDat<<(datacount*8);
            datacount++;
            if(datacount >= sizeof(u16)) {
                datacount = 0;
                g_sys_sts.uart_frm.RxSts= 2;
            }	
        break;
			
        case 2:     // Ŀ��ID��
            g_sys_sts.uart_frm.RxUartData.devDAID +=  TempComDat<<(datacount*8);
            datacount++;
            if(datacount >= sizeof(u16)) {
                datacount = 0;
                g_sys_sts.uart_frm.RxSts= 3;
            }	
        break;
		
        case 3:     // ��������
            g_sys_sts.uart_frm.RxUartData.commend +=  TempComDat<<(datacount*8);
            datacount++;
			
            if (datacount >= sizeof(u16)) {
                datacount = 0;
                g_sys_sts.uart_frm.RxSts= 4;
                g_sys_sts.uart_frm.RxUartData.iLenght=0;
            }	
        break;
			
        case 4:     // �������ݳ���
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
			
        case 5:     //��������
            g_sys_sts.uart_frm.RxUartData.databuf[datacount++] = data;
			
            if (datacount >= g_sys_sts.uart_frm.RxUartData.iLenght) {
                datacount = 0;
                g_sys_sts.uart_frm.RxSts = 6;
                g_sys_sts.uart_frm.RxUartTail.tailer = 0;
            }	
        break;
			
        case 6:     // ����֡β
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
�������ƣ�Config_uart_sendData(uint8_t* buf,uint16_t length)
��    �ܣ���������
˵    ����
��    ��: buf:���͵����� length:���͵����ݳ���
��    �أ�
ע    �⣺
*******************************************************************************/
void Config_uart_sendData(u8* buf,u16 length)
{
    HAL_UART_Transmit_DMA(&huart1,buf,length);
}

/*******************************************************************************
�������ƣ�Config_uart_sendStructData(UartUpData *RxUartData)
��    �ܣ����ͽṹ������
˵    ����
��    ��: UartUpData *RxUartData:��������ͷ�豸ԴID������ݳ��ȵ�
��    �أ�
ע    �⣺
*******************************************************************************/
void Config_uart_sendStructData(UartUpData *RxUartData)
{
    g_sys_sts.uart_frm.TxUarttail.tailer = UARTTAIL;
    memcpy((uint8_t *)(&RxUartData->databuf[RxUartData->iLenght]),(uint8_t *)&g_sys_sts.uart_frm.TxUarttail.tailer,sizeof(UartUpDatatail));
    Config_uart_sendData((uint8_t *)RxUartData,RxUartData->iLenght + UARTSKEW+sizeof(UartUpDatatail));	  
}

/*******************************************************************************
�������ƣ�Config_Uart_Handle()
��    �ܣ�����Config��������
˵    ����
��    ��: 
��    �أ�
ע    �⣺
*******************************************************************************/
void Config_Uart_Handle(void)
{ 
    if(g_sys_sts.uart_frm.bComProc == 1) {
        g_sys_sts.uart_frm.bComProc = 0;
		
        /*Ƶ��չʾLED*/
        /*������λ�����͹����Ĳ���*/
        if(InsideDataHandle(&g_sys_sts.uart_frm.RxUartData,&g_sys_sts.uart_frm.uartBackData)) {
            Config_uart_sendStructData(&g_sys_sts.uart_frm.uartBackData);
        }
    }
}

/*******************************************************************************
�������ƣ�Config_uart_Upgrade()
��    �ܣ���������
˵    ���� 
��    ��: 
��    �أ�
ע    �⣺
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
    NVIC_SystemReset(); // ���ϵͳ��λ
}
    
/*******************************************************************************
�������ƣ�Config_uart_GetVersion()
��    �ܣ��ϴ��汾
˵    ����
��    ��: 
��    �أ�
ע    �⣺
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
�������ƣ�Config_TimeSynchronization(UartUpData *RxUartData)
��    �ܣ�ʱ��ͬ��
˵    ����
��    ��: UartUpData *RxUartData:��������ͷ�豸ԴID������ݳ��ȵ�
��    �أ�
ע    �⣺
*******************************************************************************/
void  Config_TimeSynchronization(UartUpData *RxUartData)
{
    RTC_TimeTypeDef sTime;
    RTC_DateTypeDef DateToUpdate;
	
    RTC_DateType *timeddata = (RTC_DateType *)RxUartData->databuf;
	
    //����ʱ��
    sTime.Hours = timeddata->Hours;
    sTime.Minutes = timeddata->Minutes;
    sTime.Seconds = timeddata->Seconds;
    sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    sTime.StoreOperation = RTC_STOREOPERATION_RESET;
    HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	
	
    //��������
    DateToUpdate.WeekDay = timeddata->WeekDay;
    DateToUpdate.Month = timeddata->Month;
    DateToUpdate.Date = timeddata->Date;
    DateToUpdate.Year = timeddata->Year;  
	 
    HAL_RTC_SetDate(&hrtc,&DateToUpdate,RTC_FORMAT_BIN);

}

/*******************************************************************************
�������ƣ�Config_InpaperEnable(UartUpData *RxUartData)
��    �ܣ���ֽ��ʹ��
˵    ����
��    ��: UartUpData *RxUartData:��������ͷ�豸ԴID������ݳ��ȵ�
��    �أ�
ע    �⣺
*******************************************************************************/
void  Config_InpaperEnable(UartUpData *RxUartData)
{
    g_sys_sts.inpaper_mgr.inpaperenable = TRUE;
}

/*******************************************************************************
�������ƣ�Config_SetIdNumber(UartUpData *RxUartData)
��    �ܣ�ID������
˵    ����
��    ��: UartUpData *RxUartData:��������ͷ�豸ԴID������ݳ��ȵ�
��    �أ�
ע    �⣺
*******************************************************************************/
void  Config_SetIdNumber(UartUpData *RxUartData)
{
    // ʹ��STM32 Unique ID��Ϊ��Ϊ UUID������Ҫ�������á�
}

/*******************************************************************************
�������ƣ�Config_ClearNumber(UartUpData *RxUartData)
��    �ܣ���������
˵    ����
��    ��: UartUpData *RxUartData:��������ͷ�豸ԴID������ݳ��ȵ�
��    �أ�
ע    �⣺
*******************************************************************************/
void  Config_ClearNumber(UartUpData *RxUartData)
{
    Collect_init(); //�ɼ����³�ʼ��
}

/*******************************************************************************
�������ƣ�Config_Collect(UartUpData *RxUartData)
��    �ܣ��ɼ���������
˵    ����
��    ��: UartUpData *RxUartData:��������ͷ�豸ԴID������ݳ��ȵ�
��    �أ�
ע    �⣺
*******************************************************************************/
void  Config_Collect(UartUpData *RxUartData)
{
    memcpy(&g_sys_cfg.col_cfg ,(CollectCfg *)RxUartData->databuf,sizeof(CollectCfg));
    //д�뵽FALSH
    g_sys_cfg.enable = 1;
    /*��������������EEPROM�Ĵ浽FLASH,�������ݲ��ٱ���*/
    STMFLASH_Write(FLASH_ENV_ADDR,(u32*)&g_sys_cfg, sizeof(SysCfg));
}

/*******************************************************************************
�������ƣ�Get_Collect_Config()
��    �ܣ���ȡ�ɼ�����
˵    ����
��    ��: 
��    �أ�
ע    �⣺
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
�������ƣ�Get_Net_Config()
��    �ܣ���ȡ����
˵    ����
��    ��: 
��    �أ�
ע    �⣺
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
�������ƣ�Config_Net(UartUpData *RxUartData)
��    �ܣ���������
˵    ����
��    ��: UartUpData *RxUartData:��������ͷ�豸ԴID������ݳ��ȵ�
��    �أ�
ע    �⣺
*******************************************************************************/
void  Config_Net(UartUpData *RxUartData)
{
    char databuf[100];
    g_sys_sts.wifi_mgr.configenable = 1;
    NetCfg* netconfig = (NetCfg *)RxUartData->databuf;
    memset(&g_sys_cfg.net_cfg,0,sizeof(NetCfg));
    memcpy(&g_sys_cfg.net_cfg ,RxUartData->databuf,sizeof(NetCfg));
	
    g_sys_cfg.enable = 1;
    /*��������������EEPROM�Ĵ浽FLASH,�������ݲ��ٱ���*/
    STMFLASH_Write(FLASH_ENV_ADDR,(u32*)&g_sys_cfg, sizeof(SysCfg));
//
//	/*�ָ���������*/
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
	
    /*��������*/
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
    //����WiFiģ��
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
�������ƣ�Get_Out_Config()
��    �ܣ���ȡ�������
˵    ����
��    ��: 
��    �أ�
ע    �⣺
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
�������ƣ�Config_Out(UartUpData *RxUartData)
��    �ܣ��������
˵    ����
��    ��: UartUpData *RxUartData:��������ͷ�豸ԴID������ݳ��ȵ�
��    �أ�
ע    �⣺
*******************************************************************************/
void  Config_Out(UartUpData *RxUartData)
{
    g_sys_sts.wifi_mgr.configenable = 1;
    memset(&g_sys_cfg.out_cfg,0,sizeof(OutCfg));
    memcpy(&g_sys_cfg.out_cfg ,RxUartData->databuf,sizeof(OutCfg));
	
    g_sys_cfg.enable = 1;
    /*��������������EEPROM�Ĵ浽FLASH,�������ݲ��ٱ���*/
    STMFLASH_Write(FLASH_ENV_ADDR,(u32*)&g_sys_cfg, sizeof(SysCfg));
}

/*******************************************************************************
�������ƣ�Config_Uart_PackPcData(CommitCollectData *datainfo)
��    �ܣ��ύ�������������ǰ������Ϣ
˵    ����
��    ��: CommitCollectData *datainfo:���ӵ�GUID�� ������ʱ�� ͣ�� ��������
��    �أ�
ע    �⣺
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
�������ƣ�Config_UploadMonitorState(void)
��    �ܣ��ϱ���ǰ���״̬
˵    ����
��    ��:  
��    �أ�
ע    �⣺
*******************************************************************************/
void  Config_UploadMonitorState(void)
{
    if (g_sys_sts.cfg_mgr.upload_en == TRUE) { 
        g_sys_sts.cfg_mgr.upload_en = FALSE;
        memcpy(g_sys_sts.commit_data.guid , g_sys_cfg.dev_cfg.guid,sizeof(int8_t)*UUIDBUFLENGTH);                      //�ύ�ĺ���guid��
        //���ݴ��
        g_sys_sts.commit_data.alltime   = g_sys_sts.col_info.all_tm;     //������ʱ��
        g_sys_sts.commit_data.boottime  = g_sys_sts.col_info.bt_tm;        //����������ʱ��
        g_sys_sts.commit_data.stoptime  = g_sys_sts.col_info.st_tm;        //ͣ����ʱ��
        g_sys_sts.commit_data.faulttime = g_sys_sts.col_info.ft_tm;       //������ʱ��
				
        g_sys_sts.commit_data.bootstopsig = ((g_sys_sts.col_sts.bootsig !=0) && (g_sys_sts.col_sts.mainpulsesig!=0));       //�豸״̬��������ͣ��
        g_sys_sts.commit_data.faultsig = g_sys_sts.col_sts.faultsig;	            //�����ź�״̬	
        g_sys_sts.commit_data.pulsecount = g_sys_sts.col_sts.mainpulsecount;     //������
			 
        //2019��11��30��   �������ܵ�������
        g_sys_sts.commit_data.bladepulsesig = g_sys_sts.col_sts.bladepulsesig;     //����״̬
        g_sys_sts.commit_data.bladepulsecount = g_sys_sts.col_sts.bladepulsecount;     //��������

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
�������ƣ�Config_UploadTimeCount(void)
��    �ܣ�������Ϣ�ɼ����ݶ�ʱ�ϱ�
˵    ����
��    ��:  
��    �أ�
ע    �⣺
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

