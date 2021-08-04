/**
  ******************************************************************************
  * @file           : modulebus.c
  * @brief          : moduledus mangement 
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
函数名称：AD5761_1_ioconfig
功    能：AD5761_1IO配置.
说    明：
参    数:
返    回: 
注    意：
*******************************************************************************/
void  AD5761_1_ioconfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	//AD5761_1_CLEAR/AD5761_1_RESET/AD5761_1_LDAC
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOG, &GPIO_InitStructure);	

	//AD5761_1_SCLK/AD5761_1_SYNC/AD5761_1_SDI

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//AD5761_1_ALERT
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOG, &GPIO_InitStructure);

	//AD5761_1_SDO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/*******************************************************************************
函数名称：AD5761_1_delay
功    能：AD5761_1延时.
说    明：
参    数:num:延时时间
返    回: 
注    意：
*******************************************************************************/
void AD5761_1_delay(int num)
{
	int i = 0;
	for (i = 0;i < num;i++)
		;
}

/*******************************************************************************
函数名称：AD5761_1_init
功    能：AD5761_1初始化.
说    明：
参    数:
返    回: 
注    意：
*******************************************************************************/
void  AD5761_1_init(void)
{
	
	AD5761_1_ioconfig();

	AD5761_1_RESET_LOW();
	AD5761_1_delay(1);

	AD5761_1_RESET_HIGH();
	AD5761_1_delay(1);

	AD5761_1_CLEAR_LOW();
	AD5761_1_delay(1); 

	AD5761_1_CLEAR_HIGH();
	AD5761_1_LDAC_LOW();
}

/*******************************************************************************
函数名称：AD5761_1_reset
功    能：AD5761_1复位.
说    明：
参    数:
返    回: 
注    意：
*******************************************************************************/
void  AD5761_1_reset(void)
{
	AD5761_1_delay(1);
	AD5761_1_RESET_LOW();
	AD5761_1_delay(1); 
	AD5761_1_RESET_HIGH();
}

/*******************************************************************************
函数名称：AD5761_1_config
功    能：AD5761_1初始化配置.
说    明：
参    数:
返    回: 
注    意：
*******************************************************************************/
void  AD5761_1_config(void)
{
	AD5761_1_write(ALLRESET,0x0000);
	AD5761_1_write(CONFIGREG,ADCONFIG);//配置控制寄存器
	//AD5761_1_reset();
}

/*******************************************************************************
函数名称：AD5761_1_write
功    能：AD5761_1写入数据
说    明：
参    数:addr:地址
        data：写入的数据
返    回: 
注    意：
*******************************************************************************/
void  AD5761_1_write(u8 addr,u16 data)
{
	int i = 0;
	u32 index = 0x80000000;
	u32 _data = addr;
	_data = (_data << 16) & 0x00ff0000;
	_data += data;
	_data = (_data << 8) & 0xffffff00;
	AD5761_1_SYNC_LOW();
	for (i = 0;i < 24;i++) {
		AD5761_1_CLK_HIGH();
		if (_data&index) {
			AD5761_1_SDI_HIGH();
		}
		else {
			AD5761_1_SDI_LOW();
		}
		AD5761_1_delay(1);
		AD5761_1_CLK_LOW();
		AD5761_1_delay(1); 
		index >>= 1;
	}
	AD5761_1_CLK_HIGH();
	AD5761_1_SDI_HIGH();
	AD5761_1_SYNC_HIGH();
}

/*******************************************************************************
函数名称：AD5761_1_read
功    能：AD5761_1读取数据
说    明：
参    数:addr：地址
返    回: 返回值：返回读取的数据
注    意：
*******************************************************************************/
u16  AD5761_1_read(u8 addr)
{ 
   return 0;
}

/*******************************************************************************
函数名称：TIM4_IRQHandler
功    能：输出波形-1
说    明：
参    数:
返    回: 
注    意：
*******************************************************************************/
void TIM4_IRQHandler(void)
{
	static u8 state = 0;
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) {
		TIM_ClearITPendingBit(TIM4 , TIM_IT_Update);
		if (sysinfo.sigoutmgr1.infoenable == 2) {
			if(state == 0) {
				AD5761_1_write(DATACONFIG,sysinfo.sigoutmgr1.hignampvalue);
				state = 1;
			}
			else {
				AD5761_1_write(DATACONFIG,sysinfo.sigoutmgr1.lowampvalue);
				state = 0;
			}  
		}
		else if	(sysinfo.sigoutmgr1.infoenable == 0) {   
			AD5761_1_write(DATACONFIG,0);
			sysinfo.sigoutmgr1.infoenable = 3;
			TIM_Cmd(TIM4, DISABLE);
		}
		else {

		}				 
	}
}

