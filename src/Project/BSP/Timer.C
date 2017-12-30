#include "bsp.h"
BOOL TimerB_1S_Flag=FALSE;
BOOL TimerB_125mS_Flag=FALSE;

/*
TA有三个模块，其中CCR0不能完整的输出PWM，只有三种输出模式可用。
现在是低电平模式，如果改为高电平模式，则
1.初始化时候Mode=P
2.TimerA_PwmStartStop,停止时，设为低电平
*/
BOOL TimerA_PwmInit(char Clk,char Div,char Mode)
{
  TACTL = 0; //清除以前设置
  TACTL |= MC_1; //定时器 TA 设为增计数模式
  switch(Clk) //选择时钟源
  {
    case'A': case'a': TACTL|=TASSEL_1; break; //ACLK
    case'S': case's': TACTL|=TASSEL_2; break; //SMCLK
    case'E': TACTL|=TASSEL_0; break; //外部输入(TACLK)
    case'e': TACTL|=TASSEL_3; break; //外部输入(TACLK 取反)
    default: return FALSE; //参数有误
  }
  
  switch(Div) //选择分频系数
  {
    case 1: TACTL|=ID_0; break; //1
    case 2: TACTL|=ID_1; break; //2
    case 4: TACTL|=ID_2; break; //4
    case 8: TACTL|=ID_3; break; //8
    default: return FALSE; //参数有误
  }
  switch(Mode) //设置 PWM 通道1的输出模式。
  {
    case 'P':
    case 'p': //如果设置为高电平模式
      TACCTL1 = OUTMOD_7; //高电平 PWM 输出
      P1SEL |= BIT2; //从 P1.2输出 (不同型号单片机可能不一样)
      P1DIR |= BIT2; //从 P1.2输出 (不同型号单片机可能不一样)
      break;
    case 'N':
    case 'n': //如果设置为低电平模式   
      TACCTL1 = OUTMOD_3; //低电平PWM输出，P1SEL|= BIT2;
      P1SEL |= BIT2; //从 P1.2输出 (不同型号单片机可能不一样)
      P1DIR |= BIT2; //从 P1.2输出 (不同型号单片机可能不一样)
      break;
    case '0':
    case 0: //如果设置为禁用
      P1SEL &= ~BIT2; //P1.2恢复为普通 IO 口
      break;
    default: 
      return FALSE; //参数有误
  }

  return TRUE;

}

BOOL TimerA_PwmStartStop(BOOL startFlag)
{
  
  if(startFlag)
  {
    TACTL |= MC_1; //定时器 TA 设为增计数模式,启动定时   
    P1SEL |= BIT2; //从 P1.2输出 (不同型号单片机可能不一样)
  }
  else
  {
    TACTL &= ~MC_1; //定时器 TA 设为停止模式
    P1SEL &= ~BIT2; //P1.2恢复为普通 IO 口
    P1DIR |= BIT2; //从 P1.2输出 (不同型号单片机可能不一样)
    //P1OUT |= BIT2;//拉高，因为是反相     
    
    P1OUT &= ~BIT2;//拉高，因为是反相     
  }
  return TRUE; 
}

void TimerA_PwmSetPeriod(INT16U Period)
{
  TACCR0 = Period;
}

void TimerA_PwmSetDuty(char Channel,INT16U Duty)
{
  switch(Channel)
  {
    case 1: TACCR1=Duty; break;
    case 2: TACCR2=Duty; break;
  }
}

void TimerB_Init()
{
  //TBCTL=TBSSEL1+ID1+ID0+MC0+TBCLR;//选择1/8SMCLK 增计数 清除TAR
  TBCTL=TBSSEL_1+CNTL_0+TBCLGRP_0;//+ID_3;//时钟源选择ACLK，16位定时器，8分频

  TBCCTL0=CCIE;//CCR0中断允许 比较模式
  
    TBCTL|=TBCLR;//定时器清零
    TBCCR0=130;//16*8;//时间间隔1/256s  32768/8=4096,
    TBCTL|=MC_1;//启动定时器
}


