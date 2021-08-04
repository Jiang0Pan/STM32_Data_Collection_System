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
函数名称：clear_buffer
功    能：清楚缓存数据
说    明：
参    数:
返    回: 
注    意：
*******************************************************************************/
static void clear_buffer(void)
{
	printf("周期的边沿数量:%d\n",
			maincnt.odests.dppcnt);
	maincnt.odests.dppcnt = 0;
}

/*******************************************************************************
函数名称：getsystime_us()
功    能：获取系统时间(us)
说    明：
参    数:
返    回: 返回U64数据类型的系统时间
注    意：
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
函数名称：Collect_Paper_Init
功    能：计数初始化，将空转的默认边沿数设置为2
说    明：
参    数:
返    回: 
注    意：
*******************************************************************************/
void Collect_Paper_Init(void)
{
	maincnt.empty_count = SECOND;
}

/*******************************************************************************
函数名称：Collext_Paper_Clear
功    能：清空当前的计数值
说    明：
参    数:
返    回: 
注    意：
*******************************************************************************/
void Collext_Paper_Clear(void)
{
	maincnt.paper_count = ZERO;
}

/*******************************************************************************
函数名称：Collect_Get_Tooth_Interrupt_countval
功    能：获取牙排中断，将牙排触发一次作为一个周期
说    明：
参    数:
返    回: 
注    意：
*******************************************************************************/
void Collect_Get_Tooth_Interrupt_countval(void)
{
	/* 将标志位置位 */
	maincnt.tooth_row_count_flag = FIRST;
	
	/* 新的周期来了，保存到旧的缓存区中 */
	maincnt.old_paper_period_count = 
			maincnt.current_paper_period_count;
    maincnt.current_paper_period_count = ZERO;
	maincnt.old_paper_edge[ZERO] =
			maincnt.current_paper_edge[ZERO];
	maincnt.current_paper_edge[ZERO] = ZERO;
    maincnt.old_paper_edge[FIRST] =
			maincnt.current_paper_edge[FIRST];
	maincnt.current_paper_edge[FIRST] = ZERO;

	/* 获得中断时间 */
	maincnt.intterrupt_time = GetSysTime_us();

}

/*******************************************************************************
函数名称：Collect_Get_Paper_Interrupt_countval
功    能：获取纸中断次数
说    明：
参    数:
返    回: 
注    意：
*******************************************************************************/
void Collect_Get_Paper_Interrupt_countval(void)
{
	/* 将当前周期的边沿数量++ */
	maincnt.current_paper_period_count++;	
	
	/* 获得第一个边沿的电平值 */
    if (FIRST == maincnt.current_paper_period_count) {
		maincnt.current_paper_edge[ZERO] = 
				READ_RESERVEDPULSE_EDGE;
	}
	/* 获得第二个边沿的电平值 */
	else if (SECOND == maincnt.current_paper_period_count) {
		maincnt.current_paper_edge[FIRST] = 
				READ_RESERVEDPULSE_EDGE;	
	} 
	/* 此种情况无需处理 */
	else {
		
	}
}

/*******************************************************************************
函数名称：Collect_Get_Paper_countval
功    能：进行状态判断、计数
说    明：
参    数:
返    回: 返回计数值
注    意：
*******************************************************************************/
U32 Collect_Get_Paper_countval(void)
{
    /* 运行状态的判断 */
    if (maincnt.device_current_status == RUNSTATUS) {
		/* 计数标志位被置位 */
        if (maincnt.tooth_row_count_flag) {
			/* 清位标志位 */
		    maincnt.tooth_row_count_flag = ZERO;
			/* >2 一定有纸 */
	        if (maincnt.old_paper_period_count > 
	                maincnt.empty_count) {
				maincnt.paper_count++;
				maincnt.device_old_status = RUNSTATUS;
			    maincnt.device_current_status = RUNSTATUS;
				Clear_Cache();
	        }
			/* ==2 继续判断是否有纸 */
	        else if (maincnt.old_paper_period_count == 
	                maincnt.empty_count) {
				maincnt.device_old_status = RUNSTATUS;
				/* 先上升沿后下降沿(对MCU来说)，有纸 */
				if ((maincnt.old_paper_edge[ZERO] == FIRST) &&
						(maincnt.old_paper_edge[FIRST] == ZERO)) {
	            	maincnt.paper_count++;
	                maincnt.device_current_status = RUNSTATUS;
				}
				/* 先下降沿后上升沿(对MCU来说)，无纸 */		
				else {
	                maincnt.device_current_status = EMPTYSTATUS;
				}
				Clear_Cache();
	        }
			/* 其他情况，无需处理 */
	        else {
	            
	        }
        }
		/* 超时，判断为停机 */
        maincnt.current_time = GetSysTime_us();
        if (labs(maincnt.current_time - 
                maincnt.intterrupt_time) > TIMEOUT_) {
            maincnt.device_old_status = RUNSTATUS;
            maincnt.device_current_status = STOPSTATUS;
        }
    }
	/* 空转状态判断 */
    else if (maincnt.device_current_status == EMPTYSTATUS) {
		/* 计数标志位被置位 */ 
         if (maincnt.tooth_row_count_flag) {
			/* 清位标志位 */
		    maincnt.tooth_row_count_flag = ZERO;
		    /* >2 一定有纸 */
            if (maincnt.old_paper_period_count > 
                    maincnt.empty_count) {
           /* 用于过滤纸光电将纸分成2个部分的后导致的多计数 */
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
			/* ==2 继续判断是否有纸 */
            else if (maincnt.old_paper_period_count == 
                    maincnt.empty_count) {
				maincnt.device_old_status = EMPTYSTATUS;
                /* 先上升沿后下降沿(对MCU来说)，有纸 */
               	if ((maincnt.old_paper_edge[ZERO] == FIRST) &&
						(maincnt.old_paper_edge[FIRST] == ZERO)) {
					maincnt.paper_count++;
					maincnt.device_current_status = RUNSTATUS;
				}
				/* 先下降沿后上升沿(对MCU来说)，无纸 */		
				else {
                    maincnt.device_current_status = EMPTYSTATUS;
				}
				Clear_Cache();
            }
		    /* 其他情况，无需处理 */
            else {
                
            }
        }
	    /* 超时，判断为停机 */
        maincnt.current_time = GetSysTime_us();
        if (labs(maincnt.current_time - 
            maincnt.intterrupt_time) > TIMEOUT_) {
            maincnt.device_old_status = EMPTYSTATUS;
            maincnt.device_current_status = STOPSTATUS;
        }
    }
	/* 停机状态判断 */	
    else if (maincnt.device_current_status == STOPSTATUS) {
		/* 如果设备上一次状态为运行或停机状态 */
		if ((maincnt.device_old_status == RUNSTATUS) ||
				(maincnt.device_old_status == STOPSTATUS)) {
			if (maincnt.tooth_row_count_flag) {
				maincnt.tooth_row_count_flag = ZERO;
				/* >2一定有纸 */
				if (maincnt.old_paper_period_count > 
						maincnt.empty_count) { 	
					maincnt.paper_count++;
					maincnt.device_old_status = STOPSTATUS;
					maincnt.device_current_status = RUNSTATUS;
					Clear_Cache();
				}
				/* ==2进行进一步判断 */
				else if (maincnt.old_paper_period_count == 
						maincnt.empty_count) {
					maincnt.device_old_status = STOPSTATUS; 
					/* 先上升沿后下降沿(对MCU来说)，有纸 */
					if ((maincnt.old_paper_edge[ZERO] == FIRST) &&
							(maincnt.old_paper_edge[FIRST] == ZERO)) {
						maincnt.paper_count++;
						maincnt.device_current_status = RUNSTATUS;
					}
		            /* 先下降沿后上升沿(对MCU来说)，无纸 */		
					else {
						maincnt.device_current_status = EMPTYSTATUS;
					}
					Clear_Cache();
				}
				else {
						
				}  
			}
	    }
		/* 若上次设备状态为空转状态，需要进行特殊处理 */
		else if (maincnt.device_old_status == EMPTYSTATUS) {
			maincnt.current_time = GetSysTime_us();
			/* 转速小于2400转/h */
			if (labs(maincnt.current_time - 
					maincnt.intterrupt_time) > 1500000) {
				maincnt.tooth_row_count_flag = ZERO;
				maincnt.current_paper_period_count = ZERO;
				Clear_Cache();
			}
			else {
			    /* 复位当前周期 */
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
					    /* >2一定有纸 */
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
							/* 先上升沿后下降沿(对MCU来说)，有纸 */
							if ((maincnt.old_paper_edge[ZERO] == FIRST_VAL) &&
									(maincnt.old_paper_edge[FIRST_VAL] == ZERO)) {
								maincnt.paper_count++;
								maincnt.device_current_status = RUNSTATUS;
							}
						    /* 先下降沿后上升沿(对MCU来说)，无纸 */		
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
函数名称：AD5761_1_ioconfig
功    能：AD5761_1IO配置.
说    明：
参    数:
返    回: 
注    意：
*******************************************************************************/
static void Collect_Status_Switch(U8 devicecurrentstatus)
{
	
}

/*******************************************************************************
函数名称：Clear_Cache
功    能：清楚缓存数据
说    明：
参    数:
返    回: 
注    意：
*******************************************************************************/
static void Clear_Cache(void)
{
	printf("第一个电平:%d\n第二个电平:%d\n周期的边沿数量:%d\n",
			maincnt.old_paper_edge[ZERO],
			maincnt.old_paper_edge[FIRST],
			maincnt.old_paper_period_count);
	maincnt.old_paper_period_count = ZERO;
	maincnt.old_paper_edge[ZERO] = ZERO;
	maincnt.old_paper_edge[FIRST] = ZERO;
}

/*******************************************************************************
函数名称：GetSysTime_us()
功    能：获取系统时间(us)
说    明：
参    数:
返    回: 返回U64数据类型的系统时间
注    意：
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

