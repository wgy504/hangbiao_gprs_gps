#include "app.h"

//variable
INT8U nStatus=0;				/*GPRS״̬*/
INT32U	nTimeTick=0;			/*��ʱ������������GPRS��ʱ�ж�*/





BOOL GPRS_RxFlag=FALSE;
INT8U GPRS_RxBuf[MAX_GPRS_RxBUF_SIZE];
INT16U GPRS_RxCount=0;
INT8U GPRS_TimeOut_Count=0xff;//����ͨ�ų�ʱʱ�䣬��ʱ��ζ��һ֡������ɡ�

//�ڲ���������	

BOOL CheckSendAck(INT8U* pBuffer,INT8U Len);
BOOL CheckSendOK(INT8U* pBuffer,INT8U Len);

BOOL GetIMEICode(INT8U* pDst,INT8U* pBuffer,INT8U Len);
BOOL CheckOKAck(INT8U* pBuffer,INT8U Len);
BOOL CheckRegStatus(INT8U* pBuffer,INT8U Len);	 
INT8U CheckCSQStatus(INT8U* pBuffer,INT8U Len);
BOOL CheckConnect(INT8U* pBuffer,INT8U Len);	 
BOOL Clear_RxBuf(void);

void GPRS_OutData(INT8U *Data,INT16U Len)
{
  for(int i=0;i<Len;i++)
  {
    Uart1_SendByte(*Data++);
  }
}


    //�����Ļ���GPRS���ϱ�ģʽ����ʱ����->�ϱ�->�ػ���
    //�ϱ�ʱ����n�Σ������ɹ�������GPRS���������³��ԣ�
    //�����涨�������������������3�Σ���δ����ϱ������¼���������ҹر�GPRS��
