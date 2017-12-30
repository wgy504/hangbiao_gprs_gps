#include "bsp.h"
#include <stdlib.h>
#include <stdarg.h>

#include "stdarg.h"
#include "stdio.h"
#include "..\CLIB\icclbutl.h"

/*************************************************************
* �� �ƣ�SetBaudRateRegisters
* �� �ܣ�����ʱ�� ���������ö�Ӧ�Ĵ���
* ��ڲ�����
* clk: ѡʱ��Ƶ��(�磺32768)baud ������ (300~115200)
* ���ڲ�������
* �� ��: SetBaudRateRegisters(32768,9600) //��ʱ��Ƶ��32768����9600��
������
*************************************************/
void SetBaudRateRegisters(uchar portNo,long clk,long bauds)
{
  int n = clk / bauds; //����������
  char mSum = 0; //��mi
  int txEr0; //��ӦλΪ0ʱ������
  int txEr1; //��ӦλΪ1ʱ������
  char i = 0; //ѭ������
  if(portNo==0)
  {
    U0BR1 = n >> 8; //��8λ
    U0BR0 = n & 0xff; //��8λ
    U0MCTL = 0;  
  }
  else
  {
    U1BR1 = n >> 8; //��8λ
    U1BR0 = n & 0xff; //��8λ
    U1MCTL = 0;  
  }

  //ѭ�� �Ƚϴ����ʴ�С ���� UxMCTL
  for(;i < 8;i++)
  {
    txEr0 = 100 * bauds * ((i + 1) * n + mSum) / clk - 100 * (i + 1);
    txEr1 = 100 * bauds * ((i + 1) * n + mSum + 1) / clk - 100 * (i
    + 1);
    if(abs(txEr1) < abs(txEr0))
    {
      mSum++;
      if(portNo==0)
      {
        U0MCTL |= (1<<i);
      }
      else
      {
        U1MCTL |= (1<<i);
      }
    }
  }
}


BOOL Uart0_Init(long bauds,char parity,uchar dataBits,uchar stopBits)
{
  long brclk=0;
  U0TCTL &=~ (SSEL0+SSEL1); //���֮ǰ��ʱ������
  if(bauds<=9600) //brclk Ϊʱ��ԴƵ��
  {
    U0TCTL |= SSEL0; //ACLK�����͹���
    brclk = 32768; //�����ʷ�����ʱ��Ƶ��=ACLK(32768)
  }
  else
  {
    U0TCTL |= SSEL1; //SMCLK����֤�ٶ�
    brclk = 1000000; //�����ʷ�����ʱ��Ƶ��=SMCLK(1MHz)
  }
  if(bauds<300||bauds>115200)//�����ʳ�����Χ
  {
    return FALSE;
  }

  SetBaudRateRegisters(0,brclk,bauds); //���ò����ʼĴ���
  switch(parity)
  {
    case 'n': //��У��
    case 'N': 
      U0CTL &=~ PENA; 
      break;
    case 'p': //żУ��
    case'P': 
      U0CTL |= PENA + PEV; 
      break;
    case 'o':
    case 'O': 
      U0CTL |= PENA; 
      U0CTL &=~ PEV; 
      break; //��У��
  default: 
    return FALSE; //��������
  }
  
  //------------------------��������λ-------------------------
  
  switch(dataBits)
  {
    case 7:
      U0CTL &=~ CHAR; 
      break; //7λ����
    case 8:
      U0CTL |= CHAR; 
      break; //8λ����
    default: 
      return FALSE; //��������
  }
  //------------------------����ֹͣλ-------------------------
  switch(stopBits)
  {
    case 1:
      U0CTL &= ~SPB; break; //1λֹͣλ
    case 2:
      U0CTL |= SPB; break; //2λֹͣλ
    default: 
      return FALSE; //��������
  } 
  
  P3SEL |= 0X30; // P3.4,5 = USART0 TXD/RXD //�˿�ʹ��
  U0ME |= UTXE0 + URXE0; //���� ����ʹ��
  U0CTL &= ~SWRST; // Initialize USART state machine
  U0IE |= URXIE0;// + UTXIE0; // Enable USART0 RX interrupt
  return TRUE;
}

