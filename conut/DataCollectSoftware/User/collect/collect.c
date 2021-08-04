/**
  ******************************************************************************
  * @file           : collect.c
  * @brief          : collect data
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
#include "string.h"
#include "maincounter.h"
#include "qulitycounter.h"
#include "diechangecounter.h"
//#include "waveformcapture.h"


/*******************************************************************************
函数名称：Collect_config()
功    能：采集参数配置,上电配置
说    明：
参    数: 
返    回：
注    意：
*******************************************************************************/
void  Collect_config(void)
{
    //根据传感器类型配置处理方式
	
    g_sys_cfg.col_cfg.pulserelate = RELATE_STATE;
    g_sys_cfg.col_cfg.up_prd = UPLOADTIME;
	
    g_sys_cfg.col_cfg.bt_typ = BOOTSENSOR; //高电平有效
    g_sys_cfg.col_cfg.bt_wd = COLLECTSIGWIDTH; //有效信号最小宽度
	
    g_sys_cfg.col_cfg.st_typ = STOPSENSOR; //高电平有效
    g_sys_cfg.col_cfg.st_wd = COLLECTSIGWIDTH;//有效信号最小宽度
	
    g_sys_cfg.col_cfg.ft_typ = FAULTSENSOR; //高电平有效
    g_sys_cfg.col_cfg.ft_wd = COLLECTSIGWIDTH;//有效信号最小宽度
		
    g_sys_cfg.col_cfg.m_ch_typ = MAINPULSESENSOR; //主计数传感器
    g_sys_cfg.col_cfg.m_ch_wd = COLLECTSIGWIDTH;//有效信号最小宽度
    g_sys_cfg.col_cfg.m_ch_prd_min = COLLECTSIGMININTERVAL;//最小信号间隔时间
    g_sys_cfg.col_cfg.m_ch_prd_max = COLLECTSIGMAXINTERVAL;//最大信号间隔时间
	
	
    g_sys_cfg.col_cfg.r_ch_typ = REVPULSESENSOR;    //预留计数传感器
    g_sys_cfg.col_cfg.r_ch_wd = COLLECTSIGWIDTH; //有效信号最小宽度
    g_sys_cfg.col_cfg.r_ch_prd_min = COLLECTSIGMININTERVAL;//最小信号间隔时间
    g_sys_cfg.col_cfg.r_ch_prd_max = COLLECTSIGMAXINTERVAL;//最大信号间隔时间
	
    //刀版脉冲信号  2019年11月30日新增
    g_sys_cfg.col_cfg.b_ch_typ = REVPULSESENSOR;    //刀版计数传感器
    g_sys_cfg.col_cfg.b_ch_wd = COLLECTSIGWIDTH;            //刀版有效信号最小宽度
    g_sys_cfg.col_cfg.b_ch_prd_min = COLLECTSIGMININTERVAL;//最小信号间隔时间
    g_sys_cfg.col_cfg.b_ch_prd_max = COLLECTSIGMAXINTERVAL;//最大信号间隔时间
	
    g_sys_cfg.col_cfg.cl_tm.hr    = CLEARHOUR;      //清零小时
    g_sys_cfg.col_cfg.cl_tm.min= CLEARMINUTES;   //清零分钟
    g_sys_cfg.col_cfg.rt_prd    = REALSTORETIME;  //实时存储时间
	
    //默认正纸
    g_sys_sts.col_sts.pnopaperstate = TRUE;  
}


/*******************************************************************************
函数名称：Collect_init()
功    能：采集检测数据初始化,上电检测数据初始化
说    明：
参    数: 
返    回：
注    意：
*******************************************************************************/
void  Collect_init(void)
{
    memset(&g_sys_sts.col_sts,0,sizeof(CollectSigState));
    memset(&g_sys_sts.col_info,0,sizeof(CollectData));
    //旧状态初始化
    g_sys_sts.col_info.bt_old_sts = BOOTSIG_IN;
    g_sys_sts.col_info.st_old_sts = STOPSIG_IN;
    g_sys_sts.col_info.ft_old_sts = FAULTSIG_IN;
    //时间初始化
    HAL_RTC_GetDate(&hrtc, &g_sys_sts.col_info.pwr_on_tm.sdate, RTC_FORMAT_BIN);
    HAL_RTC_GetTime(&hrtc, &g_sys_sts.col_info.pwr_on_tm.stime, RTC_FORMAT_BIN);

    sprintf((char*)g_sys_sts.commit_data.timebegin,"20%02d-%02d-%02d %02d:%02d:%02d",
            g_sys_sts.col_info.pwr_on_tm.sdate.Year,
            g_sys_sts.col_info.pwr_on_tm.sdate.Month,
            g_sys_sts.col_info.pwr_on_tm.sdate.Date,
            g_sys_sts.col_info.pwr_on_tm.stime.Hours,
            g_sys_sts.col_info.pwr_on_tm.stime.Minutes,
            g_sys_sts.col_info.pwr_on_tm.stime.Seconds);
	collect_clear_paper_cnt();
	collect_clear_qt_cnt();
	collect_clear_ep_cnt();
}

