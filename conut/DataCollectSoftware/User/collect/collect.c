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
�������ƣ�Collect_config()
��    �ܣ��ɼ���������,�ϵ�����
˵    ����
��    ��: 
��    �أ�
ע    �⣺
*******************************************************************************/
void  Collect_config(void)
{
    //���ݴ������������ô���ʽ
	
    g_sys_cfg.col_cfg.pulserelate = RELATE_STATE;
    g_sys_cfg.col_cfg.up_prd = UPLOADTIME;
	
    g_sys_cfg.col_cfg.bt_typ = BOOTSENSOR; //�ߵ�ƽ��Ч
    g_sys_cfg.col_cfg.bt_wd = COLLECTSIGWIDTH; //��Ч�ź���С���
	
    g_sys_cfg.col_cfg.st_typ = STOPSENSOR; //�ߵ�ƽ��Ч
    g_sys_cfg.col_cfg.st_wd = COLLECTSIGWIDTH;//��Ч�ź���С���
	
    g_sys_cfg.col_cfg.ft_typ = FAULTSENSOR; //�ߵ�ƽ��Ч
    g_sys_cfg.col_cfg.ft_wd = COLLECTSIGWIDTH;//��Ч�ź���С���
		
    g_sys_cfg.col_cfg.m_ch_typ = MAINPULSESENSOR; //������������
    g_sys_cfg.col_cfg.m_ch_wd = COLLECTSIGWIDTH;//��Ч�ź���С���
    g_sys_cfg.col_cfg.m_ch_prd_min = COLLECTSIGMININTERVAL;//��С�źż��ʱ��
    g_sys_cfg.col_cfg.m_ch_prd_max = COLLECTSIGMAXINTERVAL;//����źż��ʱ��
	
	
    g_sys_cfg.col_cfg.r_ch_typ = REVPULSESENSOR;    //Ԥ������������
    g_sys_cfg.col_cfg.r_ch_wd = COLLECTSIGWIDTH; //��Ч�ź���С���
    g_sys_cfg.col_cfg.r_ch_prd_min = COLLECTSIGMININTERVAL;//��С�źż��ʱ��
    g_sys_cfg.col_cfg.r_ch_prd_max = COLLECTSIGMAXINTERVAL;//����źż��ʱ��
	
    //���������ź�  2019��11��30������
    g_sys_cfg.col_cfg.b_ch_typ = REVPULSESENSOR;    //�������������
    g_sys_cfg.col_cfg.b_ch_wd = COLLECTSIGWIDTH;            //������Ч�ź���С���
    g_sys_cfg.col_cfg.b_ch_prd_min = COLLECTSIGMININTERVAL;//��С�źż��ʱ��
    g_sys_cfg.col_cfg.b_ch_prd_max = COLLECTSIGMAXINTERVAL;//����źż��ʱ��
	
    g_sys_cfg.col_cfg.cl_tm.hr    = CLEARHOUR;      //����Сʱ
    g_sys_cfg.col_cfg.cl_tm.min= CLEARMINUTES;   //�������
    g_sys_cfg.col_cfg.rt_prd    = REALSTORETIME;  //ʵʱ�洢ʱ��
	
    //Ĭ����ֽ
    g_sys_sts.col_sts.pnopaperstate = TRUE;  
}


