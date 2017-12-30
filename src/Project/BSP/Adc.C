#include "bsp.h"

unsigned int AdcRes[Num_of_Chs][Num_of_Results];  // 
unsigned char AdcResCount=0;


/*
MSP430F149的ADC12为SAR型12位AD,共有16路输入通道，
其中8路独立的外部输入通道，2路接外部Vref+,Vref-，
3路内部通道可分别测内部温度传感器、AVCC、和外部参考源.

P6口第二功能为AD输入端。MSP430F149的10、11分别接外部电压参考源正负极，
7脚可将内部电压参考源输出。

ADC12共有18个中断源，公用一个中断向量ADC12_VECTOR。

AD的参考源可选择内部电压参考源或外部电压参考源。
内部电压参考源有1.5V,2.5V可选,使用时向ADC12CTL0写入REFON+?REF2_5V就打开了2.5V。
外部电压参考源由REF+接入。上电时若不设置参考源，则参考源为系统供电电压3.3V。

*/

/*
///////for 1.5V VRef
//  oF = ((x/4096)*1500mV)-923mV)*1/1.97mV = x*761/4096 - 468
//  IntDegF = (ADC12MEM0 - 2519)* 761/4096
    IntDegF = (temp - 2519) * 761;
    IntDegF = IntDegF / 4096;

//  oC = ((x/4096)*1500mV)-986mV)*1/3.55mV = x*423/4096 - 278
//  IntDegC = (ADC12MEM0 - 2692)* 423/4096
    IntDegC = (temp - 2692) * 423;
    IntDegC = IntDegC / 4096;
---------------------------------------------
///////for 2.5V VRef
//  oC = ((x/4096)*2500mV)-986mV)*1/3.55mV = x*704/4096 - 278
//  IntDegC = (ADC12MEM0 - 1617)* 704/4096
    IntDegC = (temp - 2692) * 423;
    IntDegC = IntDegC / 4096;


*/

/*ENC=0时候，很多参数才可以修改。*/

void ADC12_Init(void)
{ 
  P6SEL = 0xFF;                             // Enable A/D channel inputs
  ADC12CTL0 = ADC12ON+MSC+SHT0_8+REFON+REF2_5V;           // Turn on ADC12, extend sampling time
                                            // to avoid overflow of results
  ADC12CTL1 = SHP+CONSEQ_3;                 // Use sampling timer, repeated sequence
  //ADC12MCTL0 = SREF_1 + INCH_0;                      // ref+=AVcc, channel = A0
  ADC12MCTL1 = SREF_1 + INCH_1;                      // ref+=AVcc, channel = A1
  ADC12MCTL2 = SREF_1 + INCH_2;                      // ref+=AVcc, channel = A2
  ADC12MCTL3 = SREF_1 + INCH_3;                      // ref+=AVcc, channel = A3
  ADC12MCTL4 = SREF_1 + INCH_4;                      // ref+=AVcc, channel = A4
  ADC12MCTL5 = SREF_1 + INCH_5;                      // ref+=AVcc, channel = A5
  ADC12MCTL6 = SREF_1 + INCH_6;                      // ref+=AVcc, channel = A6
  ADC12MCTL7 = SREF_1 + INCH_7 + EOS;                // ref+=AVcc, channel = A7

  delay_ms(20);          //为了让内部电压稳定
  ADC12IE = 0x08;                           // Enable ADC12IFG.3
  ADC12CTL0 |= ENC;                         // Enable conversions
  ADC12CTL0 |= ADC12SC;                     // Start conversion  
  
}

/*ADC中断服务函数，各个通道转换值的存储*/
#pragma vector=ADC12_VECTOR
__interrupt void ADC12ISR(void)
{
  static unsigned int index = 0;
    AdcRes[0][index]=ADC12MEM3;//Batt1
    AdcRes[1][index]=ADC12MEM4;//Batt2
    AdcRes[2][index]=ADC12MEM5;//Batt3
    AdcRes[3][index]=ADC12MEM6;//Batt4
    AdcRes[4][index]=ADC12MEM7;//Batt5
    AdcRes[5][index]=ADC12MEM2;//Charge
    AdcRes[6][index]=ADC12MEM1;//Discharge
//    AdcRes[7][index]=ADC12MEM0;//LightSense
    index = (index+1)%Num_of_Results;         // Increment results index, modulo; Set Breakpoint here
    AdcResCount=index;
}