/*******************************************************************************
函数名称：Collect_JudgeEdge(uint8_t oldsig,uint8_t currentsig)
功    能：采集检测数据，边沿判断
说    明：
参    数: oldsig:上一次的状态 currentsig:边沿信号
返    回：
注    意：
*******************************************************************************/
COLLECTJUDGEEDGE Collect_JudgeEdge(uint8_t oldsig,uint8_t currentsig)
{
    /*前一次信号为0，后一次信号为1，即为上升沿触发*/
    if (oldsig == LOWLEVEL && currentsig == HIGHLEVEL) {
        return RISE_EDGE;
    }
	
    else if (oldsig == HIGHLEVEL && currentsig == LOWLEVEL) {
        return DESC_EDGE;
    }
    else {
		return NO_EDGE;
    }
}

/*******************************************************************************
函数名称：Collect_JudeSigWidth(u8 sigtype,u8 currentsig,u8 *judgelogic,u16 sigwidth,u16 *sigwidthcount)
功    能：判断信号宽度,由定时器里面调用，1MS触发一次
说    明：
参    数: sigtype:制定触发方式 currentsig:边沿信号 judgelogic:判断逻辑
返    回：状态
注    意：
*******************************************************************************/
u8 Collect_JudeSigWidth(u8 sigtype,u8 currentsig,u8 *judgelogic,u16 sigwidth,u16 *sigwidthcount)
{
    u8  _returnstate = FALSE;
    switch (sigtype) {
        case IO_NPN :       //低电平有效
            break;		
			
        case IO_PNP :       //高电平有效
            break;		
			
        case IO_HIGH_PULSE : //高脉冲有效
        
            if (*judgelogic == JUDGE_WITCH) {
			
                if (currentsig == HIGHLEVEL) {
                    (*sigwidthcount)++;
				
                    if (*sigwidthcount >= sigwidth) {
                        _returnstate = TRUE;
                        *judgelogic = JUDGE_EDGE;
                        *sigwidthcount = 0;
                    }
                }
			
                else {
                    *sigwidthcount = 0;
                }
            }
		
            else {
                *sigwidthcount = 0;
            }
            break;	
		
        case IO_LOW_PULSE :    //低脉冲有效	
        
            if(*judgelogic == JUDGE_WITCH) {
				
                if(currentsig == LOWLEVEL) {
                    (*sigwidthcount)++;
					
                    if(*sigwidthcount >= sigwidth) {
                        _returnstate = TRUE;
                        *judgelogic = JUDGE_EDGE;
                        *sigwidthcount = 0;
                    }
                }
				
                else {
                    *sigwidthcount = 0;
                }
            }
			
            else {
                *sigwidthcount = 0;
            }
            break;
			
		  default:
            break;
    }
    return _returnstate;
}

/*******************************************************************************
函数名称：Collect_CountPwmWidth(u8 *judgelogic,u16 *pwm1width,u16 *pwm2width)
功    能：判断脉宽占空比
说    明：
参    数: judgelogic:判断逻辑 pwm1width:PWM1波形的脉宽 pwm2width:pwm2的脉宽
返    回：
注    意：
*******************************************************************************/
void  Collect_CountPwmWidth(u8 *judgelogic,u16 *pwm1width,u16 *pwm2width)
{
    switch (*judgelogic) {
        case JUDGE_PWM_1: {
            if (*pwm1width < 60000) (*pwm1width)++;
        }
            break;
			
        case JUDGE_PWM_2: {
            if (*pwm2width < 60000) (*pwm2width)++;
        }
            break;
			
        default:
            break;
    }
}

/*******************************************************************************
函数名称：Collect_JudeSigValid(u8 currentjudgelogic, //当前逻辑
                             u8 oldjudgelogic,       //旧逻辑
                             u16 *pwm1width,         //第一部分的占空时间
                             u16 *pwm2width,         //第二部分的占空时间    
                             u16 *maxduty,           //最大的占空比
                             u16 *minduty,           //最小的占空比
                             u16 *maxperiod          //最大周期，最小周期在程序中限制
                             )
功    能：通过周期和脉宽判断信号是否有效
说    明：
参    数: judgelogic:判断逻辑 pwm1width:PWM1波形的脉宽 pwm2width:pwm2的脉宽
返    回：
注    意：
*******************************************************************************/
uint8_t Collect_JudeSigValid(u8 currentjudgelogic, 
                             u8 oldjudgelogic,     
                             u16 *pwm1width,      
                             u16 *pwm2width,      
                             u16 *maxduty,         
                             u16 *minduty,        
                             u16 *maxperiod       
                             )
{
    float _duty = 0;
    u32 _period = 0;
	
    /*判断一个检测周期结束*/
    if ((oldjudgelogic == JUDGE_PWM_2) &&(currentjudgelogic == JUDGE_PWM_1)) {
        _period = *pwm1width + *pwm2width;
		  
        if ((_period < COLLECTMINPERIOD) || (_period > *maxperiod)) {
            *pwm1width = 0;//计数值清零
            *pwm2width = 0;
            return FALSE;
        }
		  
        _duty =(float)(*pwm1width)/ _period *100;
        *pwm1width = 0;//计数值清零
        *pwm2width = 0;
		  
        if ((_duty >= *minduty) && (_duty <= *maxduty)) {
            return TRUE;
        }
		  
        else {
            return FALSE;
        }
    }
	 
    else {
        return FALSE;
    }
}


/*******************************************************************************
函数名称：Collect_JudeSigInterVal(u16 *sigintervalcount,
                                  u16 sigmininterval,
                                  uint16_t sigmaxinterval,u8 *sigstate)
功    能：判断信号间隔,1MS触发一次，用于计数状态判断
说    明：
参    数: sigintervalcount:时间间隔 sigmininterval:最小的比较值
          sigmaxinterval:最大比较值 sigstate:超时
返    回：
注    意：
*******************************************************************************/
void Collect_JudeSigInterVal(u16 *sigintervalcount,u16 sigmininterval,uint16_t sigmaxinterval,u8 *sigstate)
{
     /*计算信号间隔*/
     if (*sigintervalcount < sigmaxinterval) {
          (*sigintervalcount)++;
		  
          if (*sigintervalcount < sigmininterval) {
               *sigstate = WAITDELAY;
          }
		  
          else if (*sigintervalcount >= sigmaxinterval) {
               *sigstate = INVALID;
          }
		  
          else {
               *sigstate = VALID;
          }				
	 }
	 
	 else {
          *sigstate = INVALID; //信号超时
     }
}


/*******************************************************************************
函数名称：Collect_TimerHandle()
功    能：1MS定时器里面的关于采集信号处理函数
说    明：
参    数: 
返    回：
注    意：
*******************************************************************************/
void Collect_TimerHandle(void)
{
    static uint8_t _mainlogic_old = JUDGE_EDGE;
    static uint8_t _revlogic_old = JUDGE_EDGE;
	 /*开机信号非电平状态确认*/
    if (g_sys_cfg.col_cfg.bt_typ > IO_PNP) {
	 	
        if (Collect_JudeSigWidth(g_sys_cfg.col_cfg.bt_typ,
        BOOTSIG_IN,
        &g_sys_sts.col_info.bt_trg_md,
        g_sys_cfg.col_cfg.bt_wd,
        &g_sys_sts.col_info.bt_wd
        ) == TRUE) {
          
            if (g_sys_sts.col_sts.bootsig == INVALID) {
                g_sys_sts.col_sts.bootsig = VALID;
            }
			   
            else {
                g_sys_sts.col_sts.bootsig = INVALID;
            }
        }
		  
        else {
		  	
        }
    }
	
    /*停机信号非电平状态确认*/
    if(g_sys_cfg.col_cfg.st_typ > IO_PNP) {
		
        if (Collect_JudeSigWidth(g_sys_cfg.col_cfg.st_typ,
        STOPSIG_IN,
        &g_sys_sts.col_info.st_trg_md,
        g_sys_cfg.col_cfg.st_wd,
        &g_sys_sts.col_info.st_wd
        ) == TRUE) {
        
            if (g_sys_sts.col_sts.stopsig == INVALID) {
                g_sys_sts.col_sts.stopsig = VALID;
            }
			
            else {
                g_sys_sts.col_sts.stopsig = INVALID;
            }
        }
		
        else {

        }
    }

    /*故障信号非电平状态确认*/
    if (g_sys_cfg.col_cfg.ft_typ > IO_PNP) {
		
        if (Collect_JudeSigWidth(g_sys_cfg.col_cfg.ft_typ,
        FAULTSIG_IN,
        &g_sys_sts.col_info.ft_trg_md,
        g_sys_cfg.col_cfg.ft_wd,
        &g_sys_sts.col_info.ft_wd
        ) == TRUE) {
        
            if (g_sys_sts.col_sts.faultsig == INVALID) {
                g_sys_sts.col_sts.faultsig = VALID;
            }
			
            else {
            g_sys_sts.col_sts.faultsig = INVALID;
            }
			
        }
		
        else {
			
        }
    }
    
    /*主脉冲检测*/
	/*主计数信号非电平状态确认*/
    if ((g_sys_cfg.col_cfg.m_ch_typ == IO_HIGH_PULSE) || (g_sys_cfg.col_cfg.m_ch_typ == IO_LOW_PULSE)) {

		/*间隔计数清零*/
		if (Collect_JudeSigWidth(g_sys_cfg.col_cfg.m_ch_typ,
        MAINPULSESIG_IN,
        &g_sys_sts.col_info.m_ch_trg_md,
        g_sys_cfg.col_cfg.m_ch_wd,
        &g_sys_sts.col_info.m_ch_wd
        ) == TRUE) {
        g_sys_sts.col_info.m_ch_tm_cnt  = 0; 
		
            /*关联运行状态计数*/
            if (g_sys_cfg.col_cfg.pulserelate == RELATE_STATE) {

				/*开机信号有效才计数*/
                if (g_sys_sts.col_sts.bootsig == VALID) {
                    g_sys_sts.col_sts.mainpulsecount ++;
                }				
            }

			/*关联前脉冲数计数*/
            else if (g_sys_cfg.col_cfg.pulserelate == RELATE_REVPULSE) {

				/*前一个计数传感器有效才计数*/
                if (g_sys_sts.col_sts.revpulsesig != INVALID) {
                    g_sys_sts.col_sts.mainpulsecount ++;
				}
            }
			
            else {
						
            }
        }
        else {
			
        }
		    
    }
	
    /*2020年1月12日新增,脉宽占空比检测*/
    else if ((g_sys_cfg.col_cfg.m_ch_typ == IO_PWM_HIGH) || (g_sys_cfg.col_cfg.m_ch_typ == IO_PWM_LOW)) {
    Collect_CountPwmWidth(&g_sys_sts.col_info.m_ch_trg_md,
    &g_sys_sts.col_info.m_ch_pwm_h_tm,
    &g_sys_sts.col_info.m_ch_pwm_l_tm);//PWM计数
			
        if (Collect_JudeSigValid(g_sys_sts.col_info.m_ch_trg_md, 
        _mainlogic_old,
        &g_sys_sts.col_info.m_ch_pwm_h_tm,
        &g_sys_sts.col_info.m_ch_pwm_l_tm, 
        &g_sys_cfg.col_cfg.m_ch_prd_max,
        &g_sys_cfg.col_cfg.m_ch_prd_min,
        &g_sys_cfg.col_cfg.m_ch_wd
        ) == TRUE) {

		    /*间隔计数清零*/
            g_sys_sts.col_info.m_ch_tm_cnt  = 0; 

			/*关联运行状态计数*/
            if(g_sys_cfg.col_cfg.pulserelate == RELATE_STATE) {

                /*开机信号有效才计数*/
                if(g_sys_sts.col_sts.bootsig == VALID) {
                    g_sys_sts.col_sts.mainpulsecount ++;
                }
								
            }

            /*关联前脉冲数计数*/	
            else if (g_sys_cfg.col_cfg.pulserelate == RELATE_REVPULSE) {

		    	 /*前一个计数传感器有效才计数*/
                if (g_sys_sts.col_sts.revpulsesig != INVALID) {
                    g_sys_sts.col_sts.mainpulsecount ++;
                }
            }

            else {
						
            }
        }
		
        else {
		
        }
        _mainlogic_old = g_sys_sts.col_info.m_ch_trg_md;
		
    }
	
    else {
		
    }
	
    /*辅助脉冲检测*/
	/*辅助计数信号非电平状态确认*/
    if ((g_sys_cfg.col_cfg.r_ch_typ == IO_HIGH_PULSE) || (g_sys_cfg.col_cfg.r_ch_typ == IO_LOW_PULSE)) {

		if (Collect_JudeSigWidth(g_sys_cfg.col_cfg.r_ch_typ,
        REVPULSESIG_IN,
        &g_sys_sts.col_info.r_ch_trg_md,
        g_sys_cfg.col_cfg.r_ch_wd,
        &g_sys_sts.col_info.r_ch_wd
        ) == TRUE) {
            g_sys_sts.col_info.r_ch_tm_cnt  = 0; //间隔计数清零
            g_sys_sts.col_sts.revpulsecount ++;
        }
				
        else {
					
        }
		
    }
	
    else if ((g_sys_cfg.col_cfg.r_ch_typ == IO_PWM_HIGH) || (g_sys_cfg.col_cfg.r_ch_typ == IO_PWM_LOW)) {
        Collect_CountPwmWidth(&g_sys_sts.col_info.r_ch_trg_md,
        &g_sys_sts.col_info.r_ch_pwm_h_tm,
        &g_sys_sts.col_info.r_ch_pwm_l_tm);//PWM计数
			
        if (Collect_JudeSigValid(g_sys_sts.col_info.r_ch_trg_md, 
        _revlogic_old,
        &g_sys_sts.col_info.r_ch_pwm_h_tm,
        &g_sys_sts.col_info.r_ch_pwm_l_tm, 
        &g_sys_cfg.col_cfg.r_ch_prd_max,
        &g_sys_cfg.col_cfg.r_ch_prd_min,
        &g_sys_cfg.col_cfg.r_ch_wd
        ) == TRUE) {
            g_sys_sts.col_info.r_ch_tm_cnt  = 0; //间隔计数清零
            g_sys_sts.col_sts.revpulsecount ++;
        }
		
        else {
			
        }
		
        _revlogic_old = g_sys_sts.col_info.r_ch_trg_md;
		
		
		
    }
	
    else {
		
    }

	/*辅助计数信号非电平状态确认*/
    if (g_sys_cfg.col_cfg.b_ch_typ > IO_PNP) {
		
        if (Collect_JudeSigWidth(g_sys_cfg.col_cfg.b_ch_typ,
        BLADEPULSESIG_IN,
        &g_sys_sts.col_info.b_ch_trg_md,
        g_sys_cfg.col_cfg.b_ch_wd,
        &g_sys_sts.col_info.b_ch_wd
        ) == TRUE) {
            g_sys_sts.col_info.b_ch_tm_cnt  = 0; //间隔计数清零
            g_sys_sts.col_sts.bladepulsecount ++;
					
            g_sys_sts.col_info.bladeenable = TRUE; //刀版数据使能上传
        }
				
        else {
					
        }
		
    }
		
    /*主脉冲信号间隔时间检测*/
    if((g_sys_cfg.col_cfg.m_ch_typ == IO_HIGH_PULSE) || (g_sys_cfg.col_cfg.m_ch_typ == IO_LOW_PULSE)) {
        Collect_JudeSigInterVal(&g_sys_sts.col_info.m_ch_tm_cnt,
        g_sys_cfg.col_cfg.m_ch_prd_min,
        g_sys_cfg.col_cfg.m_ch_prd_max,
        &g_sys_sts.col_sts.mainpulsesig);
    }
	
    else {
        Collect_JudeSigInterVal(&g_sys_sts.col_info.m_ch_tm_cnt,
        10,
        1000,
        &g_sys_sts.col_sts.mainpulsesig);
    }
     
    /*预留脉冲信号间隔时间检测*/
    if((g_sys_cfg.col_cfg.r_ch_typ == IO_HIGH_PULSE) || (g_sys_cfg.col_cfg.r_ch_typ == IO_LOW_PULSE)) {
        Collect_JudeSigInterVal(&g_sys_sts.col_info.r_ch_tm_cnt,
        g_sys_cfg.col_cfg.r_ch_prd_min,
        g_sys_cfg.col_cfg.r_ch_prd_max,
        &g_sys_sts.col_sts.revpulsesig);
    }
	
    else {
        Collect_JudeSigInterVal(&g_sys_sts.col_info.r_ch_tm_cnt,
        10,
        1000,
        &g_sys_sts.col_sts.revpulsesig);
    }
		
    /*刀版脉冲信号间隔时间检测  2019年11月30日新增*/
    Collect_JudeSigInterVal(&g_sys_sts.col_info.b_ch_tm_cnt,
    g_sys_cfg.col_cfg.b_ch_prd_min,
    g_sys_cfg.col_cfg.b_ch_prd_max,
    &g_sys_sts.col_sts.bladepulsesig);	
}