BOOL Uart1_Init(long bauds,char parity,uchar dataBits,uchar stopBits)
{
  long brclk=0;
  U1TCTL &=~ (SSEL0+SSEL1); //���֮ǰ��ʱ������
  if(bauds<=9600) //brclk Ϊʱ��ԴƵ��
  {
    U1TCTL |= SSEL0; //ACLK�����͹���
    brclk = 32768; //�����ʷ�����ʱ��Ƶ��=ACLK(32768)
  }
  else
  {
    U1TCTL |= SSEL1; //SMCLK����֤�ٶ�
    brclk = 1000000; //�����ʷ�����ʱ��Ƶ��=SMCLK(1MHz)
  }
  if(bauds<300||bauds>115200)//�����ʳ�����Χ
  {
    return FALSE;
  }

  SetBaudRateRegisters(1,brclk,bauds); //���ò����ʼĴ���
  switch(parity)
  {
    case 'n': //��У��
    case 'N': 
      U1CTL &=~ PENA; 
      break;
    case 'p': //żУ��
    case'P': 
      U1CTL |= PENA + PEV; 
      break;
    case 'o':
    case 'O': 
      U1CTL |= PENA; 
      U1CTL &=~ PEV; 
      break; //��У��
  default: 
    return FALSE; //��������
  }
  
  //------------------------��������λ-------------------------
  
  switch(dataBits)
  {
    case 7:
      U1CTL &=~ CHAR; 
      break; //7λ����
    case 8:
      U1CTL |= CHAR; 
      break; //8λ����
    default: 
      return FALSE; //��������
  }
  //------------------------����ֹͣλ-------------------------
  switch(stopBits)
  {
    case 1:
      U1CTL &= ~SPB; break; //1λֹͣλ
    case 2:
      U1CTL |= SPB; break; //2λֹͣλ
    default: 
      return FALSE; //��������
  } 
  
  P3SEL |= 0XC0; // P3.6,7 = USART1 TXD/RXD //�˿�ʹ��
  P3DIR |=BIT6;// ���
  P3DIR &=BIT7;//����
  U1ME |= UTXE1 + URXE1; //���� ����ʹ��
  U1CTL &= ~SWRST; // Initialize USART state machine
  U1IE |= URXIE1;// + UTXIE1; // Enable USART0 RX interrupt
  return TRUE;
}



void Uart0_SendByte(uchar dat)
{
  //while((IFG1&UTXIFG0)!=0x80);
  
  while((IFG1&UTXIFG0)==0);
  U0TXBUF=dat;
}
void Uart0_SendStr(uchar *s)
{
  while(*s)
  {
    Uart0_SendByte(*s++);
  }
}


void Uart1_SendByte(uchar dat)
{
  //while((IFG1&UTXIFG0)!=0x80);
  
  while((IFG2&UTXIFG1)==0);
  U1TXBUF=dat;
}

void Uart1_SendStr(uchar *s)
{
  while(*s)
  {
    Uart1_SendByte(*s++);
  }
}

/*
* ��������itoa
* ����  ������������ת�����ַ���
* ����  ��-radix =10 ��ʾ10���ƣ��������Ϊ0
*         -value Ҫת����������
*         -buf ת������ַ���
*         -radix = 10
* ���  ����
* ����  ����
* ����  ����USART_printf()����
*/
static char *itoa(int value, char *string, int radix)
{
  int     i, d;
  int     flag = 0;
  char    *ptr = string;
  
  /* This implementation only works for decimal numbers. */
  if (radix != 10)
  {
    *ptr = 0;
    return string;
  }
  
  if (!value)
  {
    *ptr++ = 0x30;
    *ptr = 0;
    return string;
  }
  
  /* if this is a negative value insert the minus sign. */
  if (value < 0)
  {
    *ptr++ = '-';
    
    /* Make the value positive. */
    value *= -1;
  }
  
  for (i = 10000; i > 0; i /= 10)
  {
    d = value / i;
    
    if (d || flag)
    {
      *ptr++ = (char)(d + 0x30);
      value -= (d * i);
      flag = 1;
    }
  }
  
  /* Null terminate the string. */
  *ptr = 0;
  
  return string;
  
} /* NCL_Itoa */


void Uart1_printf(INT8U *Data,...)
{
  const char *s;
  int d;   
  char buf[16];
  
  va_list ap;
  va_start(ap, Data);
  
  while ( *Data != 0)     // �ж��Ƿ񵽴��ַ���������
  {                                                          
    if ( *Data == '\\' )  //'\'
    {                                                                          
      switch ( *++Data )
      {
      case 'r':                                                                  //�س���
        Uart1_SendByte(0x0d);
        Data ++;
        break;
        
      case 'n':                                                                  //���з�
        Uart1_SendByte(0x0a);        
        Data ++;
        break;
        
      default:
        Data ++;
        break;
      }                         
    }
    else if ( *Data == '%')
    {                                                                          //
      switch ( *++Data )
      {                                
      case 's':                                                                                  //�ַ���
        s = va_arg(ap, const char *);
        for ( ; *s; s++) 
        {
          Uart1_SendByte(*s);
        }
        Data++;
        break;
        
      case 'd':                                                                                //ʮ����
        d = va_arg(ap, int);
        itoa(d, buf, 10);
        for (s = buf; *s; s++) 
        {
          Uart1_SendByte(*s);
        }
        Data++;
        break;
      default:
        Data++;
        break;
      }                 
    } /* end of else if */
    else 
    {
      Uart1_SendByte(*Data++);
    }
  }
}