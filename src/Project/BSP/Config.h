#ifndef __CONFIG_H__
#define __CONFIG_H__
#include "bsp.h"






//8个LED灯，连接在P6口，可通过断开电源停止使用，ADC使用时断开电源
#define LED8DIR         P6DIR
#define LED8            P6OUT                             //P6口接LED灯，8个

//4个独立按键连接在P10~P13
#define KeyPort         P1IN                              //独立键盘接在P10~P13

//串口波特率计算，当BRCLK=CPU_F时用下面的公式可以计算，否则要根据设置加入分频系数
#define baud           9600                                //设置波特率的大小
#define baud_setting   (uint)((ulong)CPU_F/((ulong)baud))  //波特率计算公式
#define baud_h         (uchar)(baud_setting>>8)            //提取高位
#define baud_l         (uchar)(baud_setting)               //低位





extern void Clock_Init();
extern void Clock_Init_Inc();
extern void Clock_Init_Ex32768();
extern void WDT_Init();

#endif

