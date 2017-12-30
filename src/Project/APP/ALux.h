#ifndef __ALUX_H__
#define __ALUX_H__
#include <bsp.h>
#define MAX44009_ADDR 0x94	//1001 01 A0 R/W
#define delay_MS(x) delay_ms(x)

//begin definition of slave addresses for MAX44009 
#define INT_STATUS      0x00
#define INT_ENABLE      0x01
#define CONFIG_REG      0x02
#define HIGH_BYTE       0x03
#define LOW_BYTE        0x04
#define THRESH_HIGH     0x05
#define THRESH_LOW      0x06
#define THRESH_TIMER    0x07

extern void Task_LuxCtrl();

#endif
