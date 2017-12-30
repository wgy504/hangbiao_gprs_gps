#include "app.h"
INT8U DS1337_byte_read(INT8U ee_addr);

/*------------------------------------------------*/
//ʱ������: �룬�֣�ʱ�����ڣ��գ��£���//ʱ��ṹ��

                 
//               [0]-��ʮ������ [6:4]-ʮλ, [3:0]-��λ
//               [1]-��ʮ������ [6:4]-ʮλ, [3:0]-��λ
//               [2]-ʱʮ������ [6]-12/24Сʱ, [5:4]-ʮλ, [3:0]-��λ
//               [3]-���� [2:0]-���ڣ���Χ1~7
//               [4]-����ʮ������ [5:4]-ʮλ, [3:0]-��λ
//               [5]-�·�ʮ������ [4]-ʮλ, [3:0]-��λ
//               [6]-���ʮ������ [7:4]-ʮλ, [3:0]-��λ����2000�꿪ʼ������ʵ�����Ϊ2000+[6]��ֵ
/**************************************************/
//��������: DS1337��ʼ����
//            ���ԭ�Ĵ�������������������д��
//  �������: ��
//  �������: ��
//Bit7  Bit6  Bit5  Bit4  Bit3   Bit2   Bit1   Bit0
//nEOSC  0      0    RS2   RS1   INTCN   A2IE   A1IE
//nEOSC(Enable Oscillator)
//---0:Start The OSC(Default)
//---1:Stop The OSC

//RS2:RS1(Rate Select)
//---00:1Hz
//---01:4096Hz
//---10:8192Hz
//---11:32768Hz(Default)

//INTCN(Interrupt Control)
//---0:SQW OutPut On The SQW Pin(Default)
//---1:

//A1IE(Alarm 1 Interrupt Enable)
//---0:Disable(Default)
//---1:Enable

//A2IE(Alarm 2 Interrupt Enable)
//---0:Disable(Default)
//---1:Enable
/*------------------------------------------------*/
 


BOOL DS1337_byte_write(INT16U ee_addr, INT8U ch)
{
  if (!i2c_start()) 
    return FALSE;
  
  i2c_send_byte(DS1337_ADDRESS&0xFE);		//����������ַ+�ε�ַ 
  if (!i2c_wait_ack())
  {
    i2c_stop(); 
    return FALSE;
  }
  
  i2c_send_byte(ee_addr&0xFF);	//���ö��ڵ�ַ
  i2c_wait_ack();
  
  i2c_send_byte(ch);
  i2c_wait_ack();
  
  i2c_stop();
  
  delay_MS(30);
  return TRUE;
}

INT8U DS1337_byte_read(INT8U ee_addr)
{
  INT8U ret= 0;
  if (!i2c_start()) 
    return FALSE;
  
  i2c_send_byte(DS1337_ADDRESS&0xFE);		//����������ַ 
  if (!i2c_wait_ack())
  {
    i2c_stop(); 
    return FALSE;
  }
  

  i2c_send_byte(ee_addr);	//���ö��ڵ�ַ
  i2c_wait_ack();
  
  i2c_start();
  i2c_send_byte((DS1337_ADDRESS&0xFE)+1);
  i2c_wait_ack();
  ret=i2c_read_byte();

  i2c_no_ack();  
  i2c_stop();
  //delay_MS(15);
  return ret;	
}


void DS1337_Set_Time(TIME_INFO Times)
{
  DS1337_byte_write(REG_SEC,bin2bcd(Times.second));
  DS1337_byte_write(REG_MIN,bin2bcd(Times.minute));
  DS1337_byte_write(REG_HOUR,bin2bcd(Times.hour));
  DS1337_byte_write(REG_DAY,bin2bcd(Times.day));
  DS1337_byte_write(REG_MON,bin2bcd(Times.month));
  DS1337_byte_write(REG_YEAR,bin2bcd(Times.year));  
}

void Get_RtcTime()
{
  DevData.TimeInfo.second=bcd2bin(DS1337_byte_read(REG_SEC));
  DevData.TimeInfo.minute=bcd2bin(DS1337_byte_read(REG_MIN));
  DevData.TimeInfo.hour=bcd2bin(DS1337_byte_read(REG_HOUR));
  DevData.TimeInfo.day=bcd2bin(DS1337_byte_read(REG_DAY));
  DevData.TimeInfo.month=bcd2bin(DS1337_byte_read(REG_MON));
  DevData.TimeInfo.year=bcd2bin(DS1337_byte_read(REG_YEAR));
  if((DevInfo.RtcDebugLevel&DBG_MSG))//
  {
    printf("MSG-RTC:20%02d-%02d-%02d %02d:%02d:%02d.\r\n",
           DevData.TimeInfo.year,DevData.TimeInfo.month,DevData.TimeInfo.day,DevData.TimeInfo.hour,DevData.TimeInfo.minute,DevData.TimeInfo.second);
  }
}