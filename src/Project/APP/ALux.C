#include "app.h"
#define MAX_AVG_COUNT 10

INT8U LuxErrCode=0;

extern INT8U Lux_byte_read(INT8U dev_addr, INT8U ee_addr);
extern float Lux_single_val_read(INT8U dev_addr);
extern float Lux_dual_val_read(INT8U dev_addr);
extern BOOL Lux_byte_write(INT8U dev_addr, INT16U ee_addr, INT8U ch);


void Task_LuxCtrl()
{
  //BOOL retFlag; 
  static BOOL lastFlag=TRUE;
  static INT16U tmpValTH_H=0,tmpValTH_L=0;
  static INT16U SenseValArray[MAX_AVG_COUNT]={0};
  static INT8U SenseValArrayCount=0;
  
  float tmpLuxVal=0;
  
  INT32U tmpSum=0;
  INT16U currLuxVal=0;
  INT8U i=0;
  //val=Lux_single_val_read(MAX44009_ADDR);

  tmpLuxVal=Lux_dual_val_read(MAX44009_ADDR);
  if(LuxErrCode!=0)
  {
    if((DevInfo.LuxDebugLevel&DBG_ERR))//
    {
      printf("ERR-Lux:Sensor Not Mount,ErrCode=%d.\r\n",LuxErrCode);
    }
    return;
  }
  tmpLuxVal=(tmpLuxVal>60000)? 60000:tmpLuxVal;//最高限制在60000量程
  currLuxVal=(INT16U)tmpLuxVal;//读取Lux值
  
  
  //移动平均
  if(++SenseValArrayCount>=MAX_AVG_COUNT)
  {
    SenseValArrayCount=MAX_AVG_COUNT;
  }
  
  for(i=0;i<MAX_AVG_COUNT-1;i++)
  {
    tmpSum+=SenseValArray[i+1];
    SenseValArray[i]=SenseValArray[i+1];
  }
  
  SenseValArray[i]=currLuxVal;//采样电压,保存最新的数据,加入到队列最后
  tmpSum+=SenseValArray[i];	
  
  DevData.CurrLuxVal=(SenseValArrayCount==0)? 0:tmpSum/SenseValArrayCount;	
  
  if(DevData.CurrLuxVal<=DevInfo.DstLuxValL)//现在是晚上
  {
    tmpValTH_H=DevInfo.DstLuxVal;
    tmpValTH_L=DevInfo.DstLuxValL;
    
    DevData.IsLightOnFlag=TRUE;             //更新灯状态
    lastFlag=DevData.IsLightOnFlag;         //保存当前的状态
    DevData.IsDayFlag=FALSE;                //当前为晚上时间段
  }
  else if(DevData.CurrLuxVal>=DevInfo.DstLuxValH)//现在是白天
  {   
    tmpValTH_H=DevInfo.DstLuxValH;
    tmpValTH_L=DevInfo.DstLuxVal;  
    DevData.IsLightOnFlag=FALSE;              //更新灯状态
    lastFlag=DevData.IsLightOnFlag;           //保存当前的状态
    DevData.IsDayFlag=TRUE;                   //当前为白天时间段
  }
  else  //白天到晚上（或者晚上到白天）的过渡时间段
  {
    if(DevData.CurrLuxVal<=tmpValTH_L)        //低于阈值，直接开灯
    {
      DevData.IsLightOnFlag=TRUE;
      lastFlag=DevData.IsLightOnFlag;
    }
    else if(DevData.CurrLuxVal>=tmpValTH_H)   //高于阈值，直接关灯
    {
      DevData.IsLightOnFlag=FALSE;
      lastFlag=DevData.IsLightOnFlag;
    }
    else
    {
      DevData.IsLightOnFlag=lastFlag;         //保持上一状态不变
    }
  }
  
  if((DevInfo.LuxDebugLevel&DBG_MSG))//
  {
    printf("MSG-Lux:CurrLux=%d,SrcLux=%d,TH_H=%d,TH_L=%d,lastFlag=%d,IsLightOnFlag=%d,IsDayFlag=%d.\r\n",
           DevData.CurrLuxVal,currLuxVal,tmpValTH_H,tmpValTH_L,lastFlag,DevData.IsLightOnFlag,DevData.IsDayFlag);
  }
}

INT8U Lux_byte_read(INT8U dev_addr, INT8U ee_addr)
{
  INT8U ch = 0;
  LuxErrCode=0;
  if (!i2c_start())
  {	
    LuxErrCode=1;
    return FALSE;
  }
  
  i2c_send_byte(dev_addr&0xFE);		//设置器件地址+段地址 
  if(!i2c_wait_ack())
  {
    i2c_stop(); 
    LuxErrCode=2;
    return FALSE;
  }

  i2c_send_byte(ee_addr);  //设置地址
  i2c_wait_ack();
  
  i2c_start();
  i2c_send_byte((dev_addr&0xFE)+1);
  i2c_wait_ack();
  
  ch = i2c_read_byte();
  i2c_no_ack();
  
  i2c_stop();
  delay_MS(15);
  return ch;	
}

/*
=======================Register Summary=======================
REGISTER                               R/W   RstVal
Status
R0: -   -   -   -   -   -   -   INTS | R    | 0x00  | Interrupt Status
R1: -   -   -   -   -   -   -   INTE | R/W  | 0x00  | Interrupt Enable

Config
R2: COUNT  MANUAL - - CDR TIM[2:0]   | R/W  | 0x03  | Configuration

LUX Reading
R3: E3  E2  E1  E0  M7  M6  M5  M4   | R/W  | 0x00  | Lux High Byte
R4: -   -   -   -   M3  M2  M1  M0   | R/W  | 0x00  | Lux Low Byte

Threshold Set
R5:      UE[3:0]        UM[7:4]      | R/W  | 0xFF  | Upper Threshold High Byte
R6:      LE[3:0]        LM[7:4]      | R/W  | 0x00  | Lower Threshold High Byte
R7: T7  T6  T5  T4  T3  T2  T1  T0   | R/W  | 0xFF  | Threshold Timer
-------------------------------------------------------------
Once INTS bit is set,it can be clrared either by reading the R0 or by writing a 0 to R1.
在中断模式下，通过读取R0清中断，
在查询模式下，不要读R0，直接在低电平期间，读取R3和R4。

=======================Lux Calc=======================
E=8*E3 + 4*E2 + 2*E1 + E0;
M_1=128*M7 + 64*M6 + 32*M5 + 16*M4;
M_2=128*M7 + 64*M6 + 32*M5 + 16*M4 + 8*M3 + 4*M2 + 2*M1 + M0;
=============================================================
ALux=2exp(E) x M_1 x0.72   或
ALux=2exp(E) x M_22 x0.045

*/

float Lux_single_val_read(INT8U dev_addr)
{
  float retVal = 0;
  INT8U reg_addr=3;
  INT8U E=0,M=0;
  LuxErrCode=0;
  if (!i2c_start())
  {		
    LuxErrCode=1;
    return FALSE;
  }
  i2c_send_byte(dev_addr&0xFE);		//设置器件地址+段地址 
  if(!i2c_wait_ack())
  {
    i2c_stop(); 
    LuxErrCode=2;
    return FALSE;
  }
  i2c_send_byte(reg_addr);  //设置地址
  i2c_wait_ack();
  i2c_start();
  i2c_send_byte((dev_addr&0xFE)+1);
  i2c_wait_ack();
  E = i2c_read_byte();
  M=((E)&0x0F);
  E =((E>>4)&0x0F);
  i2c_no_ack();   
  i2c_stop();
  
  retVal=M*(1<<E)*0.72;
  delay_MS(15);
  return retVal;	
}

float Lux_dual_val_read(INT8U dev_addr)
{
  float retVal = 0;
  INT8U reg_addr=3;//寄存器地址
  INT8U E=0,M=0,tmp1=0,tmp2=0;
  LuxErrCode=0;
  if (!i2c_start())
  {		
    LuxErrCode=0x01;
    return FALSE;
  }
  i2c_send_byte(dev_addr&0xFE);		//设置器件地址+段地址 
  if(!i2c_wait_ack())
  {
    i2c_stop(); 
    LuxErrCode=0x02;
    return FALSE;
  }
  i2c_send_byte(reg_addr);  //设置地址
  i2c_wait_ack();
  i2c_start();
  i2c_send_byte((dev_addr&0xFE)+1);
  i2c_wait_ack();
  tmp1 = i2c_read_byte();
  i2c_no_ack();
  
  
  
  i2c_start();  
  //////////////////////////////////////
  i2c_send_byte(dev_addr&0xFE);		//设置器件地址+段地址 
  i2c_wait_ack();
  i2c_send_byte(reg_addr+1);  //设置地址
  i2c_wait_ack();
  i2c_start();
  i2c_send_byte((dev_addr&0xFE)+1);
  i2c_wait_ack();
  tmp2= i2c_read_byte();
  
//  E =((tmp1>>4)&0x0F);
//  M=((tmp1<<4)&0xF0);
//  M+=(tmp2&0x0F);
  
  E =((tmp1&0xF0)>>4);
  M=((tmp1&0x0F)<<4);
  M+=(tmp2&0x0F);
  
  i2c_no_ack(); 
  i2c_stop();
  
  //retVal=M*(1<<E)*0.045;
  retVal=pow(2,E)*M*0.045;
  
  delay_MS(15);
  return retVal;	
}


BOOL Lux_byte_write(INT8U dev_addr, INT16U ee_addr, INT8U ch)
{
  if (!i2c_start()) 
  {
    return FALSE;
  }
    
  i2c_send_byte(dev_addr&0xFE);		//设置器件地址+段地址 
  if (!i2c_wait_ack())
  {
    i2c_stop(); 
    return FALSE;
  }
  
  i2c_send_byte(ee_addr>>8);	//设置段内地址
  i2c_wait_ack();
  
  i2c_send_byte(ee_addr&0xFF);	//设置段内地址
  i2c_wait_ack();
  
  i2c_send_byte(ch);
  i2c_wait_ack();
  
  i2c_stop();
  
  delay_MS(30);
  return TRUE;
}
