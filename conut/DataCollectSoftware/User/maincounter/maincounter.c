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
#include "math.h"
#include "stdio.h"

static 	 MainCnt     maincnt;

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
�������ƣ�collect_init_paper_cnt
��    �ܣ�������ʼ��������ת��Ĭ�ϱ���������Ϊ2
˵    ����
��    ��:
��    ��: 
ע    �⣺
*******************************************************************************/
void collect_init_paper_cnt(void)
{
	maincnt.ecnt = EMPTY_DEFINE;
	maincnt.pcnt = 0;
}

/*******************************************************************************
�������ƣ�collect_clear_paper_cnt
��    �ܣ���յ�ǰֽ�ŵļ���ֵ
˵    ����
��    ��:
��    ��: 
ע    �⣺
*******************************************************************************/
void collect_clear_paper_cnt(void)
{
	maincnt.pcnt = 0;
}

/*******************************************************************************
�������ƣ�collect_set_tooth_int_cnt
��    �ܣ���ȡ�����жϣ������Ŵ���һ����Ϊһ������
˵    ����
��    ��:
��    ��: 
ע    �⣺
*******************************************************************************/
void collect_set_tooth_int_cnt(void)
{
	/* ����־λ��λ */
	maincnt.tooth_row_count_flag = SET_FLAG;
	
	/* �µ��������ˣ����浽�ɵĻ������� */
	maincnt.odests.dppcnt = 
			maincnt.cdests.dppcnt;
    maincnt.cdests.dppcnt = 0;

	/* ����ж�ʱ�� */
	maincnt.inttime = getsystime_us();

}

/*******************************************************************************
�������ƣ�collect_set_paper_int_cnt
��    �ܣ���ȡֽ�жϴ���
˵    ����
��    ��:
��    ��: 
ע    �⣺
*******************************************************************************/
void collect_set_paper_int_cnt(void)
{
	/* ����ǰ���ڵı�������++ */
	maincnt.cdests.dppcnt++;	
}

/*******************************************************************************
�������ƣ�collect_get_paper_cnt
��    �ܣ�����״̬�жϡ�����(�����)
˵    ����
��    ��:
��    ��: ���ؼ���ֵ
ע    �⣺
*******************************************************************************/
U32 collect_get_paper_cnt(void)
{
    /* ����״̬���ж� */
    if (maincnt.cdests.dsts == RUNSTATUS) {
		/* ������־λ����λ */
        if (maincnt.tooth_row_count_flag) {
			/* ��λ��־λ */
		    maincnt.tooth_row_count_flag = RESET_FLAG;
			/* >=2 һ����ֽ */
	        if (maincnt.odests.dppcnt >= 
	                maincnt.ecnt) {
				maincnt.pcnt++;
				maincnt.odests.dsts = RUNSTATUS;
			    maincnt.cdests.dsts = RUNSTATUS;
				clear_buffer();
	        }
			/* �����������ֽ */
	        else {
				maincnt.odests.dsts = RUNSTATUS;
			    maincnt.cdests.dsts = EMPTYSTATUS;
				clear_buffer();
	        }
        }
		/* ��ʱ���ж�Ϊͣ�� */
        maincnt.curtime = getsystime_us();
        if (labs(maincnt.curtime - 
                maincnt.inttime) > TIMEOUT_) {
            maincnt.odests.dsts = RUNSTATUS;
            maincnt.cdests.dsts = STOPSTATUS;
        }
    }
	/* ��ת״̬�ж� */
    else if (maincnt.cdests.dsts == EMPTYSTATUS) {
		/* ������־λ����λ */ 
         if (maincnt.tooth_row_count_flag) {
			/* ��λ��־λ */
		    maincnt.tooth_row_count_flag = RESET_FLAG;
		    /* >2 һ����ֽ */
            if (maincnt.odests.dppcnt >= 
                    maincnt.ecnt) {
					maincnt.pcnt++;
					maincnt.odests.dsts = EMPTYSTATUS;
					maincnt.cdests.dsts = RUNSTATUS;
					clear_buffer();
            }
		    /* �����������ֽ  */
            else {
				maincnt.odests.dsts = EMPTYSTATUS;
			    maincnt.cdests.dsts = EMPTYSTATUS;
				clear_buffer();
            }
        }
	    /* ��ʱ���ж�Ϊͣ�� */
        maincnt.curtime = getsystime_us();
        if (labs(maincnt.curtime - 
            maincnt.inttime) > TIMEOUT_) {
            maincnt.odests.dsts = EMPTYSTATUS;
            maincnt.cdests.dsts = STOPSTATUS;
        }
    }
	/* ͣ��״̬�ж� */	
    else if (maincnt.cdests.dsts == STOPSTATUS) {
		/* ����豸��һ��״̬Ϊ���л�ͣ��״̬ */
		if (maincnt.tooth_row_count_flag) {
			maincnt.tooth_row_count_flag = RESET_FLAG;
			/* >2һ����ֽ */
			if (maincnt.odests.dppcnt >= 
					maincnt.ecnt) { 	
				maincnt.pcnt++;
				maincnt.odests.dsts = STOPSTATUS;
				maincnt.cdests.dsts = RUNSTATUS;
				clear_buffer();
			}
		    /* �����������ֽ  */
			else {
				maincnt.odests.dsts = STOPSTATUS;
			    maincnt.cdests.dsts = EMPTYSTATUS;
				clear_buffer();		
			}  
		}
	}
	return maincnt.pcnt;
}

/************************ (C) COPYRIGHT Hopewellgroup ***********END OF FILE***/