/*******************************************************************************
函数名称：Collect_StateTimeAggregation()
功    能：采集状态时间累积,1S触发一次
说    明：
参    数: 
返    回：
注    意：
*******************************************************************************/
void Collect_StateTimeAggregation(void)
{
    if (g_sys_sts.col_sts.bootsig==VALID && g_sys_sts.col_sts.mainpulsesig!= INVALID) {
	    g_sys_sts.col_info.bt_tm++;
    }
	
    else {
        g_sys_sts.col_info.st_tm++;
    }
	 
    if (g_sys_sts.col_sts.faultsig==VALID ) {
        g_sys_sts.col_info.ft_tm++;  
    }
	
    g_sys_sts.col_info.all_tm++;//总时间 
}

/*******************************************************************************
函数名称：Collect_JudeSigState(u8 *oldsig,u8 currentsig,
                               u8 sigtype,u8 *judgelogic,u8 *sigstate)
功    能：判断信号状态
说    明：oldsig:上次状态 currentsig:边沿信号 sigtype:信号类型 judgelogic:判断逻辑
          sigstate:电平状态
参    数: 
返    回：
注    意：
*******************************************************************************/
void Collect_JudeSigState(u8 *oldsig,u8 currentsig,u8 sigtype,u8 *judgelogic,u8 *sigstate)
{
    switch (sigtype) {
		
        case IO_NPN :       //低电平有效
            if(currentsig == HIGHLEVEL) *sigstate = INVALID;
            else *sigstate = VALID;
            *judgelogic = JUDGE_EDGE;
        break;	
		
        case IO_PNP :       //高电平有效
            if(currentsig == HIGHLEVEL) *sigstate = VALID;
            else *sigstate = INVALID;
            *judgelogic = JUDGE_EDGE;
        break;	
		
        case IO_HIGH_PULSE : //高脉冲有效
            if(Collect_JudgeEdge(*oldsig,currentsig) == RISE_EDGE) {
                *judgelogic = JUDGE_WITCH;
            }
        break;	
		
        case IO_LOW_PULSE :    //低脉冲有效	
            if(Collect_JudgeEdge(*oldsig,currentsig) == DESC_EDGE) {
                *judgelogic = JUDGE_WITCH;
            }
        break;
			
        default:
        break;
    }
    *oldsig = currentsig;//加载原始信号
}

/*******************************************************************************
函数名称：Collect_Jude (u8 currentsig,u8 sigtype,u8 *judgelogic)
功    能：接收外部中断判断，计数外部IO中断
说    明：
参    数: currentsig:边沿信号 sigtype:信号类型 judgelogic:判断逻辑
返    回：
注    意：
*******************************************************************************/
void  Collect_Jude (u8 currentsig,u8 sigtype,u8 *judgelogic)
{
    switch (sigtype) {
        case IO_NPN :       //低电平有效
        break;		
		
	    case IO_PNP :       //高电平有效
        break;				
		
        case IO_HIGH_PULSE : //高脉冲有效
        
            if (currentsig == HIGHLEVEL) {
                *judgelogic = JUDGE_WITCH;
            }
        break;	
		
        case IO_LOW_PULSE :    //低脉冲有效	
        
            if(currentsig == LOWLEVEL) {
                *judgelogic = JUDGE_WITCH;
            }
        break;
		
        case IO_PWM_HIGH :    //脉宽高
        
            if(currentsig == HIGHLEVEL) {
                *judgelogic = JUDGE_PWM_1;
            }
			
            else {
                *judgelogic = JUDGE_PWM_2;
            }
        break;
		
        case IO_PWM_LOW :    //脉宽低
            if(currentsig == LOWLEVEL) {
                *judgelogic = JUDGE_PWM_1;
            }
			
            else { 
                *judgelogic = JUDGE_PWM_2;
            }
        break;
		
        default:			
        break;
    }
}

