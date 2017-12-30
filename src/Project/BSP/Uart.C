#include "bsp.h"
#include <stdlib.h>
#include <stdarg.h>

#include "stdarg.h"
#include "stdio.h"
#include "..\CLIB\icclbutl.h"

/*************************************************************
* 名 称：SetBaudRateRegisters
* 功 能：根据时钟 波特率设置对应寄存器
* 入口参数：
* clk: 选时钟频率(如：32768)baud 波特率 (300~115200)
* 出口参数：无
* 范 例: SetBaudRateRegisters(32768,9600) //用时钟频率32768产生9600的
波特率
*************************************************/
void SetBaudRateRegisters(uchar portNo,long clk,long bauds)
{
  int n = clk / bauds; //整数波特率
  char mSum = 0; //Σmi
  int txEr0; //对应位为0时错误率
  int txEr1; //对应位为1时错误率
  char i = 0; //循环计数
  if(portNo==0)
  {
    U0BR1 = n >> 8; //高8位
    U0BR0 = n & 0xff; //低8位
    U0MCTL = 0;  
  }
  else
  {
    U1BR1 = n >> 8; //高8位
    U1BR0 = n & 0xff; //低8位
    U1MCTL = 0;  
  }

  //循环 比较错误率大小 设置 UxMCTL
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
  U0TCTL &=~ (SSEL0+SSEL1); //清除之前的时钟设置
  if(bauds<=9600) //brclk 为时钟源频率
  {
    U0TCTL |= SSEL0; //ACLK，降低功耗
    brclk = 32768; //波特率发生器时钟频率=ACLK(32768)
  }
  else
  {
    U0TCTL |= SSEL1; //SMCLK，保证速度
    brclk = 1000000; //波特率发生器时钟频率=SMCLK(1MHz)
  }
  if(bauds<300||bauds>115200)//波特率超出范围
  {
    return FALSE;
  }

  SetBaudRateRegisters(0,brclk,bauds); //设置波特率寄存器
  switch(parity)
  {
    case 'n': //无校验
    case 'N': 
      U0CTL &=~ PENA; 
      break;
    case 'p': //偶校验
    case'P': 
      U0CTL |= PENA + PEV; 
      break;
    case 'o':
    case 'O': 
      U0CTL |= PENA; 
      U0CTL &=~ PEV; 
      break; //奇校验
  default: 
    return FALSE; //参数错误
  }
  
  //------------------------设置数据位-------------------------
  
  switch(dataBits)
  {
    case 7:
      U0CTL &=~ CHAR; 
      break; //7位数据
    case 8:
      U0CTL |= CHAR; 
      break; //8位数据
    default: 
      return FALSE; //参数错误
  }
  //------------------------设置停止位-------------------------
  switch(stopBits)
  {
    case 1:
      U0CTL &= ~SPB; break; //1位停止位
    case 2:
      U0CTL |= SPB; break; //2位停止位
    default: 
      return FALSE; //参数错误
  } 
  
  P3SEL |= 0X30; // P3.4,5 = USART0 TXD/RXD //端口使能
  U0ME |= UTXE0 + URXE0; //发送 接收使能
  U0CTL &= ~SWRST; // Initialize USART state machine
  U0IE |= URXIE0;// + UTXIE0; // Enable USART0 RX interrupt
  return TRUE;
}

BOOL Uart1_Init(long bauds,char parity,uchar dataBits,uchar stopBits)
{
  long brclk=0;
  U1TCTL &=~ (SSEL0+SSEL1); //清除之前的时钟设置
  if(bauds<=9600) //brclk 为时钟源频率
  {
    U1TCTL |= SSEL0; //ACLK，降低功耗
    brclk = 32768; //波特率发生器时钟频率=ACLK(32768)
  }
  else
  {
    U1TCTL |= SSEL1; //SMCLK，保证速度
    brclk = 1000000; //波特率发生器时钟频率=SMCLK(1MHz)
  }
  if(bauds<300||bauds>115200)//波特率超出范围
  {
    return FALSE;
  }

  SetBaudRateRegisters(1,brclk,bauds); //设置波特率寄存器
  switch(parity)
  {
    case 'n': //无校验
    case 'N': 
      U1CTL &=~ PENA; 
      break;
    case 'p': //偶校验
    case'P': 
      U1CTL |= PENA + PEV; 
      break;
    case 'o':
    case 'O': 
      U1CTL |= PENA; 
      U1CTL &=~ PEV; 
      break; //奇校验
  default: 
    return FALSE; //参数错误
  }
  
  //------------------------设置数据位-------------------------
  
  switch(dataBits)
  {
    case 7:
      U1CTL &=~ CHAR; 
      break; //7位数据
    case 8:
      U1CTL |= CHAR; 
      break; //8位数据
    default: 
      return FALSE; //参数错误
  }
  //------------------------设置停止位-------------------------
  switch(stopBits)
  {
    case 1:
      U1CTL &= ~SPB; break; //1位停止位
    case 2:
      U1CTL |= SPB; break; //2位停止位
    default: 
      return FALSE; //参数错误
  } 
  
  P3SEL |= 0XC0; // P3.6,7 = USART1 TXD/RXD //端口使能
  P3DIR |=BIT6;// 输出
  P3DIR &=BIT7;//输入
  U1ME |= UTXE1 + URXE1; //发送 接收使能
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
* 函数名：itoa
* 描述  ：将整形数据转换成字符串
* 输入  ：-radix =10 表示10进制，其他结果为0
*         -value 要转换的整形数
*         -buf 转换后的字符串
*         -radix = 10
* 输出  ：无
* 返回  ：无
* 调用  ：被USART_printf()调用
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
  
  while ( *Data != 0)     // 判断是否到达字符串结束符
  {                                                          
    if ( *Data == '\\' )  //'\'
    {                                                                          
      switch ( *++Data )
      {
      case 'r':                                                                  //回车符
        Uart1_SendByte(0x0d);
        Data ++;
        break;
        
      case 'n':                                                                  //换行符
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
      case 's':                                                                                  //字符串
        s = va_arg(ap, const char *);
        for ( ; *s; s++) 
        {
          Uart1_SendByte(*s);
        }
        Data++;
        break;
        
      case 'd':                                                                                //十进制
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
