#include "app.h"



#pragma vector=UART0TX_VECTOR
__interrupt void Uart0_Tx_Isr ()
{

}



#pragma vector=UART0RX_VECTOR
__interrupt void Uart0_Rx_Isr()
{

  /*�����������û��жϷ��������룬�罫����ѹ����ջ����*/
  INT8U ch=0;
  ch=RXBUF0;  

  
  
  
  
  
 
  
  if(ch=='#')
  {
    DBG_TimeOut_Count=0;                                   //���֡���м�ʱ��
    memset(DBG_RxBuf,0x00,MAX_DBG_RxBUF_SIZE);
    DBG_RxCount=0;
  }
  else
  {
    if(DBG_RxCount<MAX_DBG_RxBUF_SIZE-1)//�������������ȣ�������
    {
      DBG_RxBuf[DBG_RxCount++]=ch;
    }    
  }
  
  
  
  __low_power_mode_off_on_exit();//���жϺ�������ʱ�˳��͹���ģʽ(�κε͹���ģʽ
}

#pragma vector=UART1RX_VECTOR
__interrupt void Uart1_Rx_Isr()
{

  /*�����������û��жϷ��������룬�罫����ѹ����ջ����*/

  __low_power_mode_off_on_exit();//���жϺ�������ʱ�˳��͹���ģʽ(�κε͹���ģʽ
}

#pragma vector=TIMERB0_VECTOR
__interrupt void TimerB_Isr()
{
  static INT8U nCount_1S=0;
  static INT8U nCount_125mS=0;
  
  static INT32U tmpChgSoc=0,tmpDisChgSoc=0;
  static INT16U tmpSocCount=0;


  
  if(DBG_TimeOut_Count<2)//1*4mS,GPRS����ͨ�ų�ʱ��ʱ��
  {
    DBG_TimeOut_Count++;
  }
  else 
  {
    if((DBG_TimeOut_Count>=2)&&(DBG_TimeOut_Count<0xff))//ʱ�䵽
    {
      DBG_RxFlag=TRUE;       //�ñ�־λ
      DBG_TimeOut_Count=0xff;//ֹͣ��ʱ
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
      
    if(++tmpSocCount>=8*60)//1����
    {
      //125mS����һ�Σ�����8*60���ۼӵĽ������(8*60)�����Ϊ1���ڵ�ƽ����������mA*min

      //ȡһ���ӵ�ƽ��ֵ
      tmpChgSoc/=tmpSocCount;
      tmpDisChgSoc/=tmpSocCount;
      
      //�����mAH
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
  

  /*�����������û��жϷ��������룬�罫����ѹ����ջ����*/
  __low_power_mode_off_on_exit();//���жϺ�������ʱ�˳��͹���ģʽ(�κε͹���ģʽ
}