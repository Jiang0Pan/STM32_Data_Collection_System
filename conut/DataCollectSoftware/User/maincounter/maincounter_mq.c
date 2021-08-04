/**
  ******************************************************************************
  * @file           : maincounter.h
  * @brief          : main counter
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

#include "maincounter.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

static 	 MainCnt     maincnt;
static   U32 		 num = 0;
#define usTicks   (SystemCoreClock / 1000000)
extern __IO U32 uwTick;

/*******************************************************************************
�������ƣ�clear_buffer
��    �ܣ������������
˵    ����
��    ��:
��    ��: 
ע    �⣺
*******************************************************************************/
static void clear_buffer(void)
{
	printf("���ڵı�������:%d\n",
			maincnt.odests.dppcnt);
	maincnt.odests.dppcnt = 0;
}

/*******************************************************************************
�������ƣ�getsystime_us()
��    �ܣ���ȡϵͳʱ��(us)
˵    ����
��    ��:
��    ��: ����U64�������͵�ϵͳʱ��
ע    �⣺
*******************************************************************************/
static U64 getsystime_us(void) 
{
	register U32 ms, cycle_cnt;
	do {
		ms = uwTick;
		cycle_cnt = SysTick->VAL;
	} while (ms != uwTick); 

	return ((ms * 1000) + 1000 - cycle_cnt / usTicks);
}

/*******************************************************************************
�������ƣ�Collect_Paper_Init
��    �ܣ�������ʼ��������ת��Ĭ�ϱ���������Ϊ2
˵    ����
��    ��:
��    ��: 
ע    �⣺
*******************************************************************************/
void Collect_Paper_Init(void)
{
	maincnt.empty_count = SECOND;
}

/*******************************************************************************
�������ƣ�Collext_Paper_Clear
��    �ܣ���յ�ǰ�ļ���ֵ
˵    ����
��    ��:
��    ��: 
ע    �⣺
*******************************************************************************/
void Collect_Paper_Clear(void)
{
	maincnt.paper_count = ZERO;
	num = 0;
}

/*******************************************************************************
�������ƣ�Collect_Get_Tooth_Interrupt_countval
��    �ܣ���ȡ�����жϣ������Ŵ���һ����Ϊһ������
˵    ����
��    ��:
��    ��: 
ע    �⣺
*******************************************************************************/
void Collect_Get_Tooth_Interrupt_countval(void)
{
	/* ����־λ��λ */
	maincnt.tooth_row_count_flag = FIRST;
	
	/* �µ��������ˣ����浽�ɵĻ������� */
	memcpy(maincnt.old_paper_edge,
			maincnt.current_paper_edge,
			sizeof(maincnt.current_paper_edge));
	memset(maincnt.current_paper_edge,
			ZERO,
			sizeof(maincnt.current_paper_edge));
	maincnt.old_paper_period_count =
			maincnt.current_paper_period_count;
	maincnt.current_paper_period_count = ZERO;
//	maincnt.old_paper_period_count = 
//			maincnt.current_paper_period_count;
//    maincnt.current_paper_period_count = ZERO;
//	maincnt.old_paper_edge[ZERO] =
//			maincnt.current_paper_edge[ZERO];
//	maincnt.current_paper_edge[ZERO] = ZERO;
//    maincnt.old_paper_edge[FIRST] =
//			maincnt.current_paper_edge[FIRST];
//	maincnt.current_paper_edge[FIRST] = ZERO;

	/* ����ж�ʱ�� */
	maincnt.intterrupt_time = GetSysTime_us();

}

