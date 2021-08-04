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
�������ƣ�AD5761_1_ioconfig
��    �ܣ�AD5761_1IO����.
˵    ����
��    ��:
��    ��: 
ע    �⣺
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
�������ƣ�AD5761_1_delay
��    �ܣ�AD5761_1��ʱ.
˵    ����
��    ��:num:��ʱʱ��
��    ��: 
ע    �⣺
*******************************************************************************/
void AD5761_1_delay(int num)
{
	int i = 0;
	for (i = 0;i < num;i++)
		;
}

/*******************************************************************************
�������ƣ�AD5761_1_init
��    �ܣ�AD5761_1��ʼ��.
˵    ����
��    ��:
��    ��: 
ע    �⣺
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
�������ƣ�AD5761_1_reset
��    �ܣ�AD5761_1��λ.
˵    ����
��    ��:
��    ��: 
ע    �⣺
*******************************************************************************/
void  AD5761_1_reset(void)
{
	AD5761_1_delay(1);
	AD5761_1_RESET_LOW();
	AD5761_1_delay(1); 
	AD5761_1_RESET_HIGH();
}

/*******************************************************************************
�������ƣ�AD5761_1_config
��    �ܣ�AD5761_1��ʼ������.
˵    ����
��    ��:
��    ��: 
ע    �⣺
*******************************************************************************/
void  AD5761_1_config(void)
{
	AD5761_1_write(ALLRESET,0x0000);
	AD5761_1_write(CONFIGREG,ADCONFIG);//���ÿ��ƼĴ���
	//AD5761_1_reset();
}

/*******************************************************************************
�������ƣ�AD5761_1_write
��    �ܣ�AD5761_1д������
˵    ����
��    ��:addr:��ַ
        data��д�������
��    ��: 
ע    �⣺
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
�������ƣ�AD5761_1_read
��    �ܣ�AD5761_1��ȡ����
˵    ����
��    ��:addr����ַ
��    ��: ����ֵ�����ض�ȡ������
ע    �⣺
*******************************************************************************/
u16  AD5761_1_read(u8 addr)
{ 
   return 0;
}

/*******************************************************************************
�������ƣ�TIM4_IRQHandler
��    �ܣ��������-1
˵    ����
��    ��:
��    ��: 
ע    �⣺
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
�������ƣ�AD5761_2_ioconfig
��    �ܣ�AD5761_2IO����
˵    ����
��    ��:
��    ��: 
ע    �⣺
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
�������ƣ�AD5761_2_delay
��    �ܣ�AD5761_2��ʱ
˵    ����
��    ��:
��    ��: 
ע    �⣺
*******************************************************************************/
void AD5761_2_delay(int num)
{
	int i = 0;
	for (i=0;i<num;i++)
		;
}

/*******************************************************************************
�������ƣ�AD5761_2_init
��    �ܣ�AD5761_2��ʼ��
˵    ����
��    ��:
��    ��: 
ע    �⣺
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
�������ƣ�AD5761_2_reset
��    �ܣ�AD5761_2��λ
˵    ����
��    ��:
��    ��: 
ע    �⣺
*******************************************************************************/
void  AD5761_2_reset(void)
{
	AD5761_2_delay(1);
	AD5761_2_RESET_LOW();
	AD5761_2_delay(1); 
	AD5761_2_RESET_HIGH();
}

/*******************************************************************************
�������ƣ�AD5761_2_config
��    �ܣ�AD5761_2��ʼ������
˵    ����
��    ��:
��    ��: 
ע    �⣺
*******************************************************************************/
void  AD5761_2_config(void)
{
	AD5761_2_write(ALLRESET,0x0000);
	AD5761_2_write(CONFIGREG,ADCONFIG);/* ���ÿ��ƼĴ��� */
	//AD5761_2_reset();
}

/*******************************************************************************
�������ƣ�AD5761_2_write
��    �ܣ�AD5761_2д������
˵    ����
��    ��:addr:Ҫд��ĵ�ַ
		data:Ҫд�������
��    ��: 
ע    �⣺
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
�������ƣ�AD5761_2_read
��    �ܣ�AD5761_2��ȡ����
˵    ����
��    ��:addr:��ȡ���ݵĵ�ַ
��    ��: ����ֵ����ȡ������
ע    �⣺
*******************************************************************************/
u16  AD5761_2_read(u8 addr)
{  
	return 0;
}

/*******************************************************************************
�������ƣ�TIM5_IRQHandler
��    �ܣ��������-2 ��Ҫ�޸�
˵    ����
��    ��:
��    ��: 
ע    �⣺
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
�������ƣ�SigOut_Handle
��    �ܣ���׼�ź�����ж�
˵    ����
��    ��:
��    ��: 
ע    �⣺
*******************************************************************************/
void SigOut_Handle(void)
{
	/* ָʾ������ */
	if ((sysinfo.sigoutmgr1.infoenable == 2) || 
			(sysinfo.sigoutmgr2.infoenable == 2)) { 	
		Load_LED3_State(LED_GREEN);
	}		  
	else {
		Load_LED3_State(LED_OFF);
	}
}

/************************ (C) COPYRIGHT Hopewellgroup ***********END OF FILE***/

