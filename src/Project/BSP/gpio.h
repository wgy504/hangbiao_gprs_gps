#ifndef __GPIO_H__
#define __GPIO_H__
#include <bsp.h>


#define WDT_CTRL_EN(a)	if (a)	\
		P5OUT &= ~BIT5;\
		else		\
		P5OUT |= BIT5

extern void Gpio_Init();
extern void ChgCtrl(INT8U chgIndex);
extern void DischgCtrl(INT8U dischgIndex);

#endif