/*******************************************************************************
�������ƣ�Collect_init()
��    �ܣ��ɼ�������ݳ�ʼ��,�ϵ������ݳ�ʼ��
˵    ����
��    ��: 
��    �أ�
ע    �⣺
*******************************************************************************/
void  Collect_init(void)
{
    memset(&g_sys_sts.col_sts,0,sizeof(CollectSigState));
    memset(&g_sys_sts.col_info,0,sizeof(CollectData));
    //��״̬��ʼ��
    g_sys_sts.col_info.bt_old_sts = BOOTSIG_IN;
    g_sys_sts.col_info.st_old_sts = STOPSIG_IN;
    g_sys_sts.col_info.ft_old_sts = FAULTSIG_IN;
    //ʱ���ʼ��
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
�������ƣ�Collect_JudgeEdge(uint8_t oldsig,uint8_t currentsig)
��    �ܣ��ɼ�������ݣ������ж�
˵    ����
��    ��: oldsig:��һ�ε�״̬ currentsig:�����ź�
��    �أ�
ע    �⣺
*******************************************************************************/
COLLECTJUDGEEDGE Collect_JudgeEdge(uint8_t oldsig,uint8_t currentsig)
{
    /*ǰһ���ź�Ϊ0����һ���ź�Ϊ1����Ϊ�����ش���*/
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
�������ƣ�Collect_JudeSigWidth(u8 sigtype,u8 currentsig,u8 *judgelogic,u16 sigwidth,u16 *sigwidthcount)
��    �ܣ��ж��źſ��,�ɶ�ʱ��������ã�1MS����һ��
˵    ����
��    ��: sigtype:�ƶ�������ʽ currentsig:�����ź� judgelogic:�ж��߼�
��    �أ�״̬
ע    �⣺
*******************************************************************************/
u8 Collect_JudeSigWidth(u8 sigtype,u8 currentsig,u8 *judgelogic,u16 sigwidth,u16 *sigwidthcount)
{
    u8  _returnstate = FALSE;
    switch (sigtype) {
        case IO_NPN :       //�͵�ƽ��Ч
            break;		
			
        case IO_PNP :       //�ߵ�ƽ��Ч
            break;		
			
        case IO_HIGH_PULSE : //��������Ч
        
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
		
        case IO_LOW_PULSE :    //��������Ч	
        
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
�������ƣ�Collect_CountPwmWidth(u8 *judgelogic,u16 *pwm1width,u16 *pwm2width)
��    �ܣ��ж�����ռ�ձ�
˵    ����
��    ��: judgelogic:�ж��߼� pwm1width:PWM1���ε����� pwm2width:pwm2������
��    �أ�
ע    �⣺
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
�������ƣ�Collect_JudeSigValid(u8 currentjudgelogic, //��ǰ�߼�
                             u8 oldjudgelogic,       //���߼�
                             u16 *pwm1width,         //��һ���ֵ�ռ��ʱ��
                             u16 *pwm2width,         //�ڶ����ֵ�ռ��ʱ��    
                             u16 *maxduty,           //����ռ�ձ�
                             u16 *minduty,           //��С��ռ�ձ�
                             u16 *maxperiod          //������ڣ���С�����ڳ���������
                             )
��    �ܣ�ͨ�����ں������ж��ź��Ƿ���Ч
˵    ����
��    ��: judgelogic:�ж��߼� pwm1width:PWM1���ε����� pwm2width:pwm2������
��    �أ�
ע    �⣺
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
	
    /*�ж�һ��������ڽ���*/
    if ((oldjudgelogic == JUDGE_PWM_2) &&(currentjudgelogic == JUDGE_PWM_1)) {
        _period = *pwm1width + *pwm2width;
		  
        if ((_period < COLLECTMINPERIOD) || (_period > *maxperiod)) {
            *pwm1width = 0;//����ֵ����
            *pwm2width = 0;
            return FALSE;
        }
		  
        _duty =(float)(*pwm1width)/ _period *100;
        *pwm1width = 0;//����ֵ����
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
�������ƣ�Collect_JudeSigInterVal(u16 *sigintervalcount,
                                  u16 sigmininterval,
                                  uint16_t sigmaxinterval,u8 *sigstate)
��    �ܣ��ж��źż��,1MS����һ�Σ����ڼ���״̬�ж�
˵    ����
��    ��: sigintervalcount:ʱ���� sigmininterval:��С�ıȽ�ֵ
          sigmaxinterval:���Ƚ�ֵ sigstate:��ʱ
��    �أ�
ע    �⣺
*******************************************************************************/
void Collect_JudeSigInterVal(u16 *sigintervalcount,u16 sigmininterval,uint16_t sigmaxinterval,u8 *sigstate)
{
     /*�����źż��*/
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
          *sigstate = INVALID; //�źų�ʱ
     }
}


/*******************************************************************************
�������ƣ�Collect_TimerHandle()
��    �ܣ�1MS��ʱ������Ĺ��ڲɼ��źŴ�����
˵    ����
��    ��: 
��    �أ�
ע    �⣺
*******************************************************************************/
void Collect_TimerHandle(void)
{
    static uint8_t _mainlogic_old = JUDGE_EDGE;
    static uint8_t _revlogic_old = JUDGE_EDGE;
	 /*�����źŷǵ�ƽ״̬ȷ��*/
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
	
    /*ͣ���źŷǵ�ƽ״̬ȷ��*/
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

    /*�����źŷǵ�ƽ״̬ȷ��*/
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
    
    /*��������*/
	/*�������źŷǵ�ƽ״̬ȷ��*/
    if ((g_sys_cfg.col_cfg.m_ch_typ == IO_HIGH_PULSE) || (g_sys_cfg.col_cfg.m_ch_typ == IO_LOW_PULSE)) {

		/*�����������*/
		if (Collect_JudeSigWidth(g_sys_cfg.col_cfg.m_ch_typ,
        MAINPULSESIG_IN,
        &g_sys_sts.col_info.m_ch_trg_md,
        g_sys_cfg.col_cfg.m_ch_wd,
        &g_sys_sts.col_info.m_ch_wd
        ) == TRUE) {
        g_sys_sts.col_info.m_ch_tm_cnt  = 0; 
		
            /*��������״̬����*/
            if (g_sys_cfg.col_cfg.pulserelate == RELATE_STATE) {

				/*�����ź���Ч�ż���*/
                if (g_sys_sts.col_sts.bootsig == VALID) {
                    g_sys_sts.col_sts.mainpulsecount ++;
                }				
            }

			/*����ǰ����������*/
            else if (g_sys_cfg.col_cfg.pulserelate == RELATE_REVPULSE) {

				/*ǰһ��������������Ч�ż���*/
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
	
    /*2020��1��12������,����ռ�ձȼ��*/
    else if ((g_sys_cfg.col_cfg.m_ch_typ == IO_PWM_HIGH) || (g_sys_cfg.col_cfg.m_ch_typ == IO_PWM_LOW)) {
    Collect_CountPwmWidth(&g_sys_sts.col_info.m_ch_trg_md,
    &g_sys_sts.col_info.m_ch_pwm_h_tm,
    &g_sys_sts.col_info.m_ch_pwm_l_tm);//PWM����
			
        if (Collect_JudeSigValid(g_sys_sts.col_info.m_ch_trg_md, 
        _mainlogic_old,
        &g_sys_sts.col_info.m_ch_pwm_h_tm,
        &g_sys_sts.col_info.m_ch_pwm_l_tm, 
        &g_sys_cfg.col_cfg.m_ch_prd_max,
        &g_sys_cfg.col_cfg.m_ch_prd_min,
        &g_sys_cfg.col_cfg.m_ch_wd
        ) == TRUE) {

		    /*�����������*/
            g_sys_sts.col_info.m_ch_tm_cnt  = 0; 

			/*��������״̬����*/
            if(g_sys_cfg.col_cfg.pulserelate == RELATE_STATE) {

                /*�����ź���Ч�ż���*/
                if(g_sys_sts.col_sts.bootsig == VALID) {
                    g_sys_sts.col_sts.mainpulsecount ++;
                }
								
            }

            /*����ǰ����������*/	
            else if (g_sys_cfg.col_cfg.pulserelate == RELATE_REVPULSE) {

		    	 /*ǰһ��������������Ч�ż���*/
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
	
    /*����������*/
	/*���������źŷǵ�ƽ״̬ȷ��*/
    if ((g_sys_cfg.col_cfg.r_ch_typ == IO_HIGH_PULSE) || (g_sys_cfg.col_cfg.r_ch_typ == IO_LOW_PULSE)) {

		if (Collect_JudeSigWidth(g_sys_cfg.col_cfg.r_ch_typ,
        REVPULSESIG_IN,
        &g_sys_sts.col_info.r_ch_trg_md,
        g_sys_cfg.col_cfg.r_ch_wd,
        &g_sys_sts.col_info.r_ch_wd
        ) == TRUE) {
            g_sys_sts.col_info.r_ch_tm_cnt  = 0; //�����������
            g_sys_sts.col_sts.revpulsecount ++;
        }
				
        else {
					
        }
		
    }
	
    else if ((g_sys_cfg.col_cfg.r_ch_typ == IO_PWM_HIGH) || (g_sys_cfg.col_cfg.r_ch_typ == IO_PWM_LOW)) {
        Collect_CountPwmWidth(&g_sys_sts.col_info.r_ch_trg_md,
        &g_sys_sts.col_info.r_ch_pwm_h_tm,
        &g_sys_sts.col_info.r_ch_pwm_l_tm);//PWM����
			
        if (Collect_JudeSigValid(g_sys_sts.col_info.r_ch_trg_md, 
        _revlogic_old,
        &g_sys_sts.col_info.r_ch_pwm_h_tm,
        &g_sys_sts.col_info.r_ch_pwm_l_tm, 
        &g_sys_cfg.col_cfg.r_ch_prd_max,
        &g_sys_cfg.col_cfg.r_ch_prd_min,
        &g_sys_cfg.col_cfg.r_ch_wd
        ) == TRUE) {
            g_sys_sts.col_info.r_ch_tm_cnt  = 0; //�����������
            g_sys_sts.col_sts.revpulsecount ++;
        }
		
        else {
			
        }
		
        _revlogic_old = g_sys_sts.col_info.r_ch_trg_md;
		
		
		
    }
	
    else {
		
    }

	/*���������źŷǵ�ƽ״̬ȷ��*/
    if (g_sys_cfg.col_cfg.b_ch_typ > IO_PNP) {
		
        if (Collect_JudeSigWidth(g_sys_cfg.col_cfg.b_ch_typ,
        BLADEPULSESIG_IN,
        &g_sys_sts.col_info.b_ch_trg_md,
        g_sys_cfg.col_cfg.b_ch_wd,
        &g_sys_sts.col_info.b_ch_wd
        ) == TRUE) {
            g_sys_sts.col_info.b_ch_tm_cnt  = 0; //�����������
            g_sys_sts.col_sts.bladepulsecount ++;
					
            g_sys_sts.col_info.bladeenable = TRUE; //��������ʹ���ϴ�
        }
				
        else {
					
        }
		
    }
		
    /*�������źż��ʱ����*/
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
     
    /*Ԥ�������źż��ʱ����*/
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
		
    /*���������źż��ʱ����  2019��11��30������*/
    Collect_JudeSigInterVal(&g_sys_sts.col_info.b_ch_tm_cnt,
    g_sys_cfg.col_cfg.b_ch_prd_min,
    g_sys_cfg.col_cfg.b_ch_prd_max,
    &g_sys_sts.col_sts.bladepulsesig);	
}

/*******************************************************************************
�������ƣ�Collect_StateTimeAggregation()
��    �ܣ��ɼ�״̬ʱ���ۻ�,1S����һ��
˵    ����
��    ��: 
��    �أ�
ע    �⣺
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
	
    g_sys_sts.col_info.all_tm++;//��ʱ�� 
}

/*******************************************************************************
�������ƣ�Collect_JudeSigState(u8 *oldsig,u8 currentsig,
                               u8 sigtype,u8 *judgelogic,u8 *sigstate)
��    �ܣ��ж��ź�״̬
˵    ����oldsig:�ϴ�״̬ currentsig:�����ź� sigtype:�ź����� judgelogic:�ж��߼�
          sigstate:��ƽ״̬
��    ��: 
��    �أ�
ע    �⣺
*******************************************************************************/
void Collect_JudeSigState(u8 *oldsig,u8 currentsig,u8 sigtype,u8 *judgelogic,u8 *sigstate)
{
    switch (sigtype) {
		
        case IO_NPN :       //�͵�ƽ��Ч
            if(currentsig == HIGHLEVEL) *sigstate = INVALID;
            else *sigstate = VALID;
            *judgelogic = JUDGE_EDGE;
        break;	
		
        case IO_PNP :       //�ߵ�ƽ��Ч
            if(currentsig == HIGHLEVEL) *sigstate = VALID;
            else *sigstate = INVALID;
            *judgelogic = JUDGE_EDGE;
        break;	
		
        case IO_HIGH_PULSE : //��������Ч
            if(Collect_JudgeEdge(*oldsig,currentsig) == RISE_EDGE) {
                *judgelogic = JUDGE_WITCH;
            }
        break;	
		
        case IO_LOW_PULSE :    //��������Ч	
            if(Collect_JudgeEdge(*oldsig,currentsig) == DESC_EDGE) {
                *judgelogic = JUDGE_WITCH;
            }
        break;
			
        default:
        break;
    }
    *oldsig = currentsig;//����ԭʼ�ź�
}

/*******************************************************************************
�������ƣ�Collect_Jude (u8 currentsig,u8 sigtype,u8 *judgelogic)
��    �ܣ������ⲿ�ж��жϣ������ⲿIO�ж�
˵    ����
��    ��: currentsig:�����ź� sigtype:�ź����� judgelogic:�ж��߼�
��    �أ�
ע    �⣺
*******************************************************************************/
void  Collect_Jude (u8 currentsig,u8 sigtype,u8 *judgelogic)
{
    switch (sigtype) {
        case IO_NPN :       //�͵�ƽ��Ч
        break;		
		
	    case IO_PNP :       //�ߵ�ƽ��Ч
        break;				
		
        case IO_HIGH_PULSE : //��������Ч
        
            if (currentsig == HIGHLEVEL) {
                *judgelogic = JUDGE_WITCH;
            }
        break;	
		
        case IO_LOW_PULSE :    //��������Ч	
        
            if(currentsig == LOWLEVEL) {
                *judgelogic = JUDGE_WITCH;
            }
        break;
		
        case IO_PWM_HIGH :    //�����
        
            if(currentsig == HIGHLEVEL) {
                *judgelogic = JUDGE_PWM_1;
            }
			
            else {
                *judgelogic = JUDGE_PWM_2;
            }
        break;
		
        case IO_PWM_LOW :    //�����
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
�������ƣ�Collect_ExitHandle (SIGINTERFACE siginterface)
��    �ܣ��жϴ�����
˵    ����
��    ��: SIGINTERFACE siginterface:�ɼ��ź�
��    �أ�
ע    �⣺
*******************************************************************************/
void  Collect_ExitHandle (SIGINTERFACE siginterface)
{
    switch(siginterface) {
		case BOOTSIG :		 //�����ź�
			break;			
	
		case STOPSIG :		 //ͣ���ź�
			break;		
	
		case FAULTSIG : 	 //�����ź�
			break;				
	
		case MAINPULSESIG :  //�������ź�
			collect_set_tooth_int_cnt();
			break;
		
		case RESERVEDPULSESIG:
			collect_set_paper_int_cnt();
			break;
		
		case REVPULSESIG :	 //���������ź�
			collect_set_qt_intcnt();
			break;
	
		case BLADEPULSESIG : //��������ź�
			collect_set_ep_intcnt();
			break;
	
		default:
			break;

    }
}

/*******************************************************************************
�������ƣ�Collect_Panpaper ()
��    �ܣ�����ֽ���
˵    ����
��    ��: 
��    �أ�
ע    �⣺
*******************************************************************************/
void  Collect_Panpaper(void)
{
    static u32 lastcount;
	
	/*�������ݱ仯*/
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
�������ƣ�Collect_Handle()
��    �ܣ��ɼ�״̬���ݶ�ȡ
˵    ����
��    ��: 
��    �أ�
ע    �⣺
*******************************************************************************/
void  Collect_Handle(void)
{
    /*�����ź�*/
    Collect_JudeSigState(&g_sys_sts.col_info.bt_old_sts,
                        BOOTSIG_IN,
                        g_sys_cfg.col_cfg.bt_typ,
                        &g_sys_sts.col_info.bt_trg_md,
                        &g_sys_sts.col_sts.bootsig
                        );
	
    /*�ػ��ź�*/
    Collect_JudeSigState(&g_sys_sts.col_info.st_old_sts,
                        STOPSIG_IN,
                        g_sys_cfg.col_cfg.st_typ,
                        &g_sys_sts.col_info.st_trg_md,
                        &g_sys_sts.col_sts.stopsig
                        );
    /*�����ź�*/
    Collect_JudeSigState(&g_sys_sts.col_info.ft_old_sts,
                        FAULTSIG_IN,
                        g_sys_cfg.col_cfg.ft_typ,
                        &g_sys_sts.col_info.ft_trg_md,
                        &g_sys_sts.col_sts.faultsig
                        );		
    /*����ֽ���*/
    Collect_Panpaper();	 
}

/*******************************************************************************
�������ƣ�Collect_UploadTimeCount()
��    �ܣ��ɼ����ݶ�ʱ�ϱ�
˵    ����
��    ��: 
��    �أ�
ע    �⣺
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
�������ƣ�Collect_RealTimeStore()
��    �ܣ����ݶ�ʱ����
˵    ����
��    ��: 
��    �أ�
ע    �⣺
*******************************************************************************/
void Collect_RealTimeStore(void)
{
    HAL_RTC_GetDate(&hrtc, &g_sys_sts.realtime_data.MachineCurrent.sdate, RTC_FORMAT_BIN);
    delay_ms(1);
    HAL_RTC_GetTime(&hrtc, &g_sys_sts.realtime_data.MachineCurrent.stime, RTC_FORMAT_BIN);
	      
    g_sys_sts.realtime_data.dataindex     =  g_sys_sts.commit_data.dataindex;//���ݴ��                                                   //�ϱ���Ϣ���к�
    
    g_sys_sts.realtime_data.alltime      =   g_sys_sts.col_info.all_tm;      //������ʱ��
    g_sys_sts.realtime_data.bootsigtime  =   g_sys_sts.col_info.bt_tm;       //����������ʱ��
    g_sys_sts.realtime_data.stopsigtime  =   g_sys_sts.col_info.st_tm;       //ͣ����ʱ��
    g_sys_sts.realtime_data.faultsigtime =   g_sys_sts.col_info.ft_tm;       //������ʱ��
				
				
    g_sys_sts.realtime_data.bootstopsig = ((g_sys_sts.col_sts.bootsig !=0) && (g_sys_sts.col_sts.mainpulsesig!=0));       //�豸״̬��������ͣ��
    g_sys_sts.realtime_data.faultsig = g_sys_sts.col_sts.faultsig;	            //�����ź�״̬	
    g_sys_sts.realtime_data.pulsecount = g_sys_sts.col_sts.mainpulsecount;     //������

    g_sys_sts.realtime_data.bladepulsecount = g_sys_sts.col_sts.bladepulsecount;     //������

    //EEPROM_writeRomData(EEPROM_FIRST_CHIP | EEPROM_DATA,(u8 *)&g_sys_sts.realtime_data,sizeof(RealTimeStore));

}

/*******************************************************************************
�������ƣ�Collect_UploadHandle()
��    �ܣ������ϱ�������
˵    ����
��    ��: 
��    �أ�
ע    �⣺
*******************************************************************************/
void Collect_UploadHandle(void)
{
    /*�����ϱ�*/
    if (g_sys_sts.col_info.upload_en == TRUE) {
			  
        g_sys_sts.col_info.upload_en = FALSE;
        memcpy(g_sys_sts.commit_data.guid , g_sys_cfg.dev_cfg.guid,sizeof(int8_t)*UUIDBUFLENGTH);                      //�ύ�ĺ���guid��
        g_sys_sts.commit_data.dataindex ++;                                                   //�ϱ���Ϣ���к�
        /*���ݴ��*/
        g_sys_sts.commit_data.alltime = g_sys_sts.col_info.all_tm;       //������ʱ��
        g_sys_sts.commit_data.boottime = g_sys_sts.col_info.bt_tm;         //����������ʱ��
        g_sys_sts.commit_data.stoptime = g_sys_sts.col_info.st_tm;         //ͣ����ʱ��
        g_sys_sts.commit_data.faulttime = g_sys_sts.col_info.ft_tm;       //������ʱ��
				
				
        g_sys_sts.commit_data.bootstopsig = ((g_sys_sts.col_sts.bootsig !=0) && (g_sys_sts.col_sts.mainpulsesig!=0));       //�豸״̬��������ͣ��
        g_sys_sts.commit_data.faultsig = g_sys_sts.col_sts.faultsig;	            //�����ź�״̬	
        g_sys_sts.commit_data.pulsecount = g_sys_sts.col_sts.mainpulsecount;     //������
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

    /*ʵʱ����*/
    if (g_sys_sts.col_info.storeenable == TRUE) {	     
        g_sys_sts.col_info.storeenable = FALSE;
        Collect_RealTimeStore();
    } 

    /*���������ϱ�*/	 
    if (g_sys_sts.col_info.bladeenable == TRUE) {
        g_sys_sts.col_info.bladeenable = FALSE;
			  
        memcpy(g_sys_sts.commit_data.guid , g_sys_cfg.dev_cfg.guid,sizeof(int8_t)*UUIDBUFLENGTH);                      //�ύ�ĺ���guid��
        g_sys_sts.commit_data.dataindex ++;                                                   //�ϱ���Ϣ���к�
        /*���ݴ��*/
        g_sys_sts.commit_data.alltime = g_sys_sts.col_info.all_tm;       //������ʱ��
        g_sys_sts.commit_data.boottime = g_sys_sts.col_info.bt_tm;         //����������ʱ��
        g_sys_sts.commit_data.stoptime = g_sys_sts.col_info.st_tm;         //ͣ����ʱ��
        g_sys_sts.commit_data.faulttime = g_sys_sts.col_info.ft_tm;       //������ʱ��
				
				
        g_sys_sts.commit_data.bootstopsig = ((g_sys_sts.col_sts.bootsig !=0) && (g_sys_sts.col_sts.mainpulsesig!=0));       //�豸״̬��������ͣ��
        g_sys_sts.commit_data.faultsig = g_sys_sts.col_sts.faultsig;	            //�����ź�״̬	
        g_sys_sts.commit_data.pulsecount = g_sys_sts.col_sts.mainpulsecount;     //������
			 
        //2019��11��30��   �������ܵ�������
        g_sys_sts.commit_data.bladepulsecount = g_sys_sts.col_sts.bladepulsecount;     //��������
			   
			 
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
				
        CommitBladeData_Upload(); //���������ύ
    }
	
	/*�������ʹ��*/
    if (g_sys_sts.col_info.cl_en == TRUE) {
        g_sys_sts.col_info.cl_en = FALSE;
			  
        memcpy(g_sys_sts.commit_data.guid , g_sys_cfg.dev_cfg.guid,sizeof(int8_t)*UUIDBUFLENGTH);                      //�ύ�ĺ���guid��
        g_sys_sts.commit_data.dataindex ++;                                                   //�ϱ���Ϣ���к�
        /*���ݴ��*/
        g_sys_sts.commit_data.alltime = g_sys_sts.col_info.all_tm;       //������ʱ��
        g_sys_sts.commit_data.boottime = g_sys_sts.col_info.bt_tm;         //����������ʱ��
        g_sys_sts.commit_data.stoptime = g_sys_sts.col_info.st_tm;         //ͣ����ʱ��
        g_sys_sts.commit_data.faulttime = g_sys_sts.col_info.ft_tm;       //������ʱ��
				
				
        g_sys_sts.commit_data.bootstopsig = ((g_sys_sts.col_sts.bootsig !=0) && (g_sys_sts.col_sts.mainpulsesig!=0));       //�豸״̬��������ͣ��
        g_sys_sts.commit_data.faultsig = g_sys_sts.col_sts.faultsig;	            //�����ź�״̬	
        g_sys_sts.commit_data.pulsecount = g_sys_sts.col_sts.mainpulsecount;     //������
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
�������ƣ�Judge_Interval(RTC_DateTypeDef sdate_begin,RTC_DateTypeDef sdate_end)
��    �ܣ��жϼ������
˵    ����
��    ��: RTC_DateTypeDef sdate_begin:�ܡ��¡��ա����¼��ʼ
          RTC_DateTypeDef sdate_end  :�ܡ��¡��ա����¼����
��    �أ��ϵ�󾭹���ʱ��
ע    �⣺
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
            return 1; //���1��
        }
		
        else {
					return sdate_end.Date-sdate_begin.Date;
        }
    }

    else if (sdate_begin.Month == 2) {
		
        if ((sdate_begin.Year+2000)%4==0) {
			 
            if (sdate_begin.Date == 29 && sdate_end.Date == 1) {
                return 1; //���1��
            }
			
            else {
							return sdate_end.Date-sdate_begin.Date;
            }
        }
		
        else {
            if (sdate_begin.Date == 28 && sdate_end.Date == 1) {
					    return 1; //���1��
            }

            else {
				return sdate_end.Date-sdate_begin.Date;
            }
        }
    }
	
    else if (sdate_begin.Date == 30 && sdate_end.Date == 1) {
        return 1; //���1�� 
    }
	
    else {
        return sdate_end.Date-sdate_begin.Date;	 
    }
}

/*******************************************************************************
�������ƣ�Collect_Judge_ClearData(RTC_DateTime _timebegin)
��    �ܣ����������ж�
˵    ����
��    ��: RTC_DateTime _timebegin:��ʼʱ��
��    �أ�TURE:ʱ���С��ȷ FALSE:����
ע    �⣺
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
        Collect_init();//������ݳ�ʼ��
        return TRUE;		
    }
	
    else if (Judge_Interval(_timebegin.sdate,_timecurrent.sdate) == 1) {
		
        if(_time3 >= _time2) {
            Collect_init();//������ݳ�ʼ��
            return TRUE;
        }
		
        else {
			return FALSE;
        }
    }
	
    else {
        if(_time3 >= _time2  && _time1 < _time2) {
            Collect_init();//������ݳ�ʼ��
            return TRUE;
        }
		
        else {
            return FALSE;	
        }
    }
}

/************************ (C) COPYRIGHT Hopewellgroup ***********END OF FILE***/

