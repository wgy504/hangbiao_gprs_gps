#ifndef __CONFIG_H__
#define __CONFIG_H__
#include "bsp.h"






//8��LED�ƣ�������P6�ڣ���ͨ���Ͽ���Դֹͣʹ�ã�ADCʹ��ʱ�Ͽ���Դ
#define LED8DIR         P6DIR
#define LED8            P6OUT                             //P6�ڽ�LED�ƣ�8��

//4����������������P10~P13
#define KeyPort         P1IN                              //�������̽���P10~P13

//���ڲ����ʼ��㣬��BRCLK=CPU_Fʱ������Ĺ�ʽ���Լ��㣬����Ҫ�������ü����Ƶϵ��
#define baud           9600                                //���ò����ʵĴ�С
#define baud_setting   (uint)((ulong)CPU_F/((ulong)baud))  //�����ʼ��㹫ʽ
#define baud_h         (uchar)(baud_setting>>8)            //��ȡ��λ
#define baud_l         (uchar)(baud_setting)               //��λ





extern void Clock_Init();
extern void Clock_Init_Inc();
extern void Clock_Init_Ex32768();
extern void WDT_Init();

#endif

