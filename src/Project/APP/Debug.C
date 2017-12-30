#include "app.h"

#include <ctype.h>
char *strupr(char *str){
    char *orign=str;
    for (; *str!='\0'; str++)
        *str = toupper(*str);
    return orign;
}

INT8U DBG_RxBuf[MAX_DBG_RxBUF_SIZE];
INT8U DBG_RxFlag=FALSE;
INT8U DBG_RxCount=0;
INT8U DBG_TimeOut_Count=0xff;//串口通信超时时间，超时意味着一帧接收完成。

void Task_Dbg()
{ 
  if(DBG_RxFlag==TRUE)//收到数据
  {
    //static BOOL IsDbgMsgDispFlag=TRUE;
    INT8U tmpBuf[MAX_DBG_RxBUF_SIZE];
  
    DBG_RxFlag=FALSE;
    memcpy(tmpBuf,DBG_RxBuf,DBG_RxCount);
    memset(DBG_RxBuf,0,MAX_DBG_RxBUF_SIZE);
    DBG_RxCount=0;
    //sprintf(tmpBuf,"%S",DBG_RxBuf);
//////    if(IsDbgMsgDispFlag)//
//////    {
//////      printf("\r\nMSG-RCV:%s,nCount=%d.\r\n\r\n",DBG_RxBuf,DBG_RxCount);//转换为大写后的字符串
//////      printf("\r\nMSG-RCV:%s,nCount=%d.\r\n\r\n",strupr((char*)tmpBuf),nLen);//转换为大写后的字符串
//////
//////    }  
    strupr((char*)tmpBuf);//转换成大写字符
    if((tmpBuf[0]=='D')&&(tmpBuf[1]=='B')&&(tmpBuf[2]=='G')&&(tmpBuf[3]=='O')&&(tmpBuf[4]=='N')&&(tmpBuf[5]=='*'))//###DBGON***
    {
      DevInfo.SysDebugLevel=(DBG_ERR|DBG_INF|DBG_MSG);
      DevInfo.ChgDebugLevel=(DBG_ERR|DBG_INF|DBG_MSG);
      
      printf("DBG-MSG:DbgSYS=0x%02X,DbgSUB=0x%02X,DbgCHG=0x%02X.\r\n",
             DevInfo.SysDebugLevel,DevInfo.SubDebugLevel,DevInfo.ChgDebugLevel);    
    }   
    else if((tmpBuf[0]=='D')&&(tmpBuf[1]=='B')&&(tmpBuf[2]=='G')&&(tmpBuf[3]=='O')&&(tmpBuf[4]=='F')&&(tmpBuf[5]=='F')&&(tmpBuf[6]=='*'))//###DBGOFF***
    {
      DevInfo.SysDebugLevel=0;
      DevInfo.SubDebugLevel=0;
      DevInfo.ChgDebugLevel=0;    
      printf("DBG-MSG:DbgSYS=0x%02X,DbgSUB=0x%02X,DbgCHG=0x%02X.\r\n",
             DevInfo.SysDebugLevel,DevInfo.SubDebugLevel,DevInfo.ChgDebugLevel);
    } 
      
    else if((tmpBuf[0]=='D')&&(tmpBuf[1]=='B')&&(tmpBuf[2]=='G')
            &&(tmpBuf[3]=='O')&&(tmpBuf[4]=='F')&&(tmpBuf[5]=='F')
            //&&(tmpBuf[6]=='I')&&(tmpBuf[7]=='N')&&(tmpBuf[8]=='F')  
              &&(tmpBuf[9]=='*'))                                                                     //###DBGOFF_ _ _***
    {
      if((tmpBuf[6]=='I')&&(tmpBuf[7]=='N')&&(tmpBuf[8]=='F'))//INF
      {
        DevInfo.SysDebugLevel&=(~DBG_INF);
        DevInfo.SubDebugLevel&=(~DBG_INF);
        DevInfo.ChgDebugLevel&=(~DBG_INF);   
      }
      else if((tmpBuf[6]=='M')&&(tmpBuf[7]=='S')&&(tmpBuf[8]=='G'))//MSG
      {
        DevInfo.SysDebugLevel&=(~DBG_MSG);
        DevInfo.SubDebugLevel&=(~DBG_MSG);
        DevInfo.ChgDebugLevel&=(~DBG_MSG);   
      }
      else if((tmpBuf[6]=='R')&&(tmpBuf[7]=='R')&&(tmpBuf[8]=='R'))//ERR
      {
        DevInfo.SysDebugLevel&=(~DBG_ERR);
        DevInfo.SubDebugLevel&=(~DBG_ERR);
        DevInfo.ChgDebugLevel&=(~DBG_ERR);   
      }
      else
      {
        printf("\r\nDBG-ERR:CMD ERR.\r\n\r\n");
        return;
      }     
      printf("DBG-MSG:DbgSYS=0x%02X,DbgSUB=0x%02X,DbgCHG=0x%02X.\r\n",
             DevInfo.SysDebugLevel,DevInfo.SubDebugLevel,DevInfo.ChgDebugLevel);
    }  
    
    else if((tmpBuf[0]=='D')&&(tmpBuf[1]=='B')&&(tmpBuf[2]=='G')
            &&(tmpBuf[3]=='O')&&(tmpBuf[4]=='N')
              //&&(tmpBuf[6]=='I')&&(tmpBuf[7]=='N')&&(tmpBuf[8]=='F')  
              &&(tmpBuf[8]=='*'))                                                                     //###DBGON_ _ _***
    {
      if((tmpBuf[5]=='I')&&(tmpBuf[6]=='N')&&(tmpBuf[7]=='F'))//INF
      {
        DevInfo.SysDebugLevel|=(DBG_INF);
        DevInfo.SubDebugLevel|=(DBG_INF);
        DevInfo.ChgDebugLevel|=(DBG_INF);   
      }
      else if((tmpBuf[5]=='M')&&(tmpBuf[6]=='S')&&(tmpBuf[7]=='G'))//MSG
      {
        DevInfo.SysDebugLevel|=(DBG_MSG);
        DevInfo.SubDebugLevel|=(DBG_MSG);
        DevInfo.ChgDebugLevel|=(DBG_MSG);         
  
      }
      else if((tmpBuf[5]=='R')&&(tmpBuf[6]=='R')&&(tmpBuf[7]=='R'))//ERR
      {
        DevInfo.SysDebugLevel|=(DBG_ERR);
        DevInfo.SubDebugLevel|=(DBG_ERR);
        DevInfo.ChgDebugLevel|=(DBG_ERR);    
      }
      else
      {
        printf("\r\nDBG-ERR:CMD ERR.\r\n\r\n");
      }     
      printf("DBG-MSG:DbgSYS=0x%02X,DbgSUB=0x%02X,DbgCHG=0x%02X.\r\n",
             DevInfo.SysDebugLevel,DevInfo.SubDebugLevel,DevInfo.ChgDebugLevel);
    } 
    
    else if((tmpBuf[0]=='D')&&(tmpBuf[1]=='B')&&(tmpBuf[2]=='G')
             &&(tmpBuf[6]=='=')  
              &&(tmpBuf[8]=='*'))                                                                     //###DBG_ _ _=_***
    {
      INT8U ch=(tmpBuf[7]&0x0f);
      
      printf("\r\nDBG-INF:CMD val=%d.\r\n\r\n",ch);
      if(ch>7)
      {
        printf("\r\nDBG-ERR:CMD ERR,val=%d.\r\n\r\n",ch);
        return;
      } 
      
      if((tmpBuf[3]=='S')&&(tmpBuf[4]=='Y')&&(tmpBuf[5]=='S'))
      {
        DevInfo.SysDebugLevel=ch;       
      }
      else if((tmpBuf[3]=='S')&&(tmpBuf[4]=='U')&&(tmpBuf[5]=='B'))
      {
        DevInfo.SubDebugLevel=ch;       
      }
      else if((tmpBuf[3]=='C')&&(tmpBuf[4]=='H')&&(tmpBuf[5]=='G'))
      {
        DevInfo.ChgDebugLevel=ch;       
      }
      else
      {
        printf("\r\nDBG-ERR:CMD ERR.\r\n\r\n");
        return;     
      }
      printf("DBG-MSG:DbgSYS=0x%02X,DbgSUB=0x%02X,DbgCHG=0x%02X.\r\n",
             DevInfo.SysDebugLevel,DevInfo.SubDebugLevel,DevInfo.ChgDebugLevel);
      
    }
  }
}