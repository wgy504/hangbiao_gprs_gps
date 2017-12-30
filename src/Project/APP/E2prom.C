#include "app.h"
INT8U E2p_byte_read(INT8U dev_addr, INT16U ee_addr);
BOOL E2p_byte_write(INT8U dev_addr, INT16U ee_addr, INT8U ch);
BOOL E2p_block_write(INT8U dev_addr, INT16U ee_addr, INT8U *dat, INT16U len);
BOOL E2p_block_read(INT8U dev_addr, INT16U ee_addr ,INT8U* pDst,INT8U len);

BOOL E2pErrCode=0;

void E2p_Test()
{
//  unsigned char dat=0;
//  INT32U nErrCount=0;
  
  
  E2p_Init();
  delay_MS(10);

////@stage 1:test all Byte.
  
//  printf("\r\nE2p Test:Write Start.\r\n");
//  for(INT32U i=0;i<EEP_MAX_SIZE;i++)
//  {
//    E2p_byte_write(EEP_ADDRESS, i, (INT8U)(1+(i/256)));//里面有30Ms的延时，故33循环为大概1S
//    if((i%33)==0)
//    {
//      printf("Time Total=%lu S,Eclipse %lu S.\r\n",(INT32U)(EEP_MAX_SIZE/33),(i/33)); 
//    }    
//  }
//  printf("\r\nE2p Test:Write End,Begin Read.\r\n");
// 
//  for(INT32U i=0;i<EEP_MAX_SIZE;i++)
//  {
//    dat=E2p_byte_read(EEP_ADDRESS,i);   //里面有15mS的延时，故67循环为大概1S
//    if(dat!=(INT8U)(1+(i/256)))
//    {
//      nErrCount++;
//    }
//    if((i%67)==0)
//    {
//      printf("Time Total=%lu S,Eclipse %lu S.\r\n",(INT32U)(EEP_MAX_SIZE/67),(i/67)); 
//    } 
//  }  
//   printf("\r\nE2p Test:End,Total Size=%lu Byte,ErrCount=%lu Byte.\r\n",(INT32U)(EEP_MAX_SIZE),nErrCount);
  
  ////Stage 2:test Byte read and Byte write,Block write and Block read.
  {
    INT8U src[16]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    INT8U dst[16]={0};
    INT8U ch=0,ret=0;
    BOOL bFlag=FALSE;
    
    for(int i=0;i<16;i++)
    {
      ch=0x5a;
      bFlag=E2p_byte_write(EEP_ADDRESS, i, ch);
      delay_MS(5);
      printf("ByteWrite:addr=%d,data=%d,bFlag=%d.\r\n",i,i,bFlag);
    }
    
    for(int i=0;i<16;i++)
    {
      ch=E2p_byte_read(EEP_ADDRESS, i);
            delay_MS(5);
      printf("ByteRead:addr=%d,data=%d,E2pErrCode=%d.\r\n",i,ch,E2pErrCode);
    }
    
    ret=E2p_block_write(EEP_ADDRESS, 0, src, 16);
    printf("BlockWrite:ret=%d.\r\n",ret);
 
    
    for(int i=0;i<16;i++)
    {
      ch=E2p_byte_read(EEP_ADDRESS, i);
            delay_MS(5);
      printf("ByteRead:addr=%d,data=%d,E2pErrCode=%d.\r\n",i,ch,E2pErrCode);
    }    
    
    ret=E2p_block_read(EEP_ADDRESS, 0, dst, 16);
    printf("BlockRead:ret=%d.\r\n",ret);
    
    while(1);
  }
  
}

BOOL Para_Read(INT16U ee_addr, INT8U *data, INT16U len)
{
  //Block_Read 测试有问题，不能正常读取，所以这里牺牲效率，选择Byte_Read
  for(int i=0;i<len;i++)
  {
    data[i]=E2p_byte_read(EEP_ADDRESS, ee_addr+i);
  }
  
  if((DevInfo.EepDebugLevel&DBG_MSG))//
  {
    printf("MSG-E2P:");
    for(int j=0;j<len;j++)
    {    
      printf("0x%02X.",data[j]);
    }  
    printf("\r\n\r\n");
  }  
  
  return TRUE;		
}


