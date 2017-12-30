#ifndef __ADC_H__
#define __ADC_H__
#include <bsp.h>

#define   Num_of_Results   8
#define   Num_of_Chs       7
extern void ADC12_Init(void);
extern unsigned int AdcRes[Num_of_Chs][Num_of_Results]; 
extern unsigned char AdcResCount;

#endif