void Task_Gprs(void)
{
  static BOOL IsFirstStartFlag=TRUE;
  static BOOL IsPrintFlag=FALSE;

  static INT8U	nErrCount=0;			/*�������������GPRS�������*/
  static INT8U  nRstCount=0;                    /*��������������GPRS��������*/
  
  static INT32U nCircleTick=0;//����ʱ��

  
  if(OSTimeGet()<DevInfo.GpsMaxRunTime)//�ȴ�һ��GPS��λ����
  {
    static INT32U LastTick=0;    
    if(DevData.GpsInfo.ValidFlag==FALSE)//��û�ж�λ�ɹ�
    {
      if(LastTick!=OSTimeGet())
      {
        LastTick=OSTimeGet();
        if((DevInfo.GprsDebugLevel&DBG_INF))//
        {    
          printf("INF-GPRS:20%02d-%02d-%02d %02d:%02d:%02d Waiting for GPS first positioning.\r\n",
                 DevData.TimeInfo.year,DevData.TimeInfo.month,DevData.TimeInfo.day,DevData.TimeInfo.hour,DevData.TimeInfo.minute,DevData.TimeInfo.second);
        }
      } 
      return;
    }   
  }
  
  //////////for debug
//////  {
//////    static INT32U testTick=0;
//////    if(testTick!=OSTimeGet())
//////    {
//////      testTick=OSTimeGet();
//////      if((DevInfo.GprsDebugLevel&DBG_INF))//
//////      {    
//////        printf("INF-GPRS:OSTimeGet=%lu,nCircleTick=%lu,nState=%d,nErrCount=%d,nRstCount=%d.\r\n",testTick,nCircleTick,nStatus,nErrCount,nRstCount);
//////      }
//////    } 
//////  }
  
  if((OSTimeGet()-nCircleTick>=DevInfo.GprsIntval)||(IsFirstStartFlag))//��һ����������������ѭ������
  {
    nStatus=0;//״̬����λ
    nErrCount=0;
    nTimeTick=0;
    nRstCount=0;//������������
    
    IsFirstStartFlag=FALSE;
    IsPrintFlag=FALSE;//�Ƿ��Ѿ���ӡ��־
    nCircleTick=OSTimeGet();//���¼�ʱ
    if((DevInfo.GprsDebugLevel&DBG_INF))//
    {    
       printf("INF-GPRS:20%02d-%02d-%02d %02d:%02d:%02d Gprs State Machine Init.\r\n",
           DevData.TimeInfo.year,DevData.TimeInfo.month,DevData.TimeInfo.day,DevData.TimeInfo.hour,DevData.TimeInfo.minute,DevData.TimeInfo.second);
    }
  }
  else
  {
    if(nRstCount>=MAX_GPRS_RST_COUNT)//������������������ر�GPRS
    {
      if(IsPrintFlag==FALSE)
      {
        IsPrintFlag=TRUE;
        //�ر�gprs,���־λ
        GPRS_PWR_EN(FALSE);
        if((DevInfo.GprsDebugLevel&DBG_INF))//
        {         
          printf("INF-GPRS:20%02d-%02d-%02d %02d:%02d:%02d nRstCount>=MAX_GPRS_RST_COUNT,Close the Module.\r\n",
                 DevData.TimeInfo.year,DevData.TimeInfo.month,DevData.TimeInfo.day,DevData.TimeInfo.hour,DevData.TimeInfo.minute,DevData.TimeInfo.second);
        }  
      }   
      return;      
    }
    else
    {
      if(nStatus==STATE_SUCCESS)//�ϱ��ɹ�(����254Ϊ�����ɹ�״̬)
      {
        if(IsPrintFlag==FALSE)
        {
          IsPrintFlag=TRUE;
          //�ر�gprs,���־λ
          GPRS_PWR_EN(FALSE);
          if((DevInfo.GprsDebugLevel&DBG_INF))//
          {         
            printf("INF-GPRS:20%02d-%02d-%02d %02d:%02d:%02d Upload Success,Close the Module.\r\n",
                   DevData.TimeInfo.year,DevData.TimeInfo.month,DevData.TimeInfo.day,DevData.TimeInfo.hour,DevData.TimeInfo.minute,DevData.TimeInfo.second);
          }  
        }  
        return;
      }
      
    }
    
    //״̬��
    switch(nStatus)
    {
    case 0:	 //�رյ�Դ
      {
        if(nTimeTick==0)
        {
          
          GPRS_PWR_EN(FALSE);
          nTimeTick=OSTimeGet();
          DevData.IsDevLogin=FALSE;   //�豸û�е�¼��������
          DevData.CSQ=0;              //
          memset(DevData.IMEI,0x00,8);//���IMEI��
          if((DevInfo.GprsDebugLevel&DBG_INF))//
          {
            printf("INF-GPRS:Power Off.\r\n");
          }
        }
        else
        {
          INT32U CurrTimeTick=OSTimeGet();
          if((CurrTimeTick-nTimeTick)>=MAX_GPRS_RST_TIME)
          {
            if((DevInfo.GprsDebugLevel&DBG_INF))//
            {
              printf("INF-GPRS:Power On.\r\n");
            }   
            
            nStatus++;
            nErrCount=0;
            nTimeTick=0;
            GPRS_PWR_EN(TRUE);
         
          }          
        }
        break;
      }      
    case 1:	 //�豸Ӳ����ʼ��
      {
        if(nTimeTick==0)
        {        
          Clear_RxBuf();
          nTimeTick=OSTimeGet();
        }
        else
        {
          INT32U CurrTimeTick=OSTimeGet();
          if((CurrTimeTick-nTimeTick)>=MAX_GPRS_BOOT_TIME)
          {
            if((DevInfo.GprsDebugLevel&DBG_INF))//
            {
              printf("INF-GPRS:PowerOn Finished,nStatus=%d\r\n",nStatus);
            }  
            nStatus++;
            nErrCount=0;
            nTimeTick=0;       
          }          
        }
        break;
      }
      
    case 2:	//����ź�����
      {
        if(nTimeTick==0)
        {
          Clear_RxBuf();
          nTimeTick=OSTimeGet();
          Uart1_printf("\r\nAT+CSQ\r\n" );
          
          if((DevInfo.GprsDebugLevel&DBG_MSG))//
          {
            printf("MSG-GPRS:AT_AT+CSQ,nStatus=%d\r\n",nStatus);
          }           
        }
        else
        {
          if((OSTimeGet()-nTimeTick)>=MAX_GPRS_AT_TIMEOUT)
          {
            if(++nErrCount>=MAX_GPRS_ERR_COUNT)	//��������Դ���������GPRSģ��
            {
              if((DevInfo.GprsDebugLevel&DBG_ERR))//
              {
                printf("ERR_GPRS:nErrCount>=MAX_GPRS_ERR_COUNT,restart the Module,nStatus=%d,nRstCount=%d.\r\n",nStatus,nRstCount);
              }               
              //����״̬��
              nStatus=0;
              nErrCount=0;
              nTimeTick=0;  
              nRstCount++;//������������
              
                        
            }
            else//�ط�ATָ��
            {
              nTimeTick=0;
            }
          }
          else
          {
            if(GPRS_RxFlag==TRUE)//�յ�����
            {
              GPRS_RxFlag=FALSE;
              if((DevInfo.GprsDebugLevel&DBG_MSG))//
              {
                printf("MSG-GPRS:RxBuf=%s,nStatus=%d.\r\n",GPRS_RxBuf,nStatus);
              } 
              
              if(CheckOKAck(GPRS_RxBuf,GPRS_RxCount))//��⵽OK
              {
                DevData.CSQ=CheckCSQStatus(GPRS_RxBuf,GPRS_RxCount);
                if(DevData.CSQ<99)
                {                 
                  if((DevInfo.GprsDebugLevel&DBG_INF))//
                  {
                    printf("\r\nINF-GPRS:Check CSQ Success,CSQ=%d,nStatus=%d.\r\n",DevData.CSQ,nStatus );
                  }  
                  nStatus++;								
                  nErrCount=0;
                  nTimeTick=0;  
                }
                else
                {
                  if((DevInfo.GprsDebugLevel&DBG_ERR))//
                  {
                    printf("\r\nERR-GPRS:Check CSQ Error,CSQ=%d,nStatus=%d.\r\n",DevData.CSQ,nStatus );
                  }                  
                }               
              }
            }           
          }					
        }
        break;
      }      
      
    case 3:	//AT+CREG?//��ģ�鼤��GPRS
      {
        if(nTimeTick==0)
        {
          Uart1_printf("\r\nAT+CREG?\r\n" );
          Clear_RxBuf();
          nTimeTick=OSTimeGet();
          if((DevInfo.GprsDebugLevel&DBG_MSG))//
          {
            printf("MSG-GPRS:AT_AT+CREG?,nStatus=%d\r\n",nStatus);
          }  
        }
        else
        {
          if((OSTimeGet()-nTimeTick)>=MAX_GPRS_AT_TIMEOUT)
          {
            if(++nErrCount>=MAX_GPRS_ERR_COUNT)	//��������Դ���������GPRSģ��
            {
              if((DevInfo.GprsDebugLevel&DBG_ERR))//
              {
                printf("ERR_GPRS:nErrCount>=MAX_GPRS_ERR_COUNT,restart the Module,nStatus=%d,nRstCount=%d.\r\n",nStatus,nRstCount);
              }               
              //����״̬��
              nStatus=0;
              nErrCount=0;
              nTimeTick=0;  
              nRstCount++;//������������
            }
            else//�ط�ATָ��
            {
              nTimeTick=0;
            }
          }
          else
          {
            if(GPRS_RxFlag==TRUE)//�յ�����
            {
              GPRS_RxFlag=FALSE;
              if((DevInfo.GprsDebugLevel&DBG_MSG))//
              {
                printf("MSG-GPRS:RxBuf=%s,nStatus=%d.\r\n",GPRS_RxBuf,nStatus);
              }
              if(CheckRegStatus(GPRS_RxBuf,GPRS_RxCount))//��⵽OK
              {
                if((DevInfo.GprsDebugLevel&DBG_INF))//
                {
                  printf("\r\nINF-GPRS:Check OK Success,nStatus=%d.\r\n",nStatus );
                } 
                nStatus++;
                nErrCount=0;
                nTimeTick=0;
              }
            }
          }					
        }
        break;
      }
      
    case 4:	//AT+CIICR����GPRS
      {
        if(nTimeTick==0)
        {
          Uart1_printf("\r\nAT+CIICR\r\n" );
          Clear_RxBuf();
          nTimeTick=OSTimeGet();
          if((DevInfo.GprsDebugLevel&DBG_MSG))//
          {
            printf("MSG-GPRS:AT_AT+CIICR,nStatus=%d\r\n",nStatus);
          } 
        }
        else
        {
          if((OSTimeGet()-nTimeTick)>=MAX_GPRS_AT_TIMEOUT)
          {
            if(++nErrCount>=MAX_GPRS_ERR_COUNT)	//��������Դ���������GPRSģ��
            {
              if((DevInfo.GprsDebugLevel&DBG_ERR))//
              {
                printf("ERR_GPRS:nErrCount>=MAX_GPRS_ERR_COUNT,restart the Module,nStatus=%d,nRstCount=%d.\r\n",nStatus,nRstCount);
              }               
              //����״̬��
              nStatus=0;
              nErrCount=0;
              nTimeTick=0;  
              nRstCount++;//������������
            }
            else//�ط�ATָ��
            {
              nTimeTick=0;
            }
          }
          else
          {
            if(GPRS_RxFlag==TRUE)//�յ�����
            {
              GPRS_RxFlag=FALSE;
              if((DevInfo.GprsDebugLevel&DBG_MSG))//
              {
                printf("MSG-GPRS:RxBuf=%s,nStatus=%d.\r\n",GPRS_RxBuf,nStatus);
              }			
              if(CheckOKAck(GPRS_RxBuf,GPRS_RxCount))//��⵽OK
              {
                if((DevInfo.GprsDebugLevel&DBG_INF))//
                {
                  printf("\r\nINF-GPRS:Check OK Success,nStatus=%d.\r\n",nStatus );
                } 
                nStatus++;
                nErrCount=0;
                nTimeTick=0;
              }
            }
          }					
        }
        break;  
      }
      
    case 5:	//AT+CIPMUX=0//������ģʽ
      {
        if(nTimeTick==0)
        {
          Uart1_printf("\r\nAT+CIPMUX=0\r\n");
          Clear_RxBuf();
          nTimeTick=OSTimeGet();
          if((DevInfo.GprsDebugLevel&DBG_MSG))//
          {
            printf("MSG-GPRS:AT_AT+CIPMUX=0,nStatus=%d\r\n",nStatus);
          }           
        }
        else
        {
          if((OSTimeGet()-nTimeTick)>=MAX_GPRS_AT_TIMEOUT)
          {
            if(++nErrCount>=MAX_GPRS_ERR_COUNT)	//��������Դ���������GPRSģ��
            {
              if((DevInfo.GprsDebugLevel&DBG_ERR))//
              {
                printf("ERR_GPRS:nErrCount>=MAX_GPRS_ERR_COUNT,restart the Module,nStatus=%d,nRstCount=%d.\r\n",nStatus,nRstCount);
              }               
              //����״̬��
              nStatus=0;
              nErrCount=0;
              nTimeTick=0;  
              nRstCount++;//������������
            }
            else//�ط�ATָ��
            {
              nTimeTick=0;
            }
          }
          else
          {
            if(GPRS_RxFlag==TRUE)//�յ�����
            {
              GPRS_RxFlag=FALSE;
              if((DevInfo.GprsDebugLevel&DBG_MSG))//
              {
                printf("MSG-GPRS:RxBuf=%s,nStatus=%d.\r\n",GPRS_RxBuf,nStatus);
              }				
              if(CheckOKAck(GPRS_RxBuf,GPRS_RxCount))//��⵽OK
              {
                if((DevInfo.GprsDebugLevel&DBG_INF))//
                {
                  printf("\r\nINF-GPRS:Check OK Success,nStatus=%d.\r\n",nStatus );
                } 
                nStatus++;
                nErrCount=0;
                nTimeTick=0;
              }
            }
          }					
        }
        break;
      }
      
    case 6:	//AT+CIPMODE=1,1//͸�����ܾ��绰��0,0����͸���������绰
      {
        if(nTimeTick==0)
        {
          Uart1_printf("\r\nAT+CIPMODE=1,1\r\n");
          Clear_RxBuf();
          nTimeTick=OSTimeGet();
          if((DevInfo.GprsDebugLevel&DBG_MSG))//
          {
            printf("MSG-GPRS:AT_AT+CIPMODE=1,1,nStatus=%d\r\n",nStatus);
          } 
        }
        else
        {
          if((OSTimeGet()-nTimeTick)>=MAX_GPRS_AT_TIMEOUT)
          {
            if(++nErrCount>=MAX_GPRS_ERR_COUNT)	//��������Դ���������GPRSģ��
            {
             
              
              //nStatus=0;
              nStatus++;	//��Ϊ�еĿ��ǲ�֧������ʱ��ģ��������κ󣬾�ֱ��������
              nErrCount=0;
              nTimeTick=0; 
              
              if((DevInfo.GprsDebugLevel&DBG_INF))//
              {
                printf("INF_GPRS:Get Net Time Failed,nStatus=%d.\r\n",nStatus );
              }
            }
            else
            {
              nTimeTick=0;
            }
            
          }
          else
          {
            if(GPRS_RxFlag==TRUE)//�յ�����
            {
              GPRS_RxFlag=FALSE;
              if((DevInfo.GprsDebugLevel&DBG_MSG))//
              {
                printf("MSG-GPRS:RxBuf=%s,nStatus=%d.\r\n",GPRS_RxBuf,nStatus);
              }				
              if(CheckOKAck(GPRS_RxBuf,GPRS_RxCount))//��⵽OK
              {
                if((DevInfo.GprsDebugLevel&DBG_INF))//
                {
                  printf("\r\nINF-GPRS:Check OK Success,nStatus=%d.\r\n",nStatus );
                } 
                nStatus++;
                nErrCount=0;
                nTimeTick=0;
              }
            }
          }					
        }
        break;
      }
      
    case 7:	//����APN  AT+CSTT="CMNET"
      {
        if(nTimeTick==0)
        {
          Uart1_printf("\r\nAT+CSTT=\"%s\"\r\n" ,DevInfo.SvrAPN );
          Clear_RxBuf();
          nTimeTick=OSTimeGet();
          
          if((DevInfo.GprsDebugLevel&DBG_MSG))//
          {
            printf("MSG-GPRS:AT_AT+CSTT=\"%s\",nTimeTick=%lu\r\n" ,DevInfo.SvrAPN,nTimeTick );
          }           
        }
        else
        {
          if((OSTimeGet()-nTimeTick)>=MAX_GPRS_AT_TIMEOUT)
          {
            if(++nErrCount>=MAX_GPRS_ERR_COUNT)	//��������Դ���������GPRSģ��
            {
              if((DevInfo.GprsDebugLevel&DBG_ERR))//
              {
                printf("ERR_GPRS:nErrCount>=MAX_GPRS_ERR_COUNT,restart the Module,nStatus=%d,nRstCount=%d.\r\n",nStatus,nRstCount);
              }               
              //����״̬��
              nStatus=0;
              nErrCount=0;
              nTimeTick=0;  
              nRstCount++;//������������
            }
            else//�ط�ATָ��
            {
              nTimeTick=0;
            }
          }
          else
          {
            if(GPRS_RxFlag==TRUE)//�յ�����
            {
              GPRS_RxFlag=FALSE;
              if((DevInfo.GprsDebugLevel&DBG_MSG))//
              {
                printf("MSG-GPRS:RxBuf=%s,nStatus=%d.\r\n",GPRS_RxBuf,nStatus);
              }
              if(CheckOKAck(GPRS_RxBuf,GPRS_RxCount))//��⵽OK
              {
                if((DevInfo.GprsDebugLevel&DBG_INF))//
                {
                  printf("\r\nINF-GPRS:Check OK Success,nStatus=%d.\r\n",nStatus );
                } 
                nStatus++;
                nErrCount=0;
                nTimeTick=0;
              }
            }
          }					
        }
        break;
      }
      
    case 8:	//����AT+CIPCFG=0,50,1,60//
      {
        if(nTimeTick==0)
        {
          Uart1_printf("\r\nAT+CIPCFG=0,50,1,60\r\n");
          Clear_RxBuf();
          nTimeTick=OSTimeGet();
          if((DevInfo.GprsDebugLevel&DBG_MSG))//
          {
            printf("MSG-GPRS:AT_AT+CIPCFG=0,50,1,60,nStatus=%d\r\n",nStatus);
          }           
        }
        else
        {
          if((OSTimeGet()-nTimeTick)>=MAX_GPRS_AT_TIMEOUT)
          {
            if(++nErrCount>=MAX_GPRS_ERR_COUNT)	//��������Դ���������GPRSģ��
            {
              if((DevInfo.GprsDebugLevel&DBG_ERR))//
              {
                printf("ERR_GPRS:nErrCount>=MAX_GPRS_ERR_COUNT,restart the Module,nStatus=%d,nRstCount=%d.\r\n",nStatus,nRstCount);
              }               
              //����״̬��
              nStatus=0;
              nErrCount=0;
              nTimeTick=0;  
              nRstCount++;//������������
            }
            else//�ط�ATָ��
            {
              nTimeTick=0;
            }
          }
          else
          {
            if(GPRS_RxFlag==TRUE)//�յ�����
            {
              GPRS_RxFlag=FALSE;
              if((DevInfo.GprsDebugLevel&DBG_MSG))//
              {
                printf("MSG-GPRS:RxBuf=%s,nStatus=%d.\r\n",GPRS_RxBuf,nStatus);
              }				
              if(CheckOKAck(GPRS_RxBuf,GPRS_RxCount))//��⵽OK
              {
                if((DevInfo.GprsDebugLevel&DBG_INF))//
                {
                  printf("\r\nINF-GPRS:Check OK Success,nStatus=%d.\r\n",nStatus );
                } 
                nStatus++;
                nErrCount=0;
                nTimeTick=0;
              }
            }
          }					
        }
        break;
      }
      
      
    case 9:	//�ر�����AT+CIPCLOSE//
      {
        if(nTimeTick==0)
        {
          Uart1_printf("\r\nAT+CSQ\r\n");
          Clear_RxBuf();
          nTimeTick=OSTimeGet();
          if((DevInfo.GprsDebugLevel&DBG_MSG))//
          {
            printf("MSG-GPRS:AT_AT+CSQ,nStatus=%d\r\n",nStatus);
          } 
        }
        else
        {
          if((OSTimeGet()-nTimeTick)>=MAX_GPRS_AT_TIMEOUT)
          {
            if(++nErrCount>=MAX_GPRS_ERR_COUNT)	//��������Դ���������GPRSģ��
            {
              if((DevInfo.GprsDebugLevel&DBG_ERR))//
              {
                printf("ERR_GPRS:nErrCount>=MAX_GPRS_ERR_COUNT,restart the Module,nStatus=%d,nRstCount=%d.\r\n",nStatus,nRstCount);
              }               
              //����״̬��
              nStatus=0;
              nErrCount=0;
              nTimeTick=0;  
              nRstCount++;//������������
            }
            else//�ط�ATָ��
            {
              nTimeTick=0;
            }
          }
          else
          {
            if(GPRS_RxFlag==TRUE)//�յ�����
            {
              GPRS_RxFlag=FALSE;
              if((DevInfo.GprsDebugLevel&DBG_MSG))//
              {
                printf("MSG-GPRS:RxBuf=%s,nStatus=%d.\r\n",GPRS_RxBuf,nStatus);
              }				
              if(CheckOKAck(GPRS_RxBuf,GPRS_RxCount))//��⵽OK
              {
                if((DevInfo.GprsDebugLevel&DBG_INF))//
                {
                  printf("\r\nINF-GPRS:Check OK Success,nStatus=%d.\r\n",nStatus );
                } 
                nStatus++;
                nErrCount=0;
                nTimeTick=0;
              }
            }
          }					
        }
        break;
      }
      
      
    case 10:	//���ý��յ�����Ϣ��ֱ����ʾ����
      {
        if(nTimeTick==0)
        {
          Uart1_printf("\r\nAT+CNMI=2,2\r\n");
          Clear_RxBuf();
          nTimeTick=OSTimeGet();
          if((DevInfo.GprsDebugLevel&DBG_MSG))//
          {
            printf("MSG-GPRS:AT_AT+CNMI=2,2,nStatus=%d\r\n",nStatus);
          }           
        }
        else
        {
          if((OSTimeGet()-nTimeTick)>=MAX_GPRS_AT_TIMEOUT)
          {
            if(++nErrCount>=MAX_GPRS_ERR_COUNT)	//��������Դ���������GPRSģ��
            {
              if((DevInfo.GprsDebugLevel&DBG_ERR))//
              {
                printf("ERR_GPRS:nErrCount>=MAX_GPRS_ERR_COUNT,restart the Module,nStatus=%d,nRstCount=%d.\r\n",nStatus,nRstCount);
              }               
              //����״̬��
              nStatus=0;
              nErrCount=0;
              nTimeTick=0;  
              nRstCount++;//������������
            }
            else//�ط�ATָ��
            {
              nTimeTick=0;
            }
          }
          else
          {
            if(GPRS_RxFlag==TRUE)//�յ�����
            {
              GPRS_RxFlag=FALSE;
              if((DevInfo.GprsDebugLevel&DBG_MSG))//
              {
                printf("MSG-GPRS:RxBuf=%s,nStatus=%d.\r\n",GPRS_RxBuf,nStatus);
              }			
              if(CheckOKAck(GPRS_RxBuf,GPRS_RxCount))//��⵽OK
              {
                if((DevInfo.GprsDebugLevel&DBG_INF))//
                {
                  printf("\r\nINF-GPRS:Check OK Success,nStatus=%d.\r\n",nStatus );
                } 
                nStatus++;
                nErrCount=0;
                nTimeTick=0;
              }
            }
          }					
        }
        break;
      }
      
      
    case 11:	//���ö���ϢΪ�ı���ʽ
      {
        if(nTimeTick==0)
        {
          Uart1_printf("\r\nAT+CMGF=1\r\n");
          Clear_RxBuf();
          nTimeTick=OSTimeGet();
          if((DevInfo.GprsDebugLevel&DBG_MSG))//
          {
            printf("MSG-GPRS:AT_AT+CMGF=1,nStatus=%d\r\n",nStatus);
          }           
        }
        else
        {
          if((OSTimeGet()-nTimeTick)>=MAX_GPRS_AT_TIMEOUT)
          {
            if(++nErrCount>=MAX_GPRS_ERR_COUNT)	//��������Դ���������GPRSģ��
            {
              if((DevInfo.GprsDebugLevel&DBG_ERR))//
              {
                printf("ERR_GPRS:nErrCount>=MAX_GPRS_ERR_COUNT,restart the Module,nStatus=%d,nRstCount=%d.\r\n",nStatus,nRstCount);
              }               
              //����״̬��
              nStatus=0;
              nErrCount=0;
              nTimeTick=0;  
              nRstCount++;//������������
            }
            else//�ط�ATָ��
            {
              nTimeTick=0;
            }
          }
          else
          {
            if(GPRS_RxFlag==TRUE)//�յ�����
            {
              GPRS_RxFlag=FALSE;
              if((DevInfo.GprsDebugLevel&DBG_MSG))//
              {
                printf("MSG-GPRS:RxBuf=%s,nStatus=%d.\r\n",GPRS_RxBuf,nStatus);
              }			
              if(CheckOKAck(GPRS_RxBuf,GPRS_RxCount))//��⵽OK
              {
                if((DevInfo.GprsDebugLevel&DBG_INF))//
                {
                  printf("\r\nINF-GPRS:Check OK Success,nStatus=%d.\r\n",nStatus );
                } 
                nStatus++;
                nErrCount=0;
                nTimeTick=0;
              }
            }
          }					
        }
        break;
      }
          
    case 12:	//��ѯIMEI����
      {
        if(nTimeTick==0)
        {
          Uart1_printf("\r\nAT+CGSN\r\n");
          Clear_RxBuf();
          nTimeTick=OSTimeGet();
          if((DevInfo.GprsDebugLevel&DBG_MSG))//
          {
            printf("MSG-GPRS:AT_AT+CGSN,nStatus=%d\r\n",nStatus);
          }
        }
        else
        {
          if((OSTimeGet()-nTimeTick)>=MAX_GPRS_AT_TIMEOUT)
          {
            if(++nErrCount>=MAX_GPRS_ERR_COUNT)	//��������Դ���������GPRSģ��
            {
              if((DevInfo.GprsDebugLevel&DBG_ERR))//
              {
                printf("ERR_GPRS:nErrCount>=MAX_GPRS_ERR_COUNT,restart the Module,nStatus=%d,nRstCount=%d.\r\n",nStatus,nRstCount);
              }               
              //����״̬��
              nStatus=0;
              nErrCount=0;
              nTimeTick=0;  
              nRstCount++;//������������
            }
            else//�ط�ATָ��
            {
              nTimeTick=0;
            }
          }
          else
          {
            if(GPRS_RxFlag==TRUE)//�յ�����
            {
              GPRS_RxFlag=FALSE;
              if((DevInfo.GprsDebugLevel&DBG_MSG))//
              {
                printf("MSG-GPRS:RxBuf=%s,nStatus=%d.\r\n",GPRS_RxBuf,nStatus);
              }				
              if(CheckOKAck(GPRS_RxBuf,GPRS_RxCount))//��⵽OK
              {
                if((DevInfo.GprsDebugLevel&DBG_INF))//
                {
                  printf("\r\nINF-GPRS:Check OK Success,nStatus=%d.\r\n",nStatus );
                } 
                nStatus++;
                nErrCount=0;
                nTimeTick=0;             
                if( GetIMEICode(DevData.IMEI,GPRS_RxBuf,GPRS_RxCount))//��ȡIMEI��
                {                 
                  if(DevInfo.SysRunMode==0)//��������ģʽ
                  {
                    //����ͨ�ŵ�ַ
                    memcpy(DevInfo.CommAddr,DevData.IMEI,8);
                    
                    if((DevInfo.GprsDebugLevel&DBG_INF))//
                    {
                      printf("\r\nINF-GPRS:Normal Mode,Update CommAddress With IMEI.\r\n");
                    }                     
                  }
                  else
                  {
                    DevInfo.CommAddr[0]=0x01;                                   /*�豸ͨ�ŵ�ַ,��λ��ǰ����λ�ں�*/
                    DevInfo.CommAddr[1]=0x02;
                    DevInfo.CommAddr[2]=0x03;
                    DevInfo.CommAddr[3]=0x04;
                    DevInfo.CommAddr[4]=0x05;
                    DevInfo.CommAddr[5]=0x06;
                    DevInfo.CommAddr[6]=0x07;
                    DevInfo.CommAddr[7]=0x08; 
                    
                    if((DevInfo.GprsDebugLevel&DBG_INF))//
                    {
                      printf("\r\nINF-GPRS:Debug Mode,No Update CommAddress With IMEI.\r\n");
                    }  
                  }
                      
                  if((DevInfo.GprsDebugLevel&DBG_MSG))//
                  {
                    printf("\r\nMSG-GPRS:IMEI=%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X.\r\n",
                           DevData.IMEI[0],DevData.IMEI[1],DevData.IMEI[2],DevData.IMEI[3],DevData.IMEI[4],DevData.IMEI[5],DevData.IMEI[6],DevData.IMEI[7]);
                  }                  
                }
                
                //GetIMEICode(DevData.IMEI,GPRS_RxBuf,GPRS_RxCount);
                
 
                
              }
            }
          }					
        }
        break;
      }
      
      
    case 13:	//��BT��Դ
      {
        if(nTimeTick==0)
        {
          Uart1_printf("\r\nAT+BTPWR=1\r\n");
          Clear_RxBuf();
          nTimeTick=OSTimeGet();
          if((DevInfo.GprsDebugLevel&DBG_MSG))//
          {
            printf("MSG-GPRS:AT_AT+BTPWR=1,nStatus=%d\r\n",nStatus);
          }
        }
        else
        {
          if((OSTimeGet()-nTimeTick)>=MAX_GPRS_AT_TIMEOUT)
          {
            if(++nErrCount>=MAX_GPRS_ERR_COUNT)	//��������Դ���������GPRSģ��
            {
              if((DevInfo.GprsDebugLevel&DBG_ERR))//
              {
                printf("ERR_GPRS:nErrCount>=MAX_GPRS_ERR_COUNT,restart the Module,nStatus=%d,nRstCount=%d.\r\n",nStatus,nRstCount);
              }               
              //����״̬��
              nStatus=0;
              nErrCount=0;
              nTimeTick=0;  
              nRstCount++;//������������
            }
            else//�ط�ATָ��
            {
              nTimeTick=0;
            }
          }
          else
          {
            if(GPRS_RxFlag==TRUE)//�յ�����
            {
              GPRS_RxFlag=FALSE;
              if((DevInfo.GprsDebugLevel&DBG_MSG))//
              {
                printf("MSG-GPRS:RxBuf=%s,nStatus=%d.\r\n",GPRS_RxBuf,nStatus);
              }				
              if(CheckOKAck(GPRS_RxBuf,GPRS_RxCount))//��⵽OK
              {
                if((DevInfo.GprsDebugLevel&DBG_INF))//
                {
                  printf("\r\nINF-GPRS:Check OK Success,nStatus=%d.\r\n",nStatus );
                } 
                nStatus++;
                nErrCount=0;
                nTimeTick=0;
              }
            }
          }					
        }
        break;
      }
      
      
    case 14:	//��������IP��Port  AT+CIPSTART="TCP","139.196.17.228",8888
      {
        if(nTimeTick==0)
        {
          Uart1_printf("AT+CIPSTART=\"TCP\",\"%s\",%d\r\n",DevInfo.SvrIP,DevInfo.SvrPort);
          Clear_RxBuf();
          nTimeTick=OSTimeGet();

          if((DevInfo.GprsDebugLevel&DBG_MSG))//
          {
            printf("MSG-GPRS:AT_AT+CIPSTART=\"TCP\",\"%s\",%d,nStatus=%d\r\n",DevInfo.SvrIP,DevInfo.SvrPort,nStatus);
          }
        }
        else
        {
          if((OSTimeGet()-nTimeTick)>=2*MAX_GPRS_AT_TIMEOUT)
          {
            if(++nErrCount>=MAX_GPRS_ERR_COUNT)	//��������Դ���������GPRSģ��
            {
              if((DevInfo.GprsDebugLevel&DBG_ERR))//
              {
                printf("ERR_GPRS:nErrCount>=MAX_GPRS_ERR_COUNT,restart the Module,nStatus=%d,nRstCount=%d.\r\n",nStatus,nRstCount);
              }               
              //����״̬��
              nStatus=0;
              nErrCount=0;
              nTimeTick=0;  
              nRstCount++;//������������
            }
            else//�ط�ATָ��
            {
              nTimeTick=0;
            }
          }
          else
          {
            if(GPRS_RxFlag==TRUE)//�յ�����
            {
              GPRS_RxFlag=FALSE;
              if((DevInfo.GprsDebugLevel&DBG_MSG))//
              {
                printf("MSG-GPRS:RxBuf=%s,nStatus=%d.\r\n",GPRS_RxBuf,nStatus);
              }				
              if(CheckConnect(GPRS_RxBuf,GPRS_RxCount))//��⵽OK 
              {
                if((DevInfo.GprsDebugLevel&DBG_INF))//
                {
                  printf("INF-GPRS:Check OK Success,nStatus=%d.\r\n",nStatus );
                } 
                nStatus++;
                nErrCount=0;
                nTimeTick=0;
              }
            }
          }					
        }
        break;
      }
      
    case 15://���͵�¼�������ݣ�GPRS��
      {
        if(nTimeTick==0)
        {
          //nHeartTick=OSTimeGet();//��������Tickʱ��
          nTimeTick=OSTimeGet();
          Clear_RxBuf();
          AFN02_F1_Login(CHANNEL_GPRS);
          
          if((DevInfo.GprsDebugLevel&DBG_INF))//
          {
            printf("\r\nINF-GPRS:Send Login Msg.nErrCount=%d,nStatus=%d\r\n",nErrCount,nStatus );
          } 
        }
        else
        {
          if((OSTimeGet()-nTimeTick)>=DevInfo.GprsCommTimeOut)
          {
            if(++nErrCount>=MAX_GPRS_ERR_COUNT)	//��������Դ���������GPRSģ��
            {
              if((DevInfo.GprsDebugLevel&DBG_ERR))//
              {
                printf("ERR_GPRS:nErrCount>=MAX_GPRS_ERR_COUNT,restart the Module,nStatus=%d,nRstCount=%d.\r\n",nStatus,nRstCount);
              }               
              //����״̬��
              nStatus=0;
              nErrCount=0;
              nTimeTick=0;  
              nRstCount++;//������������
            }
            else//�ط���¼ָ��
            {
              nTimeTick=0;
            }
          }
          else
          {
            if(GPRS_RxFlag==TRUE)//�յ�����
            {
              INT8U pBuffer[MAX_GPRS_RxBUF_SIZE];
              INT16U nSize;
              GPRS_RxFlag=FALSE;
              if(FetchGprsCmd(pBuffer,&nSize))//ȡ��һ�������ı���
              {
                INT8U retVal=0;
                if((DevInfo.GprsDebugLevel&DBG_INF))//
                {
                  printf("INF_GPRS:nFetchGprsCmd OK,nStatus=%d.\r\n",nStatus );
                }                  
                retVal=ProcessProtocol(pBuffer,nSize,CHANNEL_GPRS);
                if(retVal==TRUE)//Login OK
                {
                  if(DevInfo.GprsRunMode==0)//�����ϱ�ģʽ
                  {
                    nStatus=20;
                  }
                  else if(DevInfo.GprsRunMode==1)//��ѯӦ��ģʽ
                  {
                    nStatus++; //ת����һ״̬
                  }
                  
                  nErrCount=0;
                  nTimeTick=0;

                  Clear_RxBuf();
                  if((DevInfo.GprsDebugLevel&DBG_INF))//
                  {
                    printf("INF-GPRS:20%02d-%02d-%02d %02d:%02d:%02d============Login OK============,nErrCount=%d,nStatus=%d\r\n",
                           DevData.TimeInfo.year,DevData.TimeInfo.month,DevData.TimeInfo.day,DevData.TimeInfo.hour,DevData.TimeInfo.minute,DevData.TimeInfo.second,
                           nErrCount,nStatus);
                  }                   
                }
                else if(retVal==2)//����
                {
                  if((DevInfo.GprsDebugLevel&DBG_ERR))//
                  {
                    printf("ERR_GPRS:Echo dected,restart the Module,nStatus=%d,nRstCount=%d.\r\n",nStatus,nRstCount );
                  }
                  nStatus=0; //����gprsģ��
                  nErrCount=0;
                  nTimeTick=0;
                  nRstCount++;
                  
                  
                }
              }
              else
              {
                if((DevInfo.GprsDebugLevel&DBG_ERR))//
                {
                  printf("ERR_GPRS:FetchGprsCmd Error,nStatus=%d.\r\n",nStatus );
                }
              }
            }
          }					
        }
        break;	
      }
      
    case 16:	//�Ѿ���½�������������ѯӦ��ģʽ
      {
        if(nTimeTick==0)
        {
          AFN02_F3_Heart(CHANNEL_GPRS);
          nTimeTick=OSTimeGet();
          Clear_RxBuf();
          if((DevInfo.GprsDebugLevel&DBG_INF))//
          {
            printf("\r\nINF-GPRS:Send Heart Msg.nErrCount=%d,nStatus=%d\r\n",nErrCount,nStatus );
          } 
        }
        else
        {
          if((OSTimeGet()-nTimeTick)>=DevInfo.GprsHeartTimeOut)
          {
            if(++nErrCount>=MAX_GPRS_ERR_COUNT)	//��������Դ���������GPRSģ��
            {
              if((DevInfo.GprsDebugLevel&DBG_ERR))//
              {
                printf("ERR_GPRS:nErrCount>=MAX_GPRS_ERR_COUNT,restart the Module,nStatus=%d,nRstCount=%d.\r\n",nStatus,nRstCount);
              }               
              //����״̬��
              nStatus=0;
              nErrCount=0;
              nTimeTick=0;  
              nRstCount++;//������������
            }
            else//�ط�����ָ��
            {
              nTimeTick=0;
            }
          }
          else 
          {          
            if(GPRS_RxFlag==TRUE)//�յ�����
            {
              INT8U pBuffer[MAX_GPRS_RxBUF_SIZE];
              INT16U nSize;
              GPRS_RxFlag=FALSE;
              if(FetchGprsCmd(pBuffer,&nSize))//ȡ��һ�������ı���
              {
                INT8U retVal=0;
                if((DevInfo.GprsDebugLevel&DBG_INF))//
                {
                  printf("INF_GPRS:nFetchGprsCmd OK,nStatus=%d.\r\n",nStatus );
                }                  
                retVal=ProcessProtocol(pBuffer,nSize,CHANNEL_GPRS);
                if(retVal==TRUE)//Heart OK
                {
                  nErrCount=0;
                  nTimeTick=OSTimeGet();//��������ʱ��
                  Clear_RxBuf();

                  if((DevInfo.GprsDebugLevel&DBG_INF))//
                  {
                    printf("INF-GPRS:20%02d-%02d-%02d %02d:%02d:%02d============Heart OK============,nErrCount=%d,nStatus=%d\r\n",
                           DevData.TimeInfo.year,DevData.TimeInfo.month,DevData.TimeInfo.day,DevData.TimeInfo.hour,DevData.TimeInfo.minute,DevData.TimeInfo.second,
                           nErrCount,nStatus);
                  }                   
                }
                else if(retVal==2)//����
                { 
                  if((DevInfo.GprsDebugLevel&DBG_ERR))//
                  {
                    printf("ERR_GPRS:Echo dected,restart the Module,nStatus=%d,nRstCount=%d.\r\n",nStatus,nRstCount );
                  }
                  nStatus=0; //����gprsģ��
                  nErrCount=0;
                  nTimeTick=0;
                  nRstCount++;
                  
                 
                }
              }
              else 
              {
                if((DevInfo.GprsDebugLevel&DBG_ERR))//
                {
                  printf("ERR_GPRS:FetchGprsCmd Error,nStatus=%d.\r\n",nStatus );
                }
              }
            }
            
          }
        }
        break;        
      }   
      
    case 20:	//�Ѿ���½�����������������ϱ�ģʽ���ϱ���ǰ����
      {       
        if(nTimeTick==0)
        {

          AFN0F_F1_ReportRT(CHANNEL_GPRS);
//////          delay_MS(100);
//////          AFN02_F3_Heart(CHANNEL_GPRS);//for Test
          nTimeTick=OSTimeGet();
          Clear_RxBuf();
          if((DevInfo.GprsDebugLevel&DBG_INF))//
          {
            printf("\r\nINF-GPRS:Send CurrData.nErrCount=%d,nStatus=%d\r\n",nErrCount,nStatus );
          } 
        }
        else
        {
          if((OSTimeGet()-nTimeTick)>=DevInfo.GprsCommTimeOut)
          {
            if(++nErrCount>=MAX_GPRS_ERR_COUNT)	//��������Դ���������GPRSģ��
            {
              if((DevInfo.GprsDebugLevel&DBG_ERR))//
              {
                printf("ERR_GPRS:nErrCount>=MAX_GPRS_ERR_COUNT,restart the Module,nStatus=%d,nRstCount=%d.\r\n",nStatus,nRstCount);
              }               
              //����״̬��
              nStatus=0;
              nErrCount=0;
              nTimeTick=0;  
              nRstCount++;//������������
            }
            else//�ط�����ָ��
            {
              nTimeTick=0;
            }
          }
          else 
          {          
            if(GPRS_RxFlag==TRUE)//�յ�����
            {
              INT8U pBuffer[MAX_GPRS_RxBUF_SIZE];
              INT16U nSize;
              GPRS_RxFlag=FALSE;
              if(FetchGprsCmd(pBuffer,&nSize))//ȡ��һ�������ı���
              {
                INT8U retVal=0;
                if((DevInfo.GprsDebugLevel&DBG_INF))//
                {
                  printf("INF_GPRS:nFetchGprsCmd OK,nStatus=%d.\r\n",nStatus );
                }                  
                retVal=ProcessProtocol(pBuffer,nSize,CHANNEL_GPRS);
                if(retVal==TRUE)//Heart OK
                {
                   if((DevInfo.GprsDebugLevel&DBG_INF))//
                    {
                      printf("INF-GPRS:20%02d-%02d-%02d %02d:%02d:%02d Report CurrData OK,nErrCount=%d,nStatus=%d\r\n",
                             DevData.TimeInfo.year,DevData.TimeInfo.month,DevData.TimeInfo.day,DevData.TimeInfo.hour,DevData.TimeInfo.minute,DevData.TimeInfo.second,
                             nErrCount,nStatus);
                    }
     
                  nStatus++;//ת��������ʷ����
                  nErrCount=0;
                  nTimeTick=0;
                  Clear_RxBuf();
                }
                else if(retVal==2)//����
                {
                  if((DevInfo.GprsDebugLevel&DBG_ERR))//
                  {
                    printf("ERR_GPRS:Echo dected,restart the Module,nStatus=%d,nRstCount=%d.\r\n",nStatus,nRstCount );
                  }
                  nStatus=0; //����gprsģ��
                  nErrCount=0;
                  nTimeTick=0;
                  nRstCount++;                
                }
              }
              else 
              {
                if((DevInfo.GprsDebugLevel&DBG_ERR))//
                {
                  printf("ERR_GPRS:FetchGprsCmd Error,nStatus=%d.\r\n",nStatus );
                }
              }
            }
            
          }
        }
        break;        
      } 
      
    case 21:	//�Ѿ���½�����������������ϱ�ģʽ���ϱ���ʷ����
      {
        static INT8U HisRecordCount=0;
        static INT8U HisRecordPointer=0;
       
        if(nTimeTick==0)
        {
            //read eeprom
            HisRecordCount=0;
            HisRecordPointer=0;
            
            if(HisRecordCount==0)//��ʷ����Ϊ��
            {
              if((DevInfo.GprsDebugLevel&DBG_INF))//
              {
                printf("\r\nINF-GPRS:No HisData Exist,nStatus=%d\r\n",nStatus );
              } 
              nStatus=STATE_SUCCESS;//�ϱ��ɹ�
            }
            else
            {
              AFN0F_F2_ReportHIS(CHANNEL_GPRS,HisRecordCount,HisRecordPointer);
              nTimeTick=OSTimeGet();
              Clear_RxBuf();
              if((DevInfo.GprsDebugLevel&DBG_INF))//
              {
                printf("\r\nINF-GPRS:Send HisData.nErrCount=%d,nStatus=%d\r\n",nErrCount,nStatus );
              } 
            } 
        }
        else
        {
          if((OSTimeGet()-nTimeTick)>=DevInfo.GprsCommTimeOut)
          {
            if(++nErrCount>=MAX_GPRS_ERR_COUNT)	//��������Դ���������GPRSģ��
            {
              if((DevInfo.GprsDebugLevel&DBG_ERR))//
              {
                printf("ERR_GPRS:nErrCount>=MAX_GPRS_ERR_COUNT,restart the Module,nStatus=%d,nRstCount=%d.\r\n",nStatus,nRstCount);
              }               
              //����״̬��
              nStatus=0;
              nErrCount=0;
              nTimeTick=0;  
              nRstCount++;//������������
            }
            else//�ط�����ָ��
            {
              nTimeTick=0;
            }
          }
          else 
          {          
            if(GPRS_RxFlag==TRUE)//�յ�����
            {
              INT8U pBuffer[MAX_GPRS_RxBUF_SIZE];
              INT16U nSize;
              GPRS_RxFlag=FALSE;
              if(FetchGprsCmd(pBuffer,&nSize))//ȡ��һ�������ı���
              {
                INT8U retVal=0;
                if((DevInfo.GprsDebugLevel&DBG_INF))//
                {
                  printf("INF_GPRS:nFetchGprsCmd OK,nStatus=%d.\r\n",nStatus );
                }                  
                retVal=ProcessProtocol(pBuffer,nSize,CHANNEL_GPRS);
                if(retVal==TRUE)//Heart OK
                {
                  //�޸�eeprom��¼������ָ��                  
                  if((DevInfo.GprsDebugLevel&DBG_INF))//
                  {
                    printf("INF-GPRS:20%02d-%02d-%02d %02d:%02d:%02d Report HisData OK,nErrCount=%d,nStatus=%d\r\n",
                           DevData.TimeInfo.year,DevData.TimeInfo.month,DevData.TimeInfo.day,DevData.TimeInfo.hour,DevData.TimeInfo.minute,DevData.TimeInfo.second,
                           nErrCount,nStatus);
                  }         
                  nErrCount=0;
                  nTimeTick=0;//������һ����¼
                  Clear_RxBuf();
                }
                else if(retVal==2)//����
                {
                  if((DevInfo.GprsDebugLevel&DBG_ERR))//
                  {
                    printf("ERR_GPRS:Echo dected,restart the Module,nStatus=%d,nRstCount=%d.\r\n",nStatus,nRstCount);
                  }
                  nStatus=0; //����gprsģ��
                  nErrCount=0;
                  nTimeTick=0;
                  nRstCount++;                 
                }
              }
              else 
              {
                if((DevInfo.GprsDebugLevel&DBG_ERR))//
                {
                  printf("ERR_GPRS:FetchGprsCmd Error,nStatus=%d.\r\n",nStatus );
                }
              }
            }
            
          }
        }
        break;        
      }       
    default:
      break;
    }//end of switch
    
  }//end of     if(SysInfo.GprsTick>=GPRS_INTERVAL)
}


