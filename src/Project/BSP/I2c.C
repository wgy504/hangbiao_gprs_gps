#include "bsp.h"

void i2c_init()
{
  SCL_OUT(); //SCL管脚为输出
  SDA_OUT(); //SDA管脚为输出
  i2c_stop();
}

BOOL i2c_start(void) //scl为高电平期间,sda产生一个下降沿
{
  SDA_OUT(); //SDA管脚为输出 
  
  SDA_HIGH();
  delay_us(5);
  SCL_HIGH();
  delay_us(5);
  SDA_LOW();
  delay_us(5);
  SCL_LOW();
  delay_us(5);  
  return TRUE;
}

void i2c_stop(void) //scl高电平期间,sda产生一上升沿
{
  SDA_OUT(); //SDA管脚为输出  
//  
//  SCL_LOW();
//  delay_us(5);
  SDA_LOW();
  delay_us(5);
  SCL_HIGH();
  delay_us(5);
  SDA_HIGH();
  delay_us(5);
}
//
//void i2c_ack(void) //IIC总线应答////////SCL为高电平时,SDA为低电平
//{
//  SDA_OUT(); //SDA管脚为输出 
//  
//  SDA_LOW();
//  delay_us(5);
//  SCL_HIGH();
//  delay_us(5);
//  SCL_LOW();
//  delay_us(5);
//  SDA_HIGH();
//  delay_us(5);
//}

void i2c_no_ack(void) //IIC总线无应答///SDA维持高电平,SCL产生一个脉冲
{
  SDA_OUT(); //SDA管脚为输出  
  
  SDA_HIGH();
  delay_us(5);
  SCL_HIGH();
  delay_us(5);
  SCL_LOW();
  delay_us(5);
}

BOOL i2c_wait_ack(void )
{
  SDA_OUT(); //SDA管脚为输出 
  
  SDA_LOW();
  delay_us(5);  
  
  SDA_IN(); //SDA管脚为输入 
  
  SCL_HIGH();
  delay_us(5);  
  if(SDA_READ())
  {
    SCL_LOW();
    return FALSE;
  }
  SCL_LOW();
  return TRUE;
}

void i2c_send_byte(uchar dat)
{
  SDA_OUT(); //SDA管脚为输出   
  
  INT8U i = 8;
  while (i--)
  {
    if (dat&0x80)
    {
      SDA_HIGH();
    }
    else 
    {
      SDA_LOW();
    }

    SCL_HIGH();
    delay_us(5);
    SCL_LOW();
    delay_us(5);
    
    dat<<=1;
  }
  SCL_LOW();
}

uchar i2c_read_byte(void)
{
  INT8U i=8;
  INT8U dat=0;
  
  SDA_IN(); //SDA管脚为输入  
  while (i--)
  {   
    SCL_HIGH();
    delay_us(5);
    dat <<= 1;

    if (SDA_READ())
    {
      dat |= 0x01;
    }
    SCL_LOW();
    delay_us(5);
  } 
  return dat;  
}