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

static 	 maincnt     maincnt;
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
void Collext_Paper_Clear(void)
{
	maincnt.paper_count = ZERO;
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
	maincnt.old_paper_period_count = 
			maincnt.current_paper_period_count;
    maincnt.current_paper_period_count = ZERO;
	maincnt.old_paper_edge[ZERO] =
			maincnt.current_paper_edge[ZERO];
	maincnt.current_paper_edge[ZERO] = ZERO;
    maincnt.old_paper_edge[FIRST] =
			maincnt.current_paper_edge[FIRST];
	maincnt.current_paper_edge[FIRST] = ZERO;

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
	/* ����ǰ���ڵı�������++ */
	maincnt.current_paper_period_count++;	
	
	/* ��õ�һ�����صĵ�ƽֵ */
    if (FIRST == maincnt.current_paper_period_count) {
		maincnt.current_paper_edge[ZERO] = 
				READ_RESERVEDPULSE_EDGE;
	}
	/* ��õڶ������صĵ�ƽֵ */
	else if (SECOND == maincnt.current_paper_period_count) {
		maincnt.current_paper_edge[FIRST] = 
				READ_RESERVEDPULSE_EDGE;	
	} 
	/* ����������账�� */
	else {
		
	}
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
           /* ���ڹ���ֽ��罫ֽ�ֳ�2�����ֵĺ��µĶ���� */
//                if(maincnt.old_paper_period_count <= FIVE) {
//					maincnt.old_paper_period_count = ZERO;
//					maincnt.device_old_status = EMPTYSTATUS;
//					maincnt.device_current_status = RUNSTATUS;
//					maincnt.old_paper_edge[ZERO] = ZERO;
//					maincnt.old_paper_edge[FIRST] = ZERO;
//				}
//				else {
//	                maincnt.old_paper_period_count = ZERO;      
//					maincnt.paper_count++;
//					maincnt.device_old_status = EMPTYSTATUS;
//					maincnt.device_current_status = RUNSTATUS;
//					maincnt.old_paper_edge[ZERO] = ZERO;
//					maincnt.old_paper_edge[FIRST] = ZERO;
//					}
					maincnt.paper_count++;
					maincnt.device_old_status = EMPTYSTATUS;
					maincnt.device_current_status = RUNSTATUS;
					Clear_Cache();
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
		if ((maincnt.device_old_status == RUNSTATUS) ||
				(maincnt.device_old_status == STOPSTATUS)) {
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
				/* ==2���н�һ���ж� */
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
				else {
						
				}  
			}
	    }
		/* ���ϴ��豸״̬Ϊ��ת״̬����Ҫ�������⴦�� */
		else if (maincnt.device_old_status == EMPTYSTATUS) {
			maincnt.current_time = GetSysTime_us();
			/* ת��С��2400ת/h */
			if (labs(maincnt.current_time - 
					maincnt.intterrupt_time) > 1500000) {
				maincnt.tooth_row_count_flag = ZERO;
				maincnt.current_paper_period_count = ZERO;
				Clear_Cache();
			}
			else {
			    /* ��λ��ǰ���� */
				static U8 reset_flag = FIRST_VAL;
				if (reset_flag) {
					if(maincnt.current_paper_period_count == 
							maincnt.empty_count) {
						maincnt.tooth_row_count_flag = ZERO;
						maincnt.current_paper_period_count = ZERO;
						Clear_Cache();
						reset_flag = DEFINE_VAL;
					}
				}
				else {
					 if (maincnt.tooth_row_count_flag) {
						maincnt.tooth_row_count_flag = ZERO;
					    /* >2һ����ֽ */
						if (maincnt.old_paper_period_count > 
								maincnt.empty_count) {	
							maincnt.paper_count++;
							maincnt.device_old_status     = STOPSTATUS;
							maincnt.device_current_status = RUNSTATUS;
							Clear_Cache();
							reset_flag = FIRST_VAL;
						}
						else if (maincnt.old_paper_period_count == 
								maincnt.empty_count) {
							maincnt.device_old_status = STOPSTATUS;
							/* �������غ��½���(��MCU��˵)����ֽ */
							if ((maincnt.old_paper_edge[ZERO] == FIRST_VAL) &&
									(maincnt.old_paper_edge[FIRST_VAL] == ZERO)) {
								maincnt.paper_count++;
								maincnt.device_current_status = RUNSTATUS;
							}
						    /* ���½��غ�������(��MCU��˵)����ֽ */		
							else{
								maincnt.device_current_status = EMPTYSTATUS;
							}
							Clear_Cache();
							reset_flag = FIRST_VAL;
						}
						else{
								
						}  
					}
				}
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
	printf("��һ����ƽ:%d\n�ڶ�����ƽ:%d\n���ڵı�������:%d\n",
			maincnt.old_paper_edge[ZERO],
			maincnt.old_paper_edge[FIRST],
			maincnt.old_paper_period_count);
	maincnt.old_paper_period_count = ZERO;
	maincnt.old_paper_edge[ZERO] = ZERO;
	maincnt.old_paper_edge[FIRST] = ZERO;
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

