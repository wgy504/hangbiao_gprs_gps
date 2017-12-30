#ifndef __UART_H__
#define __UART_H__
#include <bsp.h>

extern BOOL Uart0_Init(long bauds,char parity,uchar dataBits,uchar stopBits);
extern BOOL Uart1_Init(long bauds,char parity,uchar dataBits,uchar stopBits);
extern void Uart0_SendByte(uchar dat);
extern void Uart0_SendStr(uchar *s);
extern void Uart1_SendByte(uchar dat);
extern void Uart1_SendStr(uchar *s);

extern void Uart1_printf(INT8U *Data,...);

#endif
