#ifndef __TIMER_H__
#define __TIMER_H__
#include <bsp.h>
extern BOOL TimerA_PwmStartStop(BOOL startFlag);
extern BOOL TimerA_PwmInit(char Clk,char Div,char Mode);
extern void TimerA_PwmSetPeriod(INT16U Period);
extern void TimerA_PwmSetDuty(char Channel,INT16U Duty);

extern BOOL TimerB_1S_Flag;
extern BOOL TimerB_125mS_Flag;
extern void TimerB_Init();

#endif
