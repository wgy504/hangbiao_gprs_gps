/********************************************************************
//DM430-L����Сϵͳ��4λ�����������Գ��򣬲��ò�ѯģʽ
//����ͬ�İ�������ʾ��ͬ��LED�ƣ������뿴����
//���Ի�����EW430 V5.30
//���ߣ�www.avrgcc.com
//ʱ�䣺2014.03.01
********************************************************************/

#include "app.h"

//*************************************************************************
//		������
//��ͨ�ŵ�ַ�⣬ͨ��Э���Ϊ��λ��ǰ��eeprom�洢Ҳ�ǵ�λ��ǰ
//���⣺ͨ�ŵ�ַ��IMEI���룬IP��ַ��APN��OEMCode
//*************************************************************************

void main(void)
{ 

  WDT_Init();                               //���Ź�����
  Clock_Init();                             //ϵͳʱ������

  Uart0_Init(9600,'N',8,1);
  Uart1_Init(9600,'N',8,1); 
  Gpio_Init();//�ȳ�ʼ��Gpio���ٳ�ʼ��Uart1, �ᵼ��IO��������ԭ��δ����
  TimerB_Init();
  ADC12_Init();
  _EINT();

  
  DevInfo.MaxChgVolt=4200;	                                                /*������ѹ��mV*/
  DevInfo.MinDischgVolt=3300;                                                   /*��С�ŵ��ѹ��mV*/
  DevInfo.ChargePollIntval=10;                                                  /*�����ѯ���ڣ�S*/
  DevInfo.DisChargePollIntval=10;	                                        /*�ŵ���ѯ���ڣ�S*/
  //DevInfo.SysDebugLevel=(DBG_ERR|DBG_INF|DBG_MSG);                              /*Ĭ��ϵͳ��־ȫ��*/
  DevInfo.ChgDebugLevel=(DBG_ERR|DBG_INF|DBG_MSG);                              /*Ĭ��ϵͳ��־ȫ��*/
  DevInfo.SubDebugLevel=(DBG_ERR|DBG_INF|DBG_MSG);                              /*Ĭ��ϵͳ��־ȫ��*/
  
  DevData.BattEnFlag=0x1F;//Ĭ��ʹ��5�����  
  DevData.CurrBackBattNo=0;
  
  //OEMCode:SNAC
  memcpy(DevBasicInfo.OEMCode,"SNAC",4);
  
  //SWVersion:V2.00
  DevBasicInfo.SWVersion[0]=0x02;//BCD
  DevBasicInfo.SWVersion[1]=0x00;  

  //Э������:1
  DevBasicInfo.PTType=0x01;//Hex
  
  //Э��汾:V1.00
  DevBasicInfo.PTVersion[0]=0x01;//BCD
  DevBasicInfo.PTVersion[1]=0x00;   
  
  //Ӳ���汾:V3.10
  DevBasicInfo.HWVersion[0]=0x03;//BCD
  DevBasicInfo.HWVersion[1]=0x10; 
  
  printf("\r\nSystem Start.SW=V1.00,HW=V3.10,Build Time=2016-5-7.\r\n\r\n\r\n");

  while(1)
  {
    Task_Dbg();
    
    if(TimerB_125mS_Flag)//ι������
    {
      TimerB_125mS_Flag=FALSE;
      Task_SysCtrl();
    }

 
    
    if(TimerB_1S_Flag==TRUE)
    {
      TimerB_1S_Flag=FALSE;

      Get_BattInfo();   
      Task_PwrMng();//��ŵ����  
      if((DevInfo.SysDebugLevel&DBG_INF))
      {
        printf("\r\nINF-CHG:TimerB 1S Int.\r\n");
      } 
      
    }   
  }
}

