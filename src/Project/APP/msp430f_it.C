#include "app.h"



#pragma vector=UART0TX_VECTOR
__interrupt void Uart0_Tx_Isr ()
{

}



#pragma vector=UART0RX_VECTOR
__interrupt void Uart0_Rx_Isr()
{

  /*在这里添加用户中断服务程序代码，如将数据压入接收缓冲等*/
  INT8U ch=0;
  ch=RXBUF0;  

  
  
  
  
  
 
  
  if(ch=='#')
  {
    DBG_TimeOut_Count=0;                                   //清除帧空闲计时器
    memset(DBG_RxBuf,0x00,MAX_DBG_RxBUF_SIZE);
    DBG_RxCount=0;
  }
  else
  {
    if(DBG_RxCount<MAX_DBG_RxBUF_SIZE-1)//超出缓冲区长度，不接收
    {
      DBG_RxBuf[DBG_RxCount++]=ch;
    }    
  }
  
  
  
  __low_power_mode_off_on_exit();//从中断函数返回时退出低功耗模式(任何低功耗模式
}

#pragma vector=UART1RX_VECTOR
__interrupt void Uart1_Rx_Isr()
{

  /*在这里添加用户中断服务程序代码，如将数据压入接收缓冲等*/

  __low_power_mode_off_on_exit();//从中断函数返回时退出低功耗模式(任何低功耗模式
}

#pragma vector=TIMERB0_VECTOR
__interrupt void TimerB_Isr()
{
  static INT8U nCount_1S=0;
  static INT8U nCount_125mS=0;
  
  static INT32U tmpChgSoc=0,tmpDisChgSoc=0;
  static INT16U tmpSocCount=0;


  
  if(DBG_TimeOut_Count<2)//1*4mS,GPRS串口通信超时计时器
  {
    DBG_TimeOut_Count++;
  }
  else 
  {
    if((DBG_TimeOut_Count>=2)&&(DBG_TimeOut_Count<0xff))//时间到
    {
      DBG_RxFlag=TRUE;       //置标志位
      DBG_TimeOut_Count=0xff;//停止计时
    }
  }
  
  if(++nCount_1S>=255)//1s Tick
  {
    TimerB_1S_Flag=TRUE;   
    SysTimeTick++;
  }
  
  if(++nCount_125mS>=32)//125mS Tick
  {
    nCount_125mS=0;
    TimerB_125mS_Flag=TRUE;
      
    if(++tmpSocCount>=8*60)//1分钟
    {
      //125mS采样一次，采样8*60次累加的结果除以(8*60)，结果为1分内的平均电流，即mA*min

      //取一分钟的平均值
      tmpChgSoc/=tmpSocCount;
      tmpDisChgSoc/=tmpSocCount;
      
      //换算成mAH
      DevData.ChargeSoc+=(tmpChgSoc/60);//;//mAH
      DevData.DisChargeSoc+=(tmpDisChgSoc/60);//mAH
      tmpChgSoc=0;
      tmpDisChgSoc=0;
      tmpSocCount=0;      
    }
    else
    {
      tmpChgSoc+=DevData.ChargeCurr;
      tmpDisChgSoc+=DevData.DisChargeCurr;
    }
  }
  

  /*在这里添加用户中断服务程序代码，如将数据压入接收缓冲等*/
  __low_power_mode_off_on_exit();//从中断函数返回时退出低功耗模式(任何低功耗模式
}