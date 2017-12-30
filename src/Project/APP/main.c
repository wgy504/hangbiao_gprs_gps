/********************************************************************
//DM430-L型最小系统板4位独立按键测试程序，采用查询模式
//按不同的按键，显示不同的LED灯，具体请看程序
//调试环境：EW430 V5.30
//作者：www.avrgcc.com
//时间：2014.03.01
********************************************************************/

#include "app.h"

//*************************************************************************
//		主程序
//除通信地址外，通信协议均为低位在前，eeprom存储也是低位在前
//例外：通信地址，IMEI号码，IP地址，APN，OEMCode
//*************************************************************************

void main(void)
{ 

  WDT_Init();                               //看门狗设置
  Clock_Init();                             //系统时钟设置

  Uart0_Init(9600,'N',8,1);
  Uart1_Init(9600,'N',8,1); 
  Gpio_Init();//先初始化Gpio，再初始化Uart1, 会导致IO不正常，原因未明。
  TimerB_Init();
  ADC12_Init();
  _EINT();

  
  DevInfo.MaxChgVolt=4200;	                                                /*最大充电电压，mV*/
  DevInfo.MinDischgVolt=3300;                                                   /*最小放电电压，mV*/
  DevInfo.ChargePollIntval=10;                                                  /*充电轮询周期，S*/
  DevInfo.DisChargePollIntval=10;	                                        /*放电轮询周期，S*/
  //DevInfo.SysDebugLevel=(DBG_ERR|DBG_INF|DBG_MSG);                              /*默认系统日志全打开*/
  DevInfo.ChgDebugLevel=(DBG_ERR|DBG_INF|DBG_MSG);                              /*默认系统日志全打开*/
  DevInfo.SubDebugLevel=(DBG_ERR|DBG_INF|DBG_MSG);                              /*默认系统日志全打开*/
  
  DevData.BattEnFlag=0x1F;//默认使能5个电池  
  DevData.CurrBackBattNo=0;
  
  //OEMCode:SNAC
  memcpy(DevBasicInfo.OEMCode,"SNAC",4);
  
  //SWVersion:V2.00
  DevBasicInfo.SWVersion[0]=0x02;//BCD
  DevBasicInfo.SWVersion[1]=0x00;  

  //协议类型:1
  DevBasicInfo.PTType=0x01;//Hex
  
  //协议版本:V1.00
  DevBasicInfo.PTVersion[0]=0x01;//BCD
  DevBasicInfo.PTVersion[1]=0x00;   
  
  //硬件版本:V3.10
  DevBasicInfo.HWVersion[0]=0x03;//BCD
  DevBasicInfo.HWVersion[1]=0x10; 
  
  printf("\r\nSystem Start.SW=V1.00,HW=V3.10,Build Time=2016-5-7.\r\n\r\n\r\n");

  while(1)
  {
    Task_Dbg();
    
    if(TimerB_125mS_Flag)//喂狗操作
    {
      TimerB_125mS_Flag=FALSE;
      Task_SysCtrl();
    }

 
    
    if(TimerB_1S_Flag==TRUE)
    {
      TimerB_1S_Flag=FALSE;

      Get_BattInfo();   
      Task_PwrMng();//充放电控制  
      if((DevInfo.SysDebugLevel&DBG_INF))
      {
        printf("\r\nINF-CHG:TimerB 1S Int.\r\n");
      } 
      
    }   
  }
}