/*******************************************************************************
函数名称：Collect_ExitHandle (SIGINTERFACE siginterface)
功    能：中断处理函数
说    明：
参    数: SIGINTERFACE siginterface:采集信号
返    回：
注    意：
*******************************************************************************/
void  Collect_ExitHandle (SIGINTERFACE siginterface)
{
    switch(siginterface) {
		case BOOTSIG :		 //开机信号
			break;			
	
		case STOPSIG :		 //停机信号
			break;		
	
		case FAULTSIG : 	 //故障信号
			break;				
	
		case MAINPULSESIG :  //主计数信号
			collect_set_tooth_int_cnt();
			break;
		
		case RESERVEDPULSESIG:
			collect_set_paper_int_cnt();
			break;
		
		case REVPULSESIG :	 //辅助计数信号
			collect_set_qt_intcnt();
			break;
	
		case BLADEPULSESIG : //刀版计数信号
			collect_set_ep_intcnt();
			break;
	
		default:
			break;

    }
}

/*******************************************************************************
函数名称：Collect_Panpaper ()
功    能：正反纸检测
说    明：
参    数: 
返    回：
注    意：
*******************************************************************************/
void  Collect_Panpaper(void)
{
    static u32 lastcount;
	
	/*计数数据变化*/
    if (g_sys_sts.col_sts.mainpulsecount > lastcount) {
		
        if (PANPAPER_IN == 1 ) {
            g_sys_sts.col_sts.pnopaperstate = TRUE;
        }
		
        else {
			g_sys_sts.col_sts.pnopaperstate = FALSE;
        }
    }
	
    lastcount = g_sys_sts.col_sts.mainpulsecount;
}

/*******************************************************************************
函数名称：Collect_Handle()
功    能：采集状态数据读取
说    明：
参    数: 
返    回：
注    意：
*******************************************************************************/
void  Collect_Handle(void)
{
    /*开机信号*/
    Collect_JudeSigState(&g_sys_sts.col_info.bt_old_sts,
                        BOOTSIG_IN,
                        g_sys_cfg.col_cfg.bt_typ,
                        &g_sys_sts.col_info.bt_trg_md,
                        &g_sys_sts.col_sts.bootsig
                        );
	
    /*关机信号*/
    Collect_JudeSigState(&g_sys_sts.col_info.st_old_sts,
                        STOPSIG_IN,
                        g_sys_cfg.col_cfg.st_typ,
                        &g_sys_sts.col_info.st_trg_md,
                        &g_sys_sts.col_sts.stopsig
                        );
    /*故障信号*/
    Collect_JudeSigState(&g_sys_sts.col_info.ft_old_sts,
                        FAULTSIG_IN,
                        g_sys_cfg.col_cfg.ft_typ,
                        &g_sys_sts.col_info.ft_trg_md,
                        &g_sys_sts.col_sts.faultsig
                        );		
    /*正反纸检测*/
    Collect_Panpaper();	 
}

/*******************************************************************************
函数名称：Collect_UploadTimeCount()
功    能：采集数据定时上报
说    明：
参    数: 
返    回：
注    意：
*******************************************************************************/
void  Collect_UploadTimeCount(void)
{
    static int time_count_1 = 0;
    static int time_count_2 = 0;
    time_count_1 ++;
	
    if (time_count_1 >= g_sys_cfg.col_cfg.up_prd) {
        time_count_1 = 0;
        g_sys_sts.col_info.upload_en = TRUE;
    } 
	
    time_count_2 ++;
   
    if (time_count_2 >= g_sys_cfg.col_cfg.rt_prd) {
        time_count_2 = 0;
        g_sys_sts.col_info.storeenable = TRUE;
    } 
}

/*******************************************************************************
函数名称：Collect_RealTimeStore()
功    能：数据定时缓存
说    明：
参    数: 
返    回：
注    意：
*******************************************************************************/
void Collect_RealTimeStore(void)
{
    HAL_RTC_GetDate(&hrtc, &g_sys_sts.realtime_data.MachineCurrent.sdate, RTC_FORMAT_BIN);
    delay_ms(1);
    HAL_RTC_GetTime(&hrtc, &g_sys_sts.realtime_data.MachineCurrent.stime, RTC_FORMAT_BIN);
	      
    g_sys_sts.realtime_data.dataindex     =  g_sys_sts.commit_data.dataindex;//数据打包                                                   //上报信息序列号
    
    g_sys_sts.realtime_data.alltime      =   g_sys_sts.col_info.all_tm;      //机器总时间
    g_sys_sts.realtime_data.bootsigtime  =   g_sys_sts.col_info.bt_tm;       //正常开机总时间
    g_sys_sts.realtime_data.stopsigtime  =   g_sys_sts.col_info.st_tm;       //停机总时间
    g_sys_sts.realtime_data.faultsigtime =   g_sys_sts.col_info.ft_tm;       //故障总时间
				
				
    g_sys_sts.realtime_data.bootstopsig = ((g_sys_sts.col_sts.bootsig !=0) && (g_sys_sts.col_sts.mainpulsesig!=0));       //设备状态开机或者停机
    g_sys_sts.realtime_data.faultsig = g_sys_sts.col_sts.faultsig;	            //错误信号状态	
    g_sys_sts.realtime_data.pulsecount = g_sys_sts.col_sts.mainpulsecount;     //脉冲数

    g_sys_sts.realtime_data.bladepulsecount = g_sys_sts.col_sts.bladepulsecount;     //脉冲数

    //EEPROM_writeRomData(EEPROM_FIRST_CHIP | EEPROM_DATA,(u8 *)&g_sys_sts.realtime_data,sizeof(RealTimeStore));

}

/*******************************************************************************
函数名称：Collect_UploadHandle()
功    能：数据上报处理函数
说    明：
参    数: 
返    回：
注    意：
*******************************************************************************/
void Collect_UploadHandle(void)
{
    /*数据上报*/
    if (g_sys_sts.col_info.upload_en == TRUE) {
			  
        g_sys_sts.col_info.upload_en = FALSE;
        memcpy(g_sys_sts.commit_data.guid , g_sys_cfg.dev_cfg.guid,sizeof(int8_t)*UUIDBUFLENGTH);                      //提交的盒子guid号
        g_sys_sts.commit_data.dataindex ++;                                                   //上报信息序列号
        /*数据打包*/
        g_sys_sts.commit_data.alltime = g_sys_sts.col_info.all_tm;       //机器总时间
        g_sys_sts.commit_data.boottime = g_sys_sts.col_info.bt_tm;         //正常开机总时间
        g_sys_sts.commit_data.stoptime = g_sys_sts.col_info.st_tm;         //停机总时间
        g_sys_sts.commit_data.faulttime = g_sys_sts.col_info.ft_tm;       //故障总时间
				
				
        g_sys_sts.commit_data.bootstopsig = ((g_sys_sts.col_sts.bootsig !=0) && (g_sys_sts.col_sts.mainpulsesig!=0));       //设备状态开机或者停机
        g_sys_sts.commit_data.faultsig = g_sys_sts.col_sts.faultsig;	            //错误信号状态	
        g_sys_sts.commit_data.pulsecount = g_sys_sts.col_sts.mainpulsecount;     //脉冲数
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
        CommitCollectData_Upload(); 
    }

    /*实时缓存*/
    if (g_sys_sts.col_info.storeenable == TRUE) {	     
        g_sys_sts.col_info.storeenable = FALSE;
        Collect_RealTimeStore();
    } 

    /*刀版数据上报*/	 
    if (g_sys_sts.col_info.bladeenable == TRUE) {
        g_sys_sts.col_info.bladeenable = FALSE;
			  
        memcpy(g_sys_sts.commit_data.guid , g_sys_cfg.dev_cfg.guid,sizeof(int8_t)*UUIDBUFLENGTH);                      //提交的盒子guid号
        g_sys_sts.commit_data.dataindex ++;                                                   //上报信息序列号
        /*数据打包*/
        g_sys_sts.commit_data.alltime = g_sys_sts.col_info.all_tm;       //机器总时间
        g_sys_sts.commit_data.boottime = g_sys_sts.col_info.bt_tm;         //正常开机总时间
        g_sys_sts.commit_data.stoptime = g_sys_sts.col_info.st_tm;         //停机总时间
        g_sys_sts.commit_data.faulttime = g_sys_sts.col_info.ft_tm;       //故障总时间
				
				
        g_sys_sts.commit_data.bootstopsig = ((g_sys_sts.col_sts.bootsig !=0) && (g_sys_sts.col_sts.mainpulsesig!=0));       //设备状态开机或者停机
        g_sys_sts.commit_data.faultsig = g_sys_sts.col_sts.faultsig;	            //错误信号状态	
        g_sys_sts.commit_data.pulsecount = g_sys_sts.col_sts.mainpulsecount;     //脉冲数
			 
        //2019年11月30日   新增功能刀版脉冲
        g_sys_sts.commit_data.bladepulsecount = g_sys_sts.col_sts.bladepulsecount;     //刀版脉冲
			   
			 
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
				
        CommitBladeData_Upload(); //刀版数据提交
    }
	
	/*清除数据使能*/
    if (g_sys_sts.col_info.cl_en == TRUE) {
        g_sys_sts.col_info.cl_en = FALSE;
			  
        memcpy(g_sys_sts.commit_data.guid , g_sys_cfg.dev_cfg.guid,sizeof(int8_t)*UUIDBUFLENGTH);                      //提交的盒子guid号
        g_sys_sts.commit_data.dataindex ++;                                                   //上报信息序列号
        /*数据打包*/
        g_sys_sts.commit_data.alltime = g_sys_sts.col_info.all_tm;       //机器总时间
        g_sys_sts.commit_data.boottime = g_sys_sts.col_info.bt_tm;         //正常开机总时间
        g_sys_sts.commit_data.stoptime = g_sys_sts.col_info.st_tm;         //停机总时间
        g_sys_sts.commit_data.faulttime = g_sys_sts.col_info.ft_tm;       //故障总时间
				
				
        g_sys_sts.commit_data.bootstopsig = ((g_sys_sts.col_sts.bootsig !=0) && (g_sys_sts.col_sts.mainpulsesig!=0));       //设备状态开机或者停机
        g_sys_sts.commit_data.faultsig = g_sys_sts.col_sts.faultsig;	            //错误信号状态	
        g_sys_sts.commit_data.pulsecount = g_sys_sts.col_sts.mainpulsecount;     //脉冲数
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
        CommitClearData_Upload();  
    }	 
}


