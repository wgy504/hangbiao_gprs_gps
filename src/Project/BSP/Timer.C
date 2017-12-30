#include "bsp.h"
BOOL TimerB_1S_Flag=FALSE;
BOOL TimerB_125mS_Flag=FALSE;

/*
TA������ģ�飬����CCR0�������������PWM��ֻ���������ģʽ���á�
�����ǵ͵�ƽģʽ�������Ϊ�ߵ�ƽģʽ����
1.��ʼ��ʱ��Mode=P
2.TimerA_PwmStartStop,ֹͣʱ����Ϊ�͵�ƽ
*/
BOOL TimerA_PwmInit(char Clk,char Div,char Mode)
{
  TACTL = 0; //�����ǰ����
  TACTL |= MC_1; //��ʱ�� TA ��Ϊ������ģʽ
  switch(Clk) //ѡ��ʱ��Դ
  {
    case'A': case'a': TACTL|=TASSEL_1; break; //ACLK
    case'S': case's': TACTL|=TASSEL_2; break; //SMCLK
    case'E': TACTL|=TASSEL_0; break; //�ⲿ����(TACLK)
    case'e': TACTL|=TASSEL_3; break; //�ⲿ����(TACLK ȡ��)
    default: return FALSE; //��������
  }
  
  switch(Div) //ѡ���Ƶϵ��
  {
    case 1: TACTL|=ID_0; break; //1
    case 2: TACTL|=ID_1; break; //2
    case 4: TACTL|=ID_2; break; //4
    case 8: TACTL|=ID_3; break; //8
    default: return FALSE; //��������
  }
  switch(Mode) //���� PWM ͨ��1�����ģʽ��
  {
    case 'P':
    case 'p': //�������Ϊ�ߵ�ƽģʽ
      TACCTL1 = OUTMOD_7; //�ߵ�ƽ PWM ���
      P1SEL |= BIT2; //�� P1.2��� (��ͬ�ͺŵ�Ƭ�����ܲ�һ��)
      P1DIR |= BIT2; //�� P1.2��� (��ͬ�ͺŵ�Ƭ�����ܲ�һ��)
      break;
    case 'N':
    case 'n': //�������Ϊ�͵�ƽģʽ   
      TACCTL1 = OUTMOD_3; //�͵�ƽPWM�����P1SEL|= BIT2;
      P1SEL |= BIT2; //�� P1.2��� (��ͬ�ͺŵ�Ƭ�����ܲ�һ��)
      P1DIR |= BIT2; //�� P1.2��� (��ͬ�ͺŵ�Ƭ�����ܲ�һ��)
      break;
    case '0':
    case 0: //�������Ϊ����
      P1SEL &= ~BIT2; //P1.2�ָ�Ϊ��ͨ IO ��
      break;
    default: 
      return FALSE; //��������
  }

  return TRUE;

}

BOOL TimerA_PwmStartStop(BOOL startFlag)
{
  
  if(startFlag)
  {
    TACTL |= MC_1; //��ʱ�� TA ��Ϊ������ģʽ,������ʱ   
    P1SEL |= BIT2; //�� P1.2��� (��ͬ�ͺŵ�Ƭ�����ܲ�һ��)
  }
  else
  {
    TACTL &= ~MC_1; //��ʱ�� TA ��Ϊֹͣģʽ
    P1SEL &= ~BIT2; //P1.2�ָ�Ϊ��ͨ IO ��
    P1DIR |= BIT2; //�� P1.2��� (��ͬ�ͺŵ�Ƭ�����ܲ�һ��)
    //P1OUT |= BIT2;//���ߣ���Ϊ�Ƿ���     
    
    P1OUT &= ~BIT2;//���ߣ���Ϊ�Ƿ���     
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
  //TBCTL=TBSSEL1+ID1+ID0+MC0+TBCLR;//ѡ��1/8SMCLK ������ ���TAR
  TBCTL=TBSSEL_1+CNTL_0+TBCLGRP_0;//+ID_3;//ʱ��Դѡ��ACLK��16λ��ʱ����8��Ƶ

  TBCCTL0=CCIE;//CCR0�ж����� �Ƚ�ģʽ
  
    TBCTL|=TBCLR;//��ʱ������
    TBCCR0=130;//16*8;//ʱ����1/256s  32768/8=4096,
    TBCTL|=MC_1;//������ʱ��
}