/*�ӻ������������Ƿ���OK�ķ���ֵ*/
//0D 0A 4F 4B 0D 0A
BOOL CheckOKAck(INT8U* pBuffer,INT8U Len)
{
  INT32U i=0;
  if(Len<=5) return FALSE;
  for(i=0;i<Len-5;i++)
  {
    if((pBuffer[i]=='\r')&&(pBuffer[i+1]=='\n')&&(pBuffer[i+2]=='O')&&(pBuffer[i+3]=='K')&&(pBuffer[i+4]=='\r')&&(pBuffer[i+5]=='\n'))
    {
      return TRUE;
    }
  }
  return FALSE;
}


/*��ȡIMEI���룬IMEI����Ϊ15λ��ͨ�ŵ�ַΪ16λ�����㲿�ֺ��油0*/
//0D 0A 4F 4B 0D 0A
BOOL GetIMEICode(INT8U* pDst,INT8U* pBuffer,INT8U Len)
{
  INT32U i=0;
  if(Len<32) return FALSE;
  for(i=0;i<Len-32;i++)
  {
    if((pBuffer[i]=='A')&&(pBuffer[i+1]=='T')&&(pBuffer[i+2]=='+')&&(pBuffer[i+3]=='C')&&(pBuffer[i+4]=='G')&&(pBuffer[i+5]=='S')&&(pBuffer[i+6]=='N')&&
       (pBuffer[i+7]=='\r')&&(pBuffer[i+8]=='\n')&&(pBuffer[i+9]=='\r')&&(pBuffer[i+10]=='\n')&&
       (pBuffer[i+26]=='\r')&&(pBuffer[i+27]=='\n')&&(pBuffer[i+28]=='\r')&&(pBuffer[i+29]=='\n')&&
       (pBuffer[i+30]=='O')&&(pBuffer[i+31]=='K'))
    {
      pDst[0]=((pBuffer[i+11]&0x0F)*16)+(pBuffer[i+12]&0x0F);
      pDst[1]=((pBuffer[i+13]&0x0F)*16)+(pBuffer[i+14]&0x0F);
      pDst[2]=((pBuffer[i+15]&0x0F)*16)+(pBuffer[i+16]&0x0F);
      pDst[3]=((pBuffer[i+17]&0x0F)*16)+(pBuffer[i+18]&0x0F);
      pDst[4]=((pBuffer[i+19]&0x0F)*16)+(pBuffer[i+20]&0x0F);
      pDst[5]=((pBuffer[i+21]&0x0F)*16)+(pBuffer[i+22]&0x0F);
      pDst[6]=((pBuffer[i+23]&0x0F)*16)+(pBuffer[i+24]&0x0F);
      pDst[7]=((pBuffer[i+25]&0x0F)*16);
      return TRUE;
    }
  }
  return FALSE;
}