/*******************************************************************************
函数名称：Judge_Interval(RTC_DateTypeDef sdate_begin,RTC_DateTypeDef sdate_end)
功    能：判断间隔天数
说    明：
参    数: RTC_DateTypeDef sdate_begin:周、月、日、年记录开始
          RTC_DateTypeDef sdate_end  :周、月、日、年记录结束
返    回：上电后经过的时间
注    意：
*******************************************************************************/
u8 Judge_Interval(RTC_DateTypeDef sdate_begin,RTC_DateTypeDef sdate_end)
{
    if (sdate_begin.Month == 1   
    ||sdate_begin.Month == 3
    ||sdate_begin.Month == 5 
    ||sdate_begin.Month == 7 
    ||sdate_begin.Month == 8
    ||sdate_begin.Month == 10
    ||sdate_begin.Month == 12
    ) {
        if (sdate_begin.Date == 31 && sdate_end.Date == 1) {
            return 1; //间隔1天
        }
		
        else {
					return sdate_end.Date-sdate_begin.Date;
        }
    }

    else if (sdate_begin.Month == 2) {
		
        if ((sdate_begin.Year+2000)%4==0) {
			 
            if (sdate_begin.Date == 29 && sdate_end.Date == 1) {
                return 1; //间隔1天
            }
			
            else {
							return sdate_end.Date-sdate_begin.Date;
            }
        }
		
        else {
            if (sdate_begin.Date == 28 && sdate_end.Date == 1) {
					    return 1; //间隔1天
            }

            else {
				return sdate_end.Date-sdate_begin.Date;
            }
        }
    }
	
    else if (sdate_begin.Date == 30 && sdate_end.Date == 1) {
        return 1; //间隔1天 
    }
	
    else {
        return sdate_end.Date-sdate_begin.Date;	 
    }
}

/*******************************************************************************
函数名称：Collect_Judge_ClearData(RTC_DateTime _timebegin)
功    能：计数清零判断
说    明：
参    数: RTC_DateTime _timebegin:开始时间
返    回：TURE:时间大小正确 FALSE:错误
注    意：
*******************************************************************************/
u8 Collect_Judge_ClearData(RTC_DateTime _timebegin)
{
    RTC_DateTime _timecurrent;
    u16 _time1,_time2,_time3;
    HAL_RTC_GetDate(&hrtc, &_timecurrent.sdate, RTC_FORMAT_BIN);
    delay_ms(1);
    HAL_RTC_GetTime(&hrtc, &_timecurrent.stime, RTC_FORMAT_BIN);
	    
	
    _time1= (u16)(_timebegin.stime.Hours<<8) + _timebegin.stime.Minutes;
    _time2= (u16)(g_sys_cfg.col_cfg.cl_tm.hr<<8) + g_sys_cfg.col_cfg.cl_tm.min;
    _time3= (u16)(_timecurrent.stime.Hours<<8) + _timecurrent.stime.Minutes;
	     
    if (Judge_Interval(_timebegin.sdate,_timecurrent.sdate) > 1) {
        Collect_init();//检测数据初始化
        return TRUE;		
    }
	
    else if (Judge_Interval(_timebegin.sdate,_timecurrent.sdate) == 1) {
		
        if(_time3 >= _time2) {
            Collect_init();//检测数据初始化
            return TRUE;
        }
		
        else {
			return FALSE;
        }
    }
	
    else {
        if(_time3 >= _time2  && _time1 < _time2) {
            Collect_init();//检测数据初始化
            return TRUE;
        }
		
        else {
            return FALSE;	
        }
    }
}

/************************ (C) COPYRIGHT Hopewellgroup ***********END OF FILE***/

