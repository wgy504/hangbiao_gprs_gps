#include "app.h"
INT8U DS1337_byte_read(INT8U ee_addr);

/*------------------------------------------------*/
//时间数组: 秒，分，时，星期，日，月，年//时间结构体

                 
//               [0]-秒十六进制 [6:4]-十位, [3:0]-个位
//               [1]-分十六进制 [6:4]-十位, [3:0]-个位
//               [2]-时十六进制 [6]-12/24小时, [5:4]-十位, [3:0]-个位
//               [3]-星期 [2:0]-星期，范围1~7
//               [4]-日期十六进制 [5:4]-十位, [3:0]-个位
//               [5]-月份十六进制 [4]-十位, [3:0]-个位
//               [6]-年份十六进制 [7:4]-十位, [3:0]-个位，从2000年开始，即，实际年份为2000+[6]的值
/**************************************************/
//函数功能: DS1337初始化，
//            如果原寄存器数据正常，则不重新写入
//  输入参数: 无
//  输出参数: 无
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
  
  i2c_send_byte(DS1337_ADDRESS&0xFE);		//设置器件地址+段地址 
  if (!i2c_wait_ack())
  {
    i2c_stop(); 
    return FALSE;
  }
  
  i2c_send_byte(ee_addr&0xFF);	//设置段内地址
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
  
  i2c_send_byte(DS1337_ADDRESS&0xFE);		//设置器件地址 
  if (!i2c_wait_ack())
  {
    i2c_stop(); 
    return FALSE;
  }
  

  i2c_send_byte(ee_addr);	//设置段内地址
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