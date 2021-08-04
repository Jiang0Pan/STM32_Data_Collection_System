
#include "system_config.h"
#include "maincounter.h"

/****************************************************************************
函数名称：_system_get_uid()
功	  能：获取STM32主芯片的unique ID
说	  明：获取96-bit unique ID,返回给上层处理过的unique id,16bytes
参    数: puuid，用于拷贝unique id的目标buf地址。
返	  回：
注	  意：
*****************************************************************************/
static void _system_getuuid(int8_t *puuid)
{
	int8_t uuid[UUIDBUFLENGTH]    = {0};
	int8_t uidbuf[UNIQUE_ID_SIZE] = {0};

	/* 
	   Unique ID Address:0x1FFF7A10, 96bit
	   e.g: 27 00 3E 00 19 50 4E 33 4B 35 39 20
	   UUDI: 2039354B 334E5019 003E0027
	*/
	memcpy(uidbuf,((uint8_t *)UID_BASE), UNIQUE_ID_SIZE);

	sprintf(uuid, "%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X", 
		uidbuf[0], uidbuf[1], uidbuf[2], uidbuf[3], 
		uidbuf[4], uidbuf[5], uidbuf[6], uidbuf[7], 
		uidbuf[8], uidbuf[9], uidbuf[10], uidbuf[11]);
	
	memcpy(puuid, uuid, sizeof(uuid));
}


//缓存数据加载
void system_loaddata(void)
{
    #if 0
	uint8_t   _state = TRUE;
	
	EEPROM_readRomData(EEPROM_FIRST_CHIP | EEPROM_DATA,
		              (u8 *)&g_sys_sts.realtime_data,sizeof(RealTimeStore));

	if(g_sys_sts.realtime_data.bootstopsig != 0xff)
	{
		_state = Collect_Judge_ClearData(g_sys_sts.realtime_data.MachineCurrent);
		if(_state == FALSE) //不需要初始化
		{
			g_sys_sts.col_info.all_tm = g_sys_sts.realtime_data.alltime;
			g_sys_sts.col_info.bt_tm = g_sys_sts.realtime_data.bootsigtime;
			g_sys_sts.col_info.ft_tm = g_sys_sts.realtime_data.faultsigtime;
			g_sys_sts.col_info.st_tm    = g_sys_sts.realtime_data.stopsigtime;
			g_sys_sts.col_sts.mainpulsecount  = g_sys_sts.realtime_data.pulsecount;
			g_sys_sts.col_sts.bladepulsecount = g_sys_sts.realtime_data.bladepulsecount;
			g_sys_sts.commit_data.dataindex            = g_sys_sts.realtime_data.dataindex;
		}
		else
		{
			g_sys_sts.col_info.cl_en = TRUE;//上传清除数据

		}
	}
    #endif
}
//系统启动初始化
void system_init(void)
{
    /*仅将环境变量从EEPROM改存到FLASH,其他数据不再保存*/
	STMFLASH_Read(FLASH_ENV_ADDR,(u32*)&g_sys_cfg, sizeof(SysCfg));

	if(g_sys_cfg.enable != 1) {
		memset(&g_sys_cfg,0,sizeof(SysCfg));
		Collect_config();//采集初始化
		WIFI_net_init(); //WIFI网络初始化，暂时不做任何处理
		g_sys_cfg.enable = 1;
		/*仅将环境变量从EEPROM改存到FLASH,其他数据不再保存*/
		STMFLASH_Write(FLASH_ENV_ADDR,(u32*)&g_sys_cfg, sizeof(SysCfg));
	}
    
	memset(&g_sys_cfg.dev_cfg,0,sizeof(DevCfg));
	_system_getuuid(g_sys_cfg.dev_cfg.guid);//在其他配置项启用时写入eeprom
	memset(&g_sys_sts,0,sizeof(SysSts)); //系统状态初始化
	mallco_dev.init(); //内存初始化
	Outcontrol_Init();//插纸机初始化
	Collect_init();//采集初始化
	collect_init_paper_cnt();
	Config_Init();//配置初始化
	WIFI_uart_init();//串口初始化
}