BOOL Para_Save(INT16U ee_addr, INT8U *dat, INT16U len)
{
  INT8U i=0,nErrCount=0;

  
  while(nErrCount<3)//连续尝试3次
  {
    E2p_block_write(EEP_ADDRESS,ee_addr, dat, len);
    if((DevInfo.EepDebugLevel&DBG_MSG))//
    {
      printf("MSG-E2P:");
      for(int j=0;j<len;j++)
      {    
        printf("0x%02X.",dat[j]);
      }  
      printf("\r\n\r\n");
    }   


    
    
    for(i=0;i<len;i++)
    {
      INT8U ch=0;
      ch=E2p_byte_read(EEP_ADDRESS, ee_addr+i);    
      if(ch!=dat[i])
      {
        if(E2pErrCode!=0)
        {
          if((DevInfo.GpsDebugLevel&DBG_ERR))//
          {
            printf("ERR-E2P:E2P Not Mount,ErrCode=%d\r\n",E2pErrCode);
          }
        }
        nErrCount++;
        delay_MS(5);
        break;					
      }
    }
    if(i>=len)//写成功
    {
      if((DevInfo.EepDebugLevel&DBG_INF))//
      {
        printf("INF-E2P:Write OK.\r\n");
      }
      				
      return TRUE;			
    }			
  }
  if((DevInfo.EepDebugLevel&DBG_ERR))//
  {
    printf("ERR-E2P:E2P Write Error.\r\n");
  }
  
  return FALSE;
}




void E2p_Init()
{
  i2c_init();
}

INT8U E2p_byte_read(INT8U dev_addr, INT16U ee_addr)
{
  INT8U ch = 0;
  E2pErrCode=0;
  if (!i2c_start())
  {		
    E2pErrCode=1;
    return FALSE;
  }
  
  i2c_send_byte(dev_addr&0xFE);		//设置器件地址+段地址 
  if(!i2c_wait_ack())
  {
    i2c_stop(); 
    E2pErrCode=2;
    return FALSE;
  }
  i2c_send_byte(ee_addr>>8);  //设置高地址
  i2c_wait_ack();
  i2c_send_byte(ee_addr&0xFF);  //设置低地址
  i2c_wait_ack();
  
  i2c_start();
  i2c_send_byte((dev_addr&0xFE)+1);
  i2c_wait_ack();
  
  ch = i2c_read_byte();
  i2c_no_ack();
  
  i2c_stop();
  delay_MS(15);
  return ch;	
}


BOOL E2p_byte_write(INT8U dev_addr, INT16U ee_addr, INT8U ch)
{
  if (!i2c_start()) 
    return FALSE;
  
  i2c_send_byte(dev_addr&0xFE);		//设置器件地址+段地址 
  if (!i2c_wait_ack())
  {
    i2c_stop(); 
    return FALSE;
  }
  
  i2c_send_byte(ee_addr>>8);	//设置段内地址
  i2c_wait_ack();
  
  i2c_send_byte(ee_addr&0xFF);	//设置段内地址
  i2c_wait_ack();
  
  i2c_send_byte(ch);
  i2c_wait_ack();
  
  i2c_stop();
  
  delay_MS(30);
  return TRUE;
}

BOOL E2p_block_read(INT8U dev_addr, INT16U ee_addr ,INT8U* pDst,INT8U len)
{
  INT8U i=0;
  E2pErrCode=0;
  if (!i2c_start())
  {		
    E2pErrCode=1;
    return FALSE;
  }
  
  i2c_send_byte(dev_addr&0xFE);		//设置器件地址+段地址 
  if(!i2c_wait_ack())
  {
    i2c_stop(); 
    E2pErrCode=2;
    return FALSE;
  }

  i2c_send_byte(ee_addr>>8);	//设置段内地址
  i2c_wait_ack();
  
  i2c_send_byte(ee_addr&0xFF);  //设置低地址
  i2c_wait_ack();
  
  i2c_start();
  i2c_send_byte((dev_addr&0xFE)+1);
  i2c_wait_ack();
  
  for(i=0;i<len;i++)
  {
    pDst[i]=i2c_read_byte();
    i2c_wait_ack();
  }
  
  i2c_stop();
  delay_MS(15);
  return TRUE;	
}

BOOL E2p_block_write(INT8U dev_addr, INT16U ee_addr, INT8U *dat, INT16U len)
{
  if (!i2c_start()) 
    return FALSE;
  
  i2c_send_byte(dev_addr&0xFE);		//设置器件地址+段地址 
  if (!i2c_wait_ack())
  {
    i2c_stop(); 
    return FALSE;
  }
  
  i2c_send_byte(ee_addr>>8);	//设置段内地址
  i2c_wait_ack();
  
  i2c_send_byte(ee_addr&0xFF);	//设置段内地址
  i2c_wait_ack();
  
  while(len--)
  {
    i2c_send_byte(*dat++);
    i2c_wait_ack();
  }
  i2c_stop();
  
  delay_MS(30);
  return TRUE;
}