/*******************************************************************************
�������ƣ�Collect_Get_Paper_Interrupt_countval
��    �ܣ���ȡֽ�жϴ���
˵    ����
��    ��:
��    ��: 
ע    �⣺
*******************************************************************************/
void Collect_Get_Paper_Interrupt_countval(void)
{
	if (maincnt.current_paper_period_count < 20) {
		maincnt.current_paper_edge[maincnt.current_paper_period_count] = 
			READ_RESERVEDPULSE_EDGE;
	}

	/* ����ǰ���ڵı�������++ */
	maincnt.current_paper_period_count++;	
	

//	/* ��õ�һ�����صĵ�ƽֵ */
//    if (FIRST == maincnt.current_paper_period_count) {
//		maincnt.current_paper_edge[ZERO] = 
//				READ_RESERVEDPULSE_EDGE;
//	}
//	/* ��õڶ������صĵ�ƽֵ */
//	else if (SECOND == maincnt.current_paper_period_count) {
//		maincnt.current_paper_edge[FIRST] = 
//				READ_RESERVEDPULSE_EDGE;	
//	} 
//	/* ����������账�� */
//	else {
//		
//	}
}

/*******************************************************************************
�������ƣ�Collect_Get_Paper_countval
��    �ܣ�����״̬�жϡ�����
˵    ����
��    ��:
��    ��: ���ؼ���ֵ
ע    �⣺
*******************************************************************************/
U32 Collect_Get_Paper_countval(void)
{
	/* ����״̬���ж� */
	if (maincnt.device_current_status == RUNSTATUS) {
		/* ������־λ����λ */
		if (maincnt.tooth_row_count_flag) {
			/* ��λ��־λ */
			maincnt.tooth_row_count_flag = ZERO;
			/* >2 һ����ֽ */
			if (maincnt.old_paper_period_count > 
					maincnt.empty_count) {
				maincnt.paper_count++;
				maincnt.device_old_status = RUNSTATUS;
				maincnt.device_current_status = RUNSTATUS;
				Clear_Cache();
			}
			/* ==2 �����ж��Ƿ���ֽ */
			else if (maincnt.old_paper_period_count == 
					maincnt.empty_count) {
				maincnt.device_old_status = RUNSTATUS;
				/* �������غ��½���(��MCU��˵)����ֽ */
				if ((maincnt.old_paper_edge[ZERO] == FIRST) &&
						(maincnt.old_paper_edge[FIRST] == ZERO)) {
					maincnt.paper_count++;
					maincnt.device_current_status = RUNSTATUS;
				}
				/* ���½��غ�������(��MCU��˵)����ֽ */		
				else {
					maincnt.device_current_status = EMPTYSTATUS;
				}
				Clear_Cache();
			}
			/* ������������账�� */
			else {
				
			}
		}
		/* ��ʱ���ж�Ϊͣ�� */
		maincnt.current_time = GetSysTime_us();
		if (labs(maincnt.current_time - 
				maincnt.intterrupt_time) > TIMEOUT_) {
			maincnt.device_old_status = RUNSTATUS;
			maincnt.device_current_status = STOPSTATUS;
		}
	}
	/* ��ת״̬�ж� */
	else if (maincnt.device_current_status == EMPTYSTATUS) {
		/* ������־λ����λ */ 
		 if (maincnt.tooth_row_count_flag) {
			/* ��λ��־λ */
			maincnt.tooth_row_count_flag = ZERO;
			/* >2 һ����ֽ */
			if (maincnt.old_paper_period_count > 
					maincnt.empty_count) {
				maincnt.device_old_status = EMPTYSTATUS;
		   /* ���ڹ���ֽ��罫ֽ�ֳ�2�����ֵĺ��µĶ���� */
				if(maincnt.old_paper_period_count == THREE) {
					num++;
					maincnt.device_current_status = RUNSTATUS;
					Clear_Cache();
				}
				else {	  
					maincnt.paper_count++;
					maincnt.device_current_status = RUNSTATUS;
					Clear_Cache();
				}
//					maincnt.paper_count++;
//					maincnt.device_old_status = EMPTYSTATUS;
//					maincnt.device_current_status = RUNSTATUS;
//					Clear_Cache();
			}
			/* ==2 �����ж��Ƿ���ֽ */
			else if (maincnt.old_paper_period_count == 
					maincnt.empty_count) {
				maincnt.device_old_status = EMPTYSTATUS;
				/* �������غ��½���(��MCU��˵)����ֽ */
				if ((maincnt.old_paper_edge[ZERO] == FIRST) &&
						(maincnt.old_paper_edge[FIRST] == ZERO)) {
					maincnt.paper_count++;
					maincnt.device_current_status = RUNSTATUS;
				}
				/* ���½��غ�������(��MCU��˵)����ֽ */		
				else {
					maincnt.device_current_status = EMPTYSTATUS;
				}
				Clear_Cache();
			}
			/* ������������账�� */
			else {
				
			}
		}
		/* ��ʱ���ж�Ϊͣ�� */
		maincnt.current_time = GetSysTime_us();
		if (labs(maincnt.current_time - 
			maincnt.intterrupt_time) > TIMEOUT_) {
			maincnt.device_old_status = EMPTYSTATUS;
			maincnt.device_current_status = STOPSTATUS;
		}
	}
	/* ͣ��״̬�ж� */	
    else if (maincnt.device_current_status == STOPSTATUS) {
		/* ����豸��һ��״̬Ϊ���л�ͣ��״̬ */
		if (maincnt.tooth_row_count_flag) {
			maincnt.tooth_row_count_flag = ZERO;
			/* >2һ����ֽ */
			if (maincnt.old_paper_period_count > 
					maincnt.empty_count) { 	
				maincnt.paper_count++;
				maincnt.device_old_status = STOPSTATUS;
				maincnt.device_current_status = RUNSTATUS;
				Clear_Cache();
			}
		    /* ==2 �����ж��Ƿ���ֽ */
			else if (maincnt.old_paper_period_count == 
					maincnt.empty_count) {
				maincnt.device_old_status = STOPSTATUS;
				/* �������غ��½���(��MCU��˵)����ֽ */
				if ((maincnt.old_paper_edge[ZERO] == FIRST) &&
						(maincnt.old_paper_edge[FIRST] == ZERO)) {
					maincnt.paper_count++;
					maincnt.device_current_status = RUNSTATUS;
				}
				/* ���½��غ�������(��MCU��˵)����ֽ */		
				else {
					maincnt.device_current_status = EMPTYSTATUS;
				}
				Clear_Cache();
			}
			/* ������������账�� */
			else {
				
			}
		}
	}
	return maincnt.paper_count;
}

/*******************************************************************************
�������ƣ�AD5761_1_ioconfig
��    �ܣ�AD5761_1IO����.
˵    ����
��    ��:
��    ��: 
ע    �⣺
*******************************************************************************/
static void Collect_Status_Switch(U8 devicecurrentstatus)
{
	
}

/*******************************************************************************
�������ƣ�Clear_Cache
��    �ܣ������������
˵    ����
��    ��:
��    ��: 
ע    �⣺
*******************************************************************************/
static void Clear_Cache(void)
{
	U8 temp[300] = {0};
	for (U8 num = 0;num < maincnt.old_paper_period_count; num++) {
		sprintf(&temp[strlen(temp)],"p_edge[%d]:%d\n",num,
				maincnt.old_paper_edge[num]);
	}
	printf("��ǰ����״̬��%d\n���ڵı�������:%d\n%snum=%d\n",
			maincnt.device_current_status,
			maincnt.old_paper_period_count,temp,num);
	maincnt.old_paper_period_count = ZERO;
	memset(maincnt.old_paper_edge,
		ZERO,
		sizeof(maincnt.old_paper_edge));
}

/*******************************************************************************
�������ƣ�GetSysTime_us()
��    �ܣ���ȡϵͳʱ��(us)
˵    ����
��    ��:
��    ��: ����U64�������͵�ϵͳʱ��
ע    �⣺
*******************************************************************************/
static U64 GetSysTime_us(void) 
{
	register U32 ms, cycle_cnt;
	do {
		ms = uwTick;
		cycle_cnt = SysTick->VAL;
	} while (ms != uwTick); 

	return ((ms * 1000) + 1000 - cycle_cnt / usTicks);
}

/************************ (C) COPYRIGHT Hopewellgroup ***********END OF FILE***/

