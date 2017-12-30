#include "app.h"


DEVICE_INFO     DevInfo;            //设备信息
DEVICE_DATA	DevData;            //设备实时数据
DEV_BASIC_INFO	DevBasicInfo;

INT32U SysTimeTick;//1Second Tick

//查找字符第一次出现的位置
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
  return SysTimeTick;	 //获取系统时钟
}

/*printf函数的实现*/
int putchar(int ch)
{
  Uart0_SendByte(ch);
  return ch;
}

/*把一个0~15的数字转换为 字符asc*/
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

系统控制:
@1.看门狗定时输出
@2.系统定时重启

@下午5点到第二天5点作为一天
@重启流程如下：
1.检测到时间到，保存参数
2.写入WDT重启标志：1
3.main 启动的时候，读取WDT标志，
  如果是1，则认为是WDT启动，从eep加载DevData，并且清除WDT标志
  同时不让修改灯质和光强

  如果是0，则认为是冷启动，正常启动
*/

void Task_SysCtrl()
{
  static BOOL bWdtFlag=FALSE;

  bWdtFlag=!bWdtFlag;
  WDT_CTRL_EN(bWdtFlag);
}
  
  


