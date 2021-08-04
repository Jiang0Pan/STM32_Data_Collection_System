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
函数名称：collect_init_paper_cnt
功    能：计数初始化，将空转的默认边沿数设置为2
说    明：
参    数:
返    回: 
注    意：
*******************************************************************************/
void collect_init_paper_cnt(void)
{
	maincnt.ecnt = EMPTY_DEFINE;
	maincnt.pcnt = 0;
}

/*******************************************************************************
函数名称：collect_clear_paper_cnt
功    能：清空当前纸张的计数值
说    明：
参    数:
返    回: 
注    意：
*******************************************************************************/
void collect_clear_paper_cnt(void)
{
	maincnt.pcnt = 0;
}

/*******************************************************************************
函数名称：collect_set_tooth_int_cnt
功    能：获取牙排中断，将牙排触发一次作为一个周期
说    明：
参    数:
返    回: 
注    意：
*******************************************************************************/
void collect_set_tooth_int_cnt(void)
{
	/* 将标志位置位 */
	maincnt.tooth_row_count_flag = SET_FLAG;
	
	/* 新的周期来了，保存到旧的缓存区中 */
	maincnt.odests.dppcnt = 
			maincnt.cdests.dppcnt;
    maincnt.cdests.dppcnt = 0;

	/* 获得中断时间 */
	maincnt.inttime = getsystime_us();

}

/*******************************************************************************
函数名称：collect_set_paper_int_cnt
功    能：获取纸中断次数
说    明：
参    数:
返    回: 
注    意：
*******************************************************************************/
void collect_set_paper_int_cnt(void)
{
	/* 将当前周期的边沿数量++ */
	maincnt.cdests.dppcnt++;	
}

/*******************************************************************************
函数名称：collect_get_paper_cnt
功    能：进行状态判断、计数(贴面机)
说    明：
参    数:
返    回: 返回计数值
注    意：
*******************************************************************************/
U32 collect_get_paper_cnt(void)
{
    /* 运行状态的判断 */
    if (maincnt.cdests.dsts == RUNSTATUS) {
		/* 计数标志位被置位 */
        if (maincnt.tooth_row_count_flag) {
			/* 清位标志位 */
		    maincnt.tooth_row_count_flag = RESET_FLAG;
			/* >=2 一定有纸 */
	        if (maincnt.odests.dppcnt >= 
	                maincnt.ecnt) {
				maincnt.pcnt++;
				maincnt.odests.dsts = RUNSTATUS;
			    maincnt.cdests.dsts = RUNSTATUS;
				clear_buffer();
	        }
			/* 其他情况，无纸 */
	        else {
				maincnt.odests.dsts = RUNSTATUS;
			    maincnt.cdests.dsts = EMPTYSTATUS;
				clear_buffer();
	        }
        }
		/* 超时，判断为停机 */
        maincnt.curtime = getsystime_us();
        if (labs(maincnt.curtime - 
                maincnt.inttime) > TIMEOUT_) {
            maincnt.odests.dsts = RUNSTATUS;
            maincnt.cdests.dsts = STOPSTATUS;
        }
    }
	/* 空转状态判断 */
    else if (maincnt.cdests.dsts == EMPTYSTATUS) {
		/* 计数标志位被置位 */ 
         if (maincnt.tooth_row_count_flag) {
			/* 清位标志位 */
		    maincnt.tooth_row_count_flag = RESET_FLAG;
		    /* >2 一定有纸 */
            if (maincnt.odests.dppcnt >= 
                    maincnt.ecnt) {
					maincnt.pcnt++;
					maincnt.odests.dsts = EMPTYSTATUS;
					maincnt.cdests.dsts = RUNSTATUS;
					clear_buffer();
            }
		    /* 其他情况，无纸  */
            else {
				maincnt.odests.dsts = EMPTYSTATUS;
			    maincnt.cdests.dsts = EMPTYSTATUS;
				clear_buffer();
            }
        }
	    /* 超时，判断为停机 */
        maincnt.curtime = getsystime_us();
        if (labs(maincnt.curtime - 
            maincnt.inttime) > TIMEOUT_) {
            maincnt.odests.dsts = EMPTYSTATUS;
            maincnt.cdests.dsts = STOPSTATUS;
        }
    }
	/* 停机状态判断 */	
    else if (maincnt.cdests.dsts == STOPSTATUS) {
		/* 如果设备上一次状态为运行或停机状态 */
		if (maincnt.tooth_row_count_flag) {
			maincnt.tooth_row_count_flag = RESET_FLAG;
			/* >2一定有纸 */
			if (maincnt.odests.dppcnt >= 
					maincnt.ecnt) { 	
				maincnt.pcnt++;
				maincnt.odests.dsts = STOPSTATUS;
				maincnt.cdests.dsts = RUNSTATUS;
				clear_buffer();
			}
		    /* 其他情况，无纸  */
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