/*�ӻ������������Ƿ���>�ķ���ֵ*/
//0D 0A 3E 
BOOL CheckSendAck(INT8U* pBuffer,INT8U Len)
{
  if(Len<3) return FALSE;
  for(INT16U i=0;i<=Len-3;i++)
  {
    printf("\r\npBuffer[%d]=%d\r\n",i,pBuffer[i] );
    if((pBuffer[i]==0x0D)&&(pBuffer[i+1]==0x0A)&&(pBuffer[i+2]==0x3E))
    {
      return TRUE;
    }
  }
  return FALSE;
}


/*�ӻ������������Ƿ���OK�ķ���ֵ*/
//0D 0A 53 45 4E 44 20 4F 4B 0D 0A
// 		S   E  N  E _  O  K
BOOL CheckSendOK(INT8U* pBuffer,INT8U Len)
{
  INT32U i=0;
  if(Len<11) return FALSE;
  for(i=0;i<Len-11;i++)
  {
    if((pBuffer[i]=='\r')&&(pBuffer[i+1]=='\n')&&(pBuffer[i+2]=='S')&&(pBuffer[i+3]=='E')&&(pBuffer[i+4]=='N')&&(pBuffer[i+5]=='D')
       &&(pBuffer[i+6]==' ')&&(pBuffer[i+7]=='O')&&(pBuffer[i+8]=='K')
         &&(pBuffer[i+9]=='\r')&&(pBuffer[i+10]=='\n'))
    {
      return TRUE;
    }
  }
  return FALSE;
}

/*�ӻ������������Ƿ���OK�ķ���ֵ*/
//CONNECT OK
BOOL CheckConnect(INT8U* pBuffer,INT8U Len)
{
  INT32U i=0;
  if(Len<=7) return FALSE;
  for(i=0;i<Len-7;i++)
  {
    if((pBuffer[i]=='C')&&(pBuffer[i+1]=='O')&&(pBuffer[i+2]=='N')&&(pBuffer[i+3]=='N')&&
       (pBuffer[i+4]=='E')&&(pBuffer[i+5]=='C')&&(pBuffer[i+6]=='T')&&(pBuffer[i+7]==' ')&&
         (pBuffer[i+8]=='O')&&(pBuffer[i+9]=='K'))
    {
      return TRUE;
    }
  }
  return FALSE;
}

//+CREG: 0,1
//0D 0A 2B 43 52 45 47 3A 20 30 2C 31 0D 0A
//\r \n +  C  R  E  G  :  _  0  ,  1  \r \n

BOOL CheckRegStatus(INT8U* pBuffer,INT8U Len)
{
  INT32U i=0;
  if(Len<=14) return FALSE;
  for(i=0;i<Len-14;i++)
  {
    if((pBuffer[i]=='\r')&&(pBuffer[i+1]=='\n')&&
       (pBuffer[i+2]=='+')&&(pBuffer[i+3]=='C')&&(pBuffer[i+4]=='R')&&(pBuffer[i+5]=='E')&&
         (pBuffer[i+6]=='G')&&(pBuffer[i+7]==':')&&
           (pBuffer[i+8]==' ')&&(pBuffer[i+9]=='0')&&(pBuffer[i+10]==',')&&
             (pBuffer[i+12]=='\r')&&(pBuffer[i+13]=='\n'))
    {
      if((pBuffer[i+11]=='1')||(pBuffer[i+11]=='5'))
      {
        return TRUE;
      }
      return FALSE;
    }
  }
  return FALSE;	
}

//+CSQ: 99, 99
//0D 0A 2B 43 52 45 47 3A 20 30 2C 31 0D 0A
//\r \n +  C  R  E  G  :  _  0  ,  1  \r \n
INT8U CheckCSQStatus(INT8U* pBuffer,INT8U Len)
{
  INT32U i=0;
  if(Len<=14) return FALSE;
  for(i=0;i<Len-14;i++)
  {
    if((pBuffer[i]=='\r')&&(pBuffer[i+1]=='\n')&&
       (pBuffer[i+2]=='+')&&(pBuffer[i+3]=='C')&&(pBuffer[i+4]=='S')&&(pBuffer[i+5]=='Q')&&
         (pBuffer[i+6]==':')&&(pBuffer[i+7]==' ')&&
           (pBuffer[i+10]==','))
    {
      
      return 10*(pBuffer[i+8]&0x0F)+(pBuffer[i+9]&0x0F);
    }
  }
  return 0;	
}


BOOL CheckStatus(INT8U* pBuffer,INT8U Len)
{
  INT8U nStatus=0;
  INT8U startPos=0,endPos=0;
  if(Len<12) return FALSE;
  while(1)
  {
    switch(nStatus)
    {
    case 0://����+CREG
      {
        INT16U i;
        for(i=startPos;i<Len-6;i++)
        {
          if((pBuffer[i]=='+')&&(pBuffer[i+1]=='C')&&(pBuffer[i+2]=='R')&&(pBuffer[i+3]=='E')&&(pBuffer[i+4]=='G')&&(pBuffer[i+5]==':'))
          {
            nStatus++;
            startPos=i;
            break;
          }
        }
        if(i==Len-6)//û���ҵ���ʶ��
        {
          return FALSE;
        }
      }
      break;
    case 1://����','
      {
        INT16U i;
        for(i=startPos;i<Len-1;i++)
        {
          if(pBuffer[i]==',')//�ҵ���
          {
            nStatus++;
            startPos=i;
          }
        }
        if(i==Len-1)
        {
          return FALSE;
        }	
      }
    case 2://����'0D 0A'
      {
        INT16U i;
        for(i=startPos;i<Len-2;i++)
        {
          if(pBuffer[i]==',')//�ҵ���
          {
            nStatus++;
            //startPos=i;
            endPos=i;
          }
        }
        if(i==Len-2)
        {
          return FALSE;
        }	
      }
    case 3://calc and judge
      {
        INT16U i,result;
        if(startPos>=endPos)
        {
          return FALSE;
        }
        for(i=startPos+1;i<endPos;i++)
        {
          result+=(pBuffer[i]&0x0F);
          //if()
        }
      }
      
    }
    
  }
  
}

BOOL Clear_RxBuf(void)
{
  for(int i=0;i<MAX_GPRS_RxBUF_SIZE;i++)
  {
    GPRS_RxBuf[i]=0;
  }
  GPRS_RxCount=0;
  
  return TRUE;
}