/*******************************************************************************
函数名称：AD5761_2_ioconfig
功    能：AD5761_2IO配置
说    明：
参    数:
返    回: 
注    意：
*******************************************************************************/
void  AD5761_2_ioconfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	//AD5761_2_CLEAR/AD5761_2_RESET
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOG, &GPIO_InitStructure);	

	//AD5761_2_LDAC
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);	

	//AD5761_2_SCLK/AD5761_2_SYNC/AD5761_2_SDI

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//AD5761_2_ALERT
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOG, &GPIO_InitStructure);

	//AD5761_2_SDO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/*******************************************************************************
函数名称：AD5761_2_delay
功    能：AD5761_2延时
说    明：
参    数:
返    回: 
注    意：
*******************************************************************************/
void AD5761_2_delay(int num)
{
	int i = 0;
	for (i=0;i<num;i++)
		;
}

/*******************************************************************************
函数名称：AD5761_2_init
功    能：AD5761_2初始化
说    明：
参    数:
返    回: 
注    意：
*******************************************************************************/
void  AD5761_2_init(void)
{
	AD5761_2_ioconfig();

	AD5761_2_RESET_LOW();
	AD5761_2_delay(1);

	AD5761_2_RESET_HIGH();
	AD5761_2_delay(1);

	AD5761_2_CLEAR_LOW();
	AD5761_2_delay(1); 

	AD5761_2_CLEAR_HIGH();
	AD5761_2_LDAC_LOW();
}

/*******************************************************************************
函数名称：AD5761_2_reset
功    能：AD5761_2复位
说    明：
参    数:
返    回: 
注    意：
*******************************************************************************/
void  AD5761_2_reset(void)
{
	AD5761_2_delay(1);
	AD5761_2_RESET_LOW();
	AD5761_2_delay(1); 
	AD5761_2_RESET_HIGH();
}

/*******************************************************************************
函数名称：AD5761_2_config
功    能：AD5761_2初始化配置
说    明：
参    数:
返    回: 
注    意：
*******************************************************************************/
void  AD5761_2_config(void)
{
	AD5761_2_write(ALLRESET,0x0000);
	AD5761_2_write(CONFIGREG,ADCONFIG);/* 配置控制寄存器 */
	//AD5761_2_reset();
}

/*******************************************************************************
函数名称：AD5761_2_write
功    能：AD5761_2写入数据
说    明：
参    数:addr:要写入的地址
		data:要写入的数据
返    回: 
注    意：
*******************************************************************************/
void  AD5761_2_write(u8 addr,u16 data)
{
	int i = 0;
	u32 index = 0x80000000;
	u32 _data = addr;
	_data = (_data << 16) & 0x00ff0000;
	_data += data;
	_data = (_data << 8) & 0xffffff00;
	AD5761_2_SYNC_LOW();
	for (i = 0;i < 24;i++) {
		AD5761_2_CLK_HIGH();
		if (_data&index) {
			AD5761_2_SDI_HIGH();
		}
		else {
			AD5761_2_SDI_LOW();
		}
		AD5761_2_delay(1);
		AD5761_2_CLK_LOW();
		AD5761_2_delay(1); 
		index >>= 1;
	}
	AD5761_2_CLK_HIGH();
	AD5761_2_SDI_HIGH();
	AD5761_2_SYNC_HIGH();
}

/*******************************************************************************
函数名称：AD5761_2_read
功    能：AD5761_2读取数据
说    明：
参    数:addr:读取数据的地址
返    回: 返回值：读取的数据
注    意：
*******************************************************************************/
u16  AD5761_2_read(u8 addr)
{  
	return 0;
}

/*******************************************************************************
函数名称：TIM5_IRQHandler
功    能：输出波形-2 需要修改
说    明：
参    数:
返    回: 
注    意：
*******************************************************************************/
void TIM5_IRQHandler(void)
{
	static u8 state = 0;
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET) {
		TIM_ClearITPendingBit(TIM5 , TIM_IT_Update);
		if (sysinfo.sigoutmgr2.infoenable == 2) {
			if (state == 0) {
				AD5761_2_write(DATACONFIG,sysinfo.sigoutmgr2.hignampvalue);
				state = 1;
			}
			else {
				AD5761_2_write(DATACONFIG,sysinfo.sigoutmgr2.lowampvalue);
				state = 0;
			}  
		}
		else if (sysinfo.sigoutmgr2.infoenable == 0) {   
			AD5761_2_write(DATACONFIG,0);
			sysinfo.sigoutmgr2.infoenable = 3;
			TIM_Cmd(TIM5, DISABLE);
		}
		else {

		}				 
	}
}


/*******************************************************************************
函数名称：SigOut_Handle
功    能：标准信号输出判断
说    明：
参    数:
返    回: 
注    意：
*******************************************************************************/
void SigOut_Handle(void)
{
	/* 指示灯设置 */
	if ((sysinfo.sigoutmgr1.infoenable == 2) || 
			(sysinfo.sigoutmgr2.infoenable == 2)) { 	
		Load_LED3_State(LED_GREEN);
	}		  
	else {
		Load_LED3_State(LED_OFF);
	}
}

/************************ (C) COPYRIGHT Hopewellgroup ***********END OF FILE***/

