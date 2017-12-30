#include "bsp.h"

//*************************************************************************
//	��ʼ��IO���ӳ���
//*************************************************************************
void Gpio_Init()
{ 
  //��ŵ����
    
  P1SEL &=~(BIT3+BIT4+BIT5+BIT6+BIT7);
  P1DIR |=(BIT3+BIT4+BIT5+BIT6+BIT7);  
  
  P2SEL &=~(BIT0+BIT1+BIT2+BIT3+BIT4);
  P2DIR |=(BIT0+BIT1+BIT2+BIT3+BIT4);  
  
  //WDT Out
  P5SEL &= ~BIT5;                   //P5.5����GPIO,WDT  
  P5DIR |=BIT5;                     //�������,WDT 
}

void DischgCtrl(INT8U chgIndex)
{
  P2OUT&=~(BIT0 + BIT1 + BIT2 + BIT3 + BIT4);//ȫ���ر�
  switch(chgIndex)
  {
  case 0:
    P2OUT|=BIT4;
    break;
  case 1:
    P2OUT|=BIT3;
    break;
  case 2:
    P2OUT|=BIT2;
    break;
  case 3:
    P2OUT|=BIT1;
    break;
  case 4://���ñ��õ��
    P2OUT|=BIT0;
    break;  
  default:  
    break;
  }
}


void ChgCtrl(INT8U dischgIndex)
{
  P1OUT&=~(BIT3 + BIT4 + BIT5 + BIT6 + BIT7);
  switch(dischgIndex)
  {
  case 0:
    P1OUT|=BIT3;
    break;
  case 1:
    P1OUT|=BIT4;
    break;
  case 2:
    P1OUT|=BIT5;
    break;
  case 3:
    P1OUT|=BIT6;
  case 4://ǿ�ƿ��ƿ���
    P1OUT|=BIT7;    
    break;
  default:
    break;  
  } 
}