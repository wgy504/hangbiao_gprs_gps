#include "app.h"


DEVICE_INFO     DevInfo;            //�豸��Ϣ
DEVICE_DATA	DevData;            //�豸ʵʱ����
DEV_BASIC_INFO	DevBasicInfo;

INT32U SysTimeTick;//1Second Tick

//�����ַ���һ�γ��ֵ�λ��
INT8U findCh(INT8U *pSrc,INT8U nLen,INT8U ch)
{
  for(INT8U i=0;i<nLen;i++)
  {
//    printf("ch=%c,i=%d.\r\n",pSrc[i],i);
    if(pSrc[i]==ch)
    {
      return i;
    }
  }
  return 0;
}


INT32U OSTimeGet(void)
{
  return SysTimeTick;	 //��ȡϵͳʱ��
}

/*printf������ʵ��*/
int putchar(int ch)
{
  Uart0_SendByte(ch);
  return ch;
}

/*��һ��0~15������ת��Ϊ �ַ�asc*/
char int2char(INT8U ch)
{
  char tmp[]="0123456789ABCDEF";
  if(ch>15)
  {
    return 0;
  }  
  return tmp[ch];
}


INT8U bcd2bin(INT8U ch)
{
  return 10*((ch>>4)&0x0F)+(ch&0x0F);
}

INT8U bin2bcd(INT8U ch)
{
  return ((ch/10)<<4)+(ch%10);
}



/*

ϵͳ����:
@1.���Ź���ʱ���
@2.ϵͳ��ʱ����

@����5�㵽�ڶ���5����Ϊһ��
@�����������£�
1.��⵽ʱ�䵽���������
2.д��WDT������־��1
3.main ������ʱ�򣬶�ȡWDT��־��
  �����1������Ϊ��WDT��������eep����DevData���������WDT��־
  ͬʱ�����޸ĵ��ʺ͹�ǿ

  �����0������Ϊ������������������
*/

void Task_SysCtrl()
{
  static BOOL bWdtFlag=FALSE;

  bWdtFlag=!bWdtFlag;
  WDT_CTRL_EN(bWdtFlag);
}
  
  

