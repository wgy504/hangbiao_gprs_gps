#include "bsp.h"

void i2c_init()
{
  SCL_OUT(); //SCL�ܽ�Ϊ���
  SDA_OUT(); //SDA�ܽ�Ϊ���
  i2c_stop();
}

BOOL i2c_start(void) //sclΪ�ߵ�ƽ�ڼ�,sda����һ���½���
{
  SDA_OUT(); //SDA�ܽ�Ϊ��� 
  
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

void i2c_stop(void) //scl�ߵ�ƽ�ڼ�,sda����һ������
{
  SDA_OUT(); //SDA�ܽ�Ϊ���  
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
//void i2c_ack(void) //IIC����Ӧ��////////SCLΪ�ߵ�ƽʱ,SDAΪ�͵�ƽ
//{
//  SDA_OUT(); //SDA�ܽ�Ϊ��� 
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

void i2c_no_ack(void) //IIC������Ӧ��///SDAά�ָߵ�ƽ,SCL����һ������
{
  SDA_OUT(); //SDA�ܽ�Ϊ���  
  
  SDA_HIGH();
  delay_us(5);
  SCL_HIGH();
  delay_us(5);
  SCL_LOW();
  delay_us(5);
}

BOOL i2c_wait_ack(void )
{
  SDA_OUT(); //SDA�ܽ�Ϊ��� 
  
  SDA_LOW();
  delay_us(5);  
  
  SDA_IN(); //SDA�ܽ�Ϊ���� 
  
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
  SDA_OUT(); //SDA�ܽ�Ϊ���   
  
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
  
  SDA_IN(); //SDA�ܽ�Ϊ����  
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