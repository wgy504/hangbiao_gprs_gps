#include "app.h"

//Function declaration
INT16U Build_General_Response(INT8U _AFN,BOOL _PRMFlag,INT8U _SEQ,INT8U* pBuffer,INT16U nSize);

BOOL AFN04_F1_SetPara(INT8U *pSrc,INT16U *nSrcLen,INT8U *pDst,INT16U *nDstLen);				/*IP��ַ�Ͷ˿�*/
BOOL AFN04_F2_SetPara(INT8U *pSrc,INT16U *nSrcLen,INT8U *pDst,INT16U *nDstLen);				/*��������*/
BOOL AFN04_F3_SetPara(INT8U *pSrc,INT16U *nSrcLen,INT8U *pDst,INT16U *nDstLen);				/*�Զ������*/
BOOL AFN04_F4_SetPara(INT8U *pSrc,INT16U *nSrcLen,INT8U *pDst,INT16U *nDstLen);				/*�豸���ò���*/
BOOL AFN04_F5_SetPara(INT8U *pSrc,INT16U *nSrcLen,INT8U *pDst,INT16U *nDstLen);				/*�豸���ò���*/
BOOL AFN04_F6_SetPara(INT8U *pSrc,INT16U *nSrcLen,INT8U *pDst,INT16U *nDstLen);				/*�豸���ò���*/
BOOL AFN04_F7_SetPara(INT8U *pSrc,INT16U *nSrcLen,INT8U *pDst,INT16U *nDstLen);				/*�豸���ò���*/
BOOL AFN04_F8_SetPara(INT8U *pSrc,INT16U *nSrcLen,INT8U *pDst,INT16U *nDstLen);				/*�豸���ò���*/
BOOL AFN04_F9_SetPara(INT8U *pSrc,INT16U *nSrcLen,INT8U *pDst,INT16U *nDstLen);				/*�豸���ò���*/
BOOL AFN04_F10_SetPara(INT8U *pSrc,INT16U *nSrcLen,INT8U *pDst,INT16U *nDstLen);			/*�豸���ò���*/
BOOL AFN04_F11_SetPara(INT8U *pSrc,INT16U *nSrcLen,INT8U *pDst,INT16U *nDstLen);			/*�豸���ò���*/
BOOL AFN04_F12_SetPara(INT8U *pSrc,INT16U *nSrcLen,INT8U *pDst,INT16U *nDstLen);			/*�豸���ò���*/
BOOL AFN04_F20_SetPara(INT8U *pSrc,INT16U *nSrcLen,INT8U *pDst,INT16U *nDstLen);			/*�豸���ò���*/

BOOL AFN05_F1_DevRmtCtrl(INT8U *pSrc,INT16U *nSrcLen,INT8U *pDst,INT16U *nDstLen);			/*�豸ң�ز���*/
BOOL AFN05_F2_DevReset(INT8U *pSrc,INT16U *nSrcLen,INT8U *pDst,INT16U *nDstLen);			/*�豸��λ*/
BOOL AFN05_F3_DevSetRtc(INT8U *pSrc,INT16U *nSrcLen,INT8U *pDst,INT16U *nDstLen);			/*ʵʱʱ��*/

BOOL AFN09_F1_BasicInfo(INT8U *pSrc,INT16U *nSrcLen,INT8U *pDst,INT16U *nDstLen);			/*�豸������Ϣ*/

BOOL AFN0A_F1_GetPara(INT8U *pSrc,INT16U *nSrcLen,INT8U *pDst,INT16U *nDstLen);				/*IP��ַ�Ͷ˿�*/
BOOL AFN0A_F2_GetPara(INT8U *pSrc,INT16U *nSrcLen,INT8U *pDst,INT16U *nDstLen);				/*��������*/
BOOL AFN0A_F3_GetPara(INT8U *pSrc,INT16U *nSrcLen,INT8U *pDst,INT16U *nDstLen);				/*�Զ������*/
BOOL AFN0A_F4_GetPara(INT8U *pSrc,INT16U *nSrcLen,INT8U *pDst,INT16U *nDstLen);				/*�豸���ò���*/
BOOL AFN0A_F5_GetPara(INT8U *pSrc,INT16U *nSrcLen,INT8U *pDst,INT16U *nDstLen);				/*�豸���ò���*/
BOOL AFN0A_F6_GetPara(INT8U *pSrc,INT16U *nSrcLen,INT8U *pDst,INT16U *nDstLen);				/*�豸���ò���*/
BOOL AFN0A_F7_GetPara(INT8U *pSrc,INT16U *nSrcLen,INT8U *pDst,INT16U *nDstLen);				/*�豸���ò���*/
BOOL AFN0A_F8_GetPara(INT8U *pSrc,INT16U *nSrcLen,INT8U *pDst,INT16U *nDstLen);				/*�豸���ò���*/
BOOL AFN0A_F9_GetPara(INT8U *pSrc,INT16U *nSrcLen,INT8U *pDst,INT16U *nDstLen);				/*�豸���ò���*/
BOOL AFN0A_F10_GetPara(INT8U *pSrc,INT16U *nSrcLen,INT8U *pDst,INT16U *nDstLen);			/*�豸���ò���*/
BOOL AFN0A_F11_GetPara(INT8U *pSrc,INT16U *nSrcLen,INT8U *pDst,INT16U *nDstLen);			/*�豸���ò���*/
BOOL AFN0A_F12_GetPara(INT8U *pSrc,INT16U *nSrcLen,INT8U *pDst,INT16U *nDstLen);			/*�豸���ò���*/
BOOL AFN0A_F20_GetPara(INT8U *pSrc,INT16U *nSrcLen,INT8U *pDst,INT16U *nDstLen);			/*�豸���ò���*/

BOOL AFN0C_F1_DevStatus(INT8U *pSrc,INT16U *nSrcLen,INT8U *pDst,INT16U *nDstLen);			/*�豸����״̬*/
BOOL AFN0C_F2_BattStatus(INT8U *pSrc,INT16U *nSrcLen,INT8U *pDst,INT16U *nDstLen);			/*�豸����״̬*/
BOOL AFN0C_F3_RtcStatus(INT8U *pSrc,INT16U *nSrcLen,INT8U *pDst,INT16U *nDstLen);			/*�豸����״̬*/

BOOL Process_AFN02_Case(INT8U* pBuffer,INT16U nSize,INT8U channel);			/*AFN02�������*/
BOOL Process_AFN04_Case(INT8U* pBuffer,INT16U nSize,INT8U channel);			/*AFN04�������*/
BOOL Process_AFN05_Case(INT8U* pBuffer,INT16U nSize,INT8U channel);			/*AFN05�������*/
BOOL Process_AFN09_Case(INT8U* pBuffer,INT16U nSize,INT8U channel);			/*AFN09�������*/
BOOL Process_AFN0A_Case(INT8U* pBuffer,INT16U nSize,INT8U channel);			/*AFN0A�������*/
BOOL Process_AFN0C_Case(INT8U* pBuffer,INT16U nSize,INT8U channel);			/*AFN0C�������*/

INT8U CalcCRC8(INT8U *pBuffer, INT32U nSize)
{
  INT8U ret = 0;
  INT16U i=0;
  for (i=0; i<nSize; ++i)
  {
    ret += (INT8U)pBuffer[i];
  }
  
  return ret;
}

INT16U GetCRC16(INT8U *data,int len) 
{ 
  INT16U ax,lsb; 
  int i,j; 
  ax=0xFFFF; 
  for(i=0;i<len;i++) 
  { 
    ax ^= data[i]; 
    for(j=0;j<8;j++) 
    { 
      lsb=ax&0x0001; 
      ax = ax>>1; 
      if(lsb!=0) 
        ax ^= 0xA001; 
    } 
  } 
  return ax; 
}




/*****************************************************************
����Э�� |68 [LL] [LL] 68 [ADDR] [AFN] [SEQ] [DA] [DT] [CRC] 16
����ֵ��
0������
1����ȷ
******************************************************************/
INT8U ProcessProtocol(INT8U* pBuffer,INT16U nSize,INT8U channel)
{
  INT16U nLenth=nSize;
  
  pBuffer+=6;//ָ��ָ��ADDR�ֶ�
  nLenth-=6;
  if((pBuffer[0]!=DevInfo.CommAddr[0])||
     (pBuffer[1]!=DevInfo.CommAddr[1])||
       (pBuffer[2]!=DevInfo.CommAddr[2])||
         (pBuffer[3]!=DevInfo.CommAddr[3])||
           (pBuffer[4]!=DevInfo.CommAddr[4])||
             (pBuffer[5]!=DevInfo.CommAddr[5])||
               (pBuffer[6]!=DevInfo.CommAddr[6])||
                 (pBuffer[7]!=DevInfo.CommAddr[7]))
  {
    printf( "\r\nProcessProtocol:Address Match Error.\r\n");
    return FALSE;//���ص�ַ����
  }
  
  pBuffer+=8;//ָ��ָ��AFN�ֶ�
  nLenth-=8;//ȥ����ַ
  nLenth-=3;//ȥ��crc ��16
  return SvrToCtu(pBuffer,nLenth,channel);
  
}
/*****************************************************************
����Svr�·��ı��� |68 [LL] [LL] 68 [ADDR] [AFN] [SEQ] [DA] [DT] [CRC] 16
����ֵ��
0������
1����ȷ
2��GPRS����
3����ַ����
����������
******************************************************************/
INT8U SvrToCtu(INT8U* pBuffer,INT16U nSize,INT8U channel)
{
  INT8U 	AFN=0;		/*������*/
  INT8U	SEQ=0;		/*֡����*/
  INT16U nLenth=nSize;
  
  AFN=*pBuffer++;
  SEQ=*pBuffer++;
  
  printf( "\r\nSvrToCtu:nLenth=%d,AFN=%x,SEQ=%x\r\n",nLenth,AFN,SEQ);
  if((AFN&DIR_UP_FLAG)==DIR_UP_FLAG)//GPRS����,��Ҫ��������ģ�� ,������1��������0
  {
    printf( "\r\nSvrToCtu:GPRS Echo Occurred.nLenth=%d,AFN=%x,SEQ=%x\r\n",nLenth,AFN,SEQ);			
    return 2;		
  }
  
  CommStatus.AFN_R=AFN;//������յ���AFN
  CommStatus.SEQ_R=SEQ;//������յ���SEQ
  nLenth-=2;
  
  if((DevData.IsDevLogin==FALSE)&&(channel==CHANNEL_GPRS))//ֻ��GPRSͨ���Ž��е�¼�ж�
  {
    if(AFN!=AFN_LINK_DECT)
    {
      printf( "\r\nSvrToCtu:Received CMD without Login.\r\n");	
      return FALSE;
    }
  }	
  switch(AFN&0x1F)
  {
  case AFN_LINK_DECT://��·�ӿڼ��
    {
      return Process_AFN02_Case(pBuffer,nLenth,channel);
    }
  case AFN_PARA_SET: //��������
    {			
      return Process_AFN04_Case(pBuffer,nLenth,channel);
    }
  case AFN_CMD_CTRL: //����ָ��
    {
      return Process_AFN05_Case(pBuffer,nLenth,channel);
    }
  case AFN_RQST_CFG: //������Ϣ
    {
      return Process_AFN09_Case(pBuffer,nLenth,channel);
    }		
  case AFN_INQURY_PARA: //������ѯ
    {
      return Process_AFN0A_Case(pBuffer,nLenth,channel);	
    }
    
  case AFN_RQST_RT_DATA: //����ʵʱ����
    {
      return Process_AFN0C_Case(pBuffer,nLenth,channel);	
    }
  case AFN_RQST_HS_DATA: //������ʷ����
    {
      //return Process_AFN0D_Case(pBuffer,nLenth,channel);	
    }		
  case AFN_RQST_EVNT_DATA: //�����¼�����
    {
      //return Process_AFN0E_Case(pBuffer,nLenth,channel);	
    }		
  default:
    return FALSE;
  }
}

BOOL Process_AFN02_Case(INT8U* pBuffer,INT16U nSize,INT8U channel)
{
  INT16U 	DA=0;		/*DA*/
  INT16U 	DT=0;		/*DT*/
  INT16U nLenth=nSize;
  BOOL retFlag=TRUE;
  do
  {
    DA=*pBuffer++;
    DA<<=8;
    DA+=*pBuffer++;
    
    DT=*pBuffer++;
    DT<<=8;
    DT+=*pBuffer++;
    nLenth-=4;	
    
    switch(DT)
    {
    case AFN02_F1_DT://��¼ȷ�ϰ�
      {
        INT8U nStatus=*pBuffer++;
        nLenth-=1;
        DevData.IsDevLogin=(nStatus==AFNXX_CONFIRM)?TRUE:FALSE;//��¼�ɹ�
        retFlag=DevData.IsDevLogin;
        break;
      }
    case AFN02_F3_DT://����ȷ�ϰ�
      {
        INT8U nStatus=*pBuffer++;
        nLenth-=1;
        retFlag=(nStatus==AFNXX_CONFIRM)?TRUE:FALSE;
        break;
      }
    default:
      return FALSE;
    }
  }while(nLenth);
  return retFlag;
}

BOOL Process_AFN04_Case(INT8U* pBuffer,INT16U nSize,INT8U channel)
{
  INT16U 	DA=0;		/*DA*/
  INT16U 	DT=0;		/*DT*/
  INT16U nLenth=nSize;
  BOOL retFlag=TRUE;
  INT8U pDst[100];	/*�������ݵĻ�����*/
  INT16U nDstLen=0;
  do
  {
    memcpy(&pDst[nDstLen],pBuffer,4);
    nDstLen+=4;
    
    DA=*pBuffer++;
    DA<<=8;
    DA+=*pBuffer++;
    
    DT=*pBuffer++;
    DT<<=8;
    DT+=*pBuffer++;
    nLenth-=4;
    switch(DT)
    {
    case AFN04_F1_DT://IP��ַ�Ͷ˿�	
      {
        retFlag=AFN04_F1_SetPara(pBuffer,&nLenth,pDst,&nDstLen);
        printf( "\r\nAFN04_F1_SetPara: isOKFlag=%d\r\n",retFlag);
        break;		
      }
    case AFN04_F2_DT://��������
      {
        retFlag=AFN04_F2_SetPara(pBuffer,&nLenth,pDst,&nDstLen);
        printf( "\r\nAFN04_F2_SetPara: isOKFlag=%d\r\n",retFlag);
        break;
      }
    case AFN04_F3_DT://�Զ������
      {
        retFlag=AFN04_F3_SetPara(pBuffer,&nLenth,pDst,&nDstLen);
        printf( "\r\nAFN04_F3_SetPara: isOKFlag=%d\r\n",retFlag);
        break;
      }
    case AFN04_F4_DT://�豸���ò���
      {
        retFlag=AFN04_F4_SetPara(pBuffer,&nLenth,pDst,&nDstLen);
        printf( "\r\nAFN04_F4_SetPara: isOKFlag=%d\r\n",retFlag);
        break;
      }
    case AFN04_F5_DT://�豸���ò���
      {
        retFlag=AFN04_F5_SetPara(pBuffer,&nLenth,pDst,&nDstLen);
        printf( "\r\nAFN04_F5_SetPara: isOKFlag=%d\r\n",retFlag);
        break;
      }
    case AFN04_F6_DT://�豸���ò���
      {
        retFlag=AFN04_F6_SetPara(pBuffer,&nLenth,pDst,&nDstLen);
        printf( "\r\nAFN04_F6_SetPara: isOKFlag=%d\r\n",retFlag);
        break;
      }
    case AFN04_F7_DT://�豸���ò���
      {
        retFlag=AFN04_F7_SetPara(pBuffer,&nLenth,pDst,&nDstLen);
        printf( "\r\nAFN04_F7_SetPara: isOKFlag=%d\r\n",retFlag);
        break;
      }
    case AFN04_F8_DT://�豸���ò���
      {
        retFlag=AFN04_F8_SetPara(pBuffer,&nLenth,pDst,&nDstLen);
        printf( "\r\nAFN04_F8_SetPara: isOKFlag=%d\r\n",retFlag);
        break;
      }
    case AFN04_F9_DT://�豸���ò���
      {
        retFlag=AFN04_F9_SetPara(pBuffer,&nLenth,pDst,&nDstLen);
        printf( "\r\nAFN04_F9_SetPara: isOKFlag=%d\r\n",retFlag);
        break;
      }
    case AFN04_F10_DT://�豸���ò���
      {
        retFlag=AFN04_F10_SetPara(pBuffer,&nLenth,pDst,&nDstLen);
        printf( "\r\nAFN04_F10_SetPara: isOKFlag=%d\r\n",retFlag);
        break;
      }
      
    case AFN04_F11_DT://�豸���ò���
      {
        retFlag=AFN04_F11_SetPara(pBuffer,&nLenth,pDst,&nDstLen);
        printf( "\r\nAFN04_F11_SetPara: isOKFlag=%d\r\n",retFlag);
        break;
      }	
      
    case AFN04_F12_DT://�豸���ò���-ϵͳ����ģʽ�������ϱ����߲�ѯӦ��
      {
        retFlag=AFN04_F12_SetPara(pBuffer,&nLenth,pDst,&nDstLen);
        printf( "\r\nAFN04_F12_SetPara: isOKFlag=%d\r\n",retFlag);
        break;
      }
      
    case AFN04_F20_DT://�豸���ò���
      {
        retFlag=AFN04_F20_SetPara(pBuffer,&nLenth,pDst,&nDstLen);
        printf( "\r\nAFN04_F20_SetPara: isOKFlag=%d\r\n",retFlag);
        break;
      }			
    default:
      return FALSE;
    }	
    
  }while(nLenth);	
  
  nDstLen=Build_General_Response(CommStatus.AFN_R,FALSE,CommStatus.SEQ_R, pDst ,nDstLen);	
  return CtuToSrv(pDst,nDstLen,channel);	
}

BOOL Process_AFN05_Case(INT8U* pBuffer,INT16U nSize,INT8U channel)
{
  INT16U 	DA=0;		/*DA*/
  INT16U 	DT=0;		/*DT*/
  INT16U nLenth=nSize;
  BOOL retFlag=TRUE;
  INT8U pDst[100];	/*�������ݵĻ�����*/
  INT16U nDstLen=0;
  do
  {
    memcpy(&pDst[nDstLen],pBuffer,4);
    nDstLen+=4;
    
    DA=*pBuffer++;
    DA<<=8;
    DA+=*pBuffer++;
    
    DT=*pBuffer++;
    DT<<=8;
    DT+=*pBuffer++;
    nLenth-=4;
    
    switch(DT)
    {
    case AFN05_F1_DT://�豸ң�ز���	
      {
        retFlag=AFN05_F1_DevRmtCtrl(pBuffer,&nLenth,pDst,&nDstLen);
        printf( "\r\nAFN05_F1_DevRmtCtrl: isOKFlag=%d\r\n",retFlag);
        break;	
      }
    case AFN05_F2_DT://�豸��λ
      {
        retFlag=AFN05_F2_DevReset(pBuffer,&nLenth,pDst,&nDstLen);
        printf( "\r\nAFN05_F2_DevReset: isOKFlag=%d\r\n",retFlag);
        break;	
      }
    case AFN05_F3_DT://�豸��λ
      {
        retFlag=AFN05_F3_DevSetRtc(pBuffer,&nLenth,pDst,&nDstLen);
        printf( "\r\nAFN05_F3_DevSetRtc: isOKFlag=%d\r\n",retFlag);
        break;					
      }
    default:
      return FALSE;
    }	
    
  }while(nLenth);	
  
  nDstLen=Build_General_Response(CommStatus.AFN_R,FALSE,CommStatus.SEQ_R, pDst ,nDstLen);	
  return CtuToSrv(pDst,nDstLen,channel);	
}

BOOL Process_AFN09_Case(INT8U* pBuffer,INT16U nSize,INT8U channel)
{
  INT16U 	DA=0;		/*DA*/
  INT16U 	DT=0;		/*DT*/
  INT16U nLenth=nSize;
  BOOL retFlag=TRUE;
  INT8U pDst[100];	/*�������ݵĻ�����*/
  INT16U nDstLen=0;
  do
  {
    memcpy(&pDst[nDstLen],pBuffer,4);
    nDstLen+=4;
    
    DA=*pBuffer++;
    DA<<=8;
    DA+=*pBuffer++;
    
    DT=*pBuffer++;
    DT<<=8;
    DT+=*pBuffer++;
    nLenth-=4;
    
    switch(DT)
    {
    case AFN09_F1_DT://�ն˻���������Ϣ
      {		
        retFlag=AFN09_F1_BasicInfo(pBuffer,&nLenth,pDst,&nDstLen);
        printf( "\r\nAFN09_F1_BasicInfo: isOKFlag=%d\r\n",retFlag);
        break;
      }
    default:
      return FALSE;		
    }	
    
  }while(nLenth);
  
  nDstLen=Build_General_Response(CommStatus.AFN_R,FALSE,CommStatus.SEQ_R, pDst ,nDstLen);	
  return CtuToSrv(pDst,nDstLen,channel);	
}
BOOL Process_AFN0A_Case(INT8U* pBuffer,INT16U nSize,INT8U channel)
{
  INT16U 	DA=0;		/*DA*/
  INT16U 	DT=0;		/*DT*/
  INT16U nLenth=nSize;
  BOOL retFlag=TRUE;
  INT8U pDst[200];	/*�������ݵĻ�����*/
  INT16U nDstLen=0;
  do
  {
    memcpy(&pDst[nDstLen],pBuffer,4);
    nDstLen+=4;
    
    DA=*pBuffer++;
    DA<<=8;
    DA+=*pBuffer++;
    
    DT=*pBuffer++;
    DT<<=8;
    DT+=*pBuffer++;
    nLenth-=4;
    
    switch(DT)
    {		
    case AFN0A_F1_DT://IP��ַ�Ͷ˿�	
      {
        retFlag= AFN0A_F1_GetPara(pBuffer,&nLenth,pDst,&nDstLen);
        printf( "\r\nAFN0A_F1_GetPara: isOKFlag=%d\r\n",retFlag);
        break;
      }
    case AFN0A_F2_DT://��������
      {
        retFlag= AFN0A_F2_GetPara(pBuffer,&nLenth,pDst,&nDstLen);
        printf( "\r\nAFN0A_F2_GetPara: isOKFlag=%d\r\n",retFlag);
        break;
      }
    case AFN0A_F3_DT://�Զ������
      {
        retFlag= AFN0A_F3_GetPara(pBuffer,&nLenth,pDst,&nDstLen);
        printf( "\r\nAFN0A_F3_GetPara: isOKFlag=%d\r\n",retFlag);
        break;
      }
    case AFN0A_F4_DT://�豸���ò���
      {
        retFlag= AFN0A_F4_GetPara(pBuffer,&nLenth,pDst,&nDstLen);
        printf( "\r\nAFN0A_F4_GetPara: isOKFlag=%d\r\n",retFlag);
        break;
      }		
    case AFN0A_F5_DT://�豸���ò���
      {
        retFlag= AFN0A_F5_GetPara(pBuffer,&nLenth,pDst,&nDstLen);
        printf( "\r\nAFN0A_F5_GetPara: isOKFlag=%d\r\n",retFlag);
        break;
      }	
    case AFN0A_F6_DT://�豸���ò���
      {
        retFlag= AFN0A_F6_GetPara(pBuffer,&nLenth,pDst,&nDstLen);
        printf( "\r\nAFN0A_F6_GetPara: isOKFlag=%d\r\n",retFlag);
        break;
      }	
    case AFN0A_F7_DT://�豸���ò���
      {
        retFlag= AFN0A_F7_GetPara(pBuffer,&nLenth,pDst,&nDstLen);
        printf( "\r\nAFN0A_F7_GetPara: isOKFlag=%d\r\n",retFlag);
        break;
      }	
    case AFN0A_F8_DT://�豸���ò���
      {
        retFlag= AFN0A_F8_GetPara(pBuffer,&nLenth,pDst,&nDstLen);
        printf( "\r\nAFN0A_F8_GetPara: isOKFlag=%d\r\n",retFlag);
        break;
      }	
    case AFN0A_F9_DT://�豸���ò���
      {
        retFlag= AFN0A_F9_GetPara(pBuffer,&nLenth,pDst,&nDstLen);
        printf( "\r\nAFN0A_F9_GetPara: isOKFlag=%d\r\n",retFlag);
        break;
      }	
    case AFN0A_F10_DT://�豸���ò���
      {
        retFlag= AFN0A_F10_GetPara(pBuffer,&nLenth,pDst,&nDstLen);
        printf( "\r\nAFN0A_F10_GetPara: isOKFlag=%d\r\n",retFlag);
        break;
      }	
    case AFN0A_F11_DT://�豸���ò���
      {
        retFlag= AFN0A_F11_GetPara(pBuffer,&nLenth,pDst,&nDstLen);
        printf( "\r\nAFN0A_F11_GetPara: isOKFlag=%d\r\n",retFlag);
        break;
      }		
    case AFN0A_F12_DT://�豸���ò���-ϵͳ����ģʽ
      {
        retFlag= AFN0A_F12_GetPara(pBuffer,&nLenth,pDst,&nDstLen);
        printf( "\r\nAFN0A_F11_GetPara: isOKFlag=%d\r\n",retFlag);
        break;
      }			
    case AFN0A_F20_DT://�豸���ò���-all
      {
        retFlag= AFN0A_F20_GetPara(pBuffer,&nLenth,pDst,&nDstLen);
        printf( "\r\nAFN0A_F20_GetPara: isOKFlag=%d\r\n",retFlag);
        break;
      }	
      
    default:
      return FALSE;		
    }	
    
  }while(nLenth);
  
  nDstLen=Build_General_Response(CommStatus.AFN_R,FALSE,CommStatus.SEQ_R, pDst ,nDstLen);	
  return CtuToSrv(pDst,nDstLen,channel);	
}

BOOL Process_AFN0C_Case(INT8U* pBuffer,INT16U nSize,INT8U channel)
{
  INT16U 	DA=0;		/*DA*/
  INT16U 	DT=0;		/*DT*/
  INT16U nLenth=nSize;
  BOOL retFlag=TRUE;
  INT8U pDst[200];	/*�������ݵĻ�����*/
  INT16U nDstLen=0;
  do
  {
    memcpy(&pDst[nDstLen],pBuffer,4);
    nDstLen+=4;
    
    DA=*pBuffer++;
    DA<<=8;
    DA+=*pBuffer++;
    
    DT=*pBuffer++;
    DT<<=8;
    DT+=*pBuffer++;
    nLenth-=4;
    
    switch(DT)
    {
      
    case AFN0C_F1_DT://�豸����״̬
      {
        retFlag= AFN0C_F1_DevStatus(pBuffer,&nLenth,pDst,&nDstLen);
        printf( "\r\nAFN0C_F1_DevStatus: isOKFlag=%d\r\n",retFlag);
        break;
      }
    case AFN0C_F2_DT://��ع���״̬
      {
        retFlag= AFN0C_F2_BattStatus(pBuffer,&nLenth,pDst,&nDstLen);
        printf( "\r\nAFN0C_F2_BattStatus: isOKFlag=%d\r\n",retFlag);
        break;
      }				
    case AFN0C_F3_DT://ʵʱʱ��
      {
        retFlag= AFN0C_F3_RtcStatus(pBuffer,&nLenth,pDst,&nDstLen);
        printf( "\r\nAFN0C_F3_RtcStatus: isOKFlag=%d\r\n",retFlag);
        break;
      }			
    default:
      return FALSE;		
    }	
    
  }while(nLenth);
  
  nDstLen=Build_General_Response(CommStatus.AFN_R,FALSE,CommStatus.SEQ_R, pDst ,nDstLen);	
  return CtuToSrv(pDst,nDstLen,channel);	
}


INT16U Build_General_Response(INT8U _AFN,BOOL _PRMFlag,INT8U _SEQ,INT8U* pBuffer,INT16U nSize)
{
  INT16U nDstLen=0,i=0;
  //memcpy(&pBuffer[2],&pBuffer[0],nSize);
  for(i=0;i<nSize;i++)
  {
    pBuffer[nSize-i+2-1]=pBuffer[nSize-i-1];
  }
  pBuffer[0]=_AFN;
  pBuffer[0]|=(DIR_UP_FLAG);				//����=1,����=0
  if(_PRMFlag)							//����=1���ظ�=0
  {
    pBuffer[0]|=(PRM_MASTER_FLAG);				
  }
  else
  {
    pBuffer[0]&=~(PRM_MASTER_FLAG);				
  }
  
  if(DevData.ErrCode>0)
  {
    pBuffer[0]|=(ACD_FLAG);					//�����ʾ
  }
  pBuffer[1]=_SEQ&0x0F;
  pBuffer[1]|=(FIR_FLAG);
  pBuffer[1]|=(FIN_FLAG);
  
  nDstLen=2+nSize;
  return nDstLen;
  
}


BOOL AFN02_F1_Login(INT8U ch)
{
  INT8U nDstLen;
  INT8U pDst[10]={0};
  pDst[0]=(INT8U)(AFNXX_FX_DA>>8);
  pDst[1]=(INT8U)(AFNXX_FX_DA);
  pDst[2]=(INT8U)(AFN02_F1_DT>>8);
  pDst[3]=(INT8U)(AFN02_F1_DT);
  nDstLen=4;
  
  nDstLen=Build_General_Response(AFN_LINK_DECT,TRUE,0x00, pDst ,nDstLen);	
  return CtuToSrv(pDst,nDstLen,ch);
}

BOOL AFN02_F3_Heart(INT8U ch)
{
  INT8U nDstLen;
  INT8U pDst[10]={0};
  pDst[0]=(INT8U)(AFNXX_FX_DA>>8);
  pDst[1]=(INT8U)(AFNXX_FX_DA);
  pDst[2]=(INT8U)(AFN02_F3_DT>>8);
  pDst[3]=(INT8U)(AFN02_F3_DT);
  nDstLen=4;
  
  nDstLen=Build_General_Response(AFN_LINK_DECT,TRUE,0x00, pDst ,nDstLen);	
  return CtuToSrv(pDst,nDstLen,ch);
}

/*pBuffer [IP][Port][APN]*/
BOOL AFN04_F1_SetPara(INT8U *pSrc,INT16U *nSrcLen,INT8U *pDst,INT16U *nDstLen)		/*IP��ַ�Ͷ˿�*/
{
  BOOL isOKFlag=TRUE;
  
  if(!Para_Save(ADDR_SVR_IP, pSrc, DATALEN_SVR_IP))/*д��IP��ַ*/
  {
    isOKFlag=FALSE;
    printf( "\r\nAFN04_F1_SetPara1: isOKFlag=%d\r\n",isOKFlag);
  }
  memcpy(DevInfo.SvrIP,pSrc,DATALEN_SVR_IP);
  pSrc+=DATALEN_SVR_IP;
  *nSrcLen-=DATALEN_SVR_IP;
  
  if(!Para_Save(ADDR_SVR_PORT, pSrc, DATALEN_SVR_PORT))/*д��˿ں�*/
  {
    isOKFlag=FALSE;
    printf( "\r\nAFN04_F1_SetPara2: isOKFlag=%d\r\n",isOKFlag);
  }
  DevInfo.SvrPort=pSrc[1];
  DevInfo.SvrPort<<=8;
  DevInfo.SvrPort+=pSrc[0];
  
  pSrc+=DATALEN_SVR_PORT;
  *nSrcLen-=DATALEN_SVR_PORT;
  
  if(!Para_Save(ADDR_SVR_APN, pSrc, DATALEN_SVR_APN))/*д��APN*/
  {
    isOKFlag=FALSE;
    printf( "\r\nAFN04_F1_SetPara3: isOKFlag=%d\r\n",isOKFlag);
  }
  memcpy(DevInfo.SvrAPN,pSrc,DATALEN_SVR_APN);
  pSrc+=DATALEN_SVR_APN;
  *nSrcLen-=DATALEN_SVR_APN;
  
  pDst[*nDstLen]=(isOKFlag==TRUE)?(AFNXX_CONFIRM):(AFNXX_IGNORE);//��ȡ��������
  *nDstLen+=1;
  return isOKFlag;
}
/*
�豸���ͣ�BYTE	2
ͨ�ŵ�ַ��BYTE	4
��װ���ȣ�BCD	5
��װγ�ȣ�BCD	5
*/
BOOL AFN04_F2_SetPara(INT8U *pSrc,INT16U *nSrcLen,INT8U *pDst,INT16U *nDstLen)		/*��������*/
{
  BOOL isOKFlag=TRUE;
  
  if(!Para_Save(ADDR_DEV_TYPE, pSrc, DATALEN_DEV_TYPE))/*д���豸����*/
  {
    isOKFlag= FALSE;
  }
  DevInfo.DevType=pSrc[1];
  DevInfo.DevType<<=8;
  DevInfo.DevType+=pSrc[0];	
  pSrc+=DATALEN_DEV_TYPE;
  *nSrcLen-=DATALEN_DEV_TYPE;
  
  if(!Para_Save(ADDR_COMMADDR, pSrc, DATALEN_COMMADDR))/*д��ͨ�ŵ�ַ*/
  {
    isOKFlag= FALSE;
  }	
  memcpy(DevInfo.CommAddr,pSrc,DATALEN_COMMADDR);
  pSrc+=DATALEN_COMMADDR;
  *nSrcLen-=DATALEN_COMMADDR;
  
  if(!Para_Save(ADDR_LOC_LONG, pSrc, DATALEN_LOC_LONG))/*д�밲װ����*/
  {
    isOKFlag= FALSE;
  }	
  
  DevInfo.SitePos.Longitude=pSrc[3];
  DevInfo.SitePos.Longitude<<=8;
  DevInfo.SitePos.Longitude+=pSrc[2];
  DevInfo.SitePos.Longitude<<=8;
  DevInfo.SitePos.Longitude+=pSrc[1];
  DevInfo.SitePos.Longitude<<=8;
  DevInfo.SitePos.Longitude+=pSrc[0];
  
  //memcpy(DevInfo.SitePos.Longitude,pSrc,DATALEN_LOC_LONG);
  pSrc+=DATALEN_LOC_LONG;
  *nSrcLen-=DATALEN_LOC_LONG;
  
  
  
  if(!Para_Save(ADDR_LOC_LAT, pSrc, DATALEN_LOC_LAT))/*д�밲װγ��*/
  {
    isOKFlag= FALSE;
  }
  
  DevInfo.SitePos.Latitude=pSrc[3];
  DevInfo.SitePos.Latitude<<=8;
  DevInfo.SitePos.Latitude+=pSrc[2];
  DevInfo.SitePos.Latitude<<=8;
  DevInfo.SitePos.Latitude+=pSrc[1];
  DevInfo.SitePos.Latitude<<=8;
  DevInfo.SitePos.Latitude+=pSrc[0]; 
  //memcpy(DevInfo.SitePos.Latitude,pSrc,DATALEN_LOC_LAT);
  pSrc+=DATALEN_LOC_LAT;
  *nSrcLen-=DATALEN_LOC_LAT;
  
  pDst[*nDstLen]=(isOKFlag==TRUE)?(AFNXX_CONFIRM):(AFNXX_IGNORE);//��ȡ��������
  *nDstLen+=1;
  
  return isOKFlag;
}

BOOL AFN04_F3_SetPara(INT8U *pSrc,INT16U *nSrcLen,INT8U *pDst,INT16U *nDstLen)		/*�Զ������*/
{
  INT8U i;
  BOOL isOKFlag=TRUE;
  
  INT8U RhythmCount=*pSrc++;//�������õĸ���
  *nDstLen-=1;
  if(RhythmCount>MAX_RHYTHM_COUNT)
  {
    return FALSE;
  }
  for(i=0;i<RhythmCount;i++)
  {
    INT8U RhythmNo=*pSrc++;
    INT8U RhythmPeriodCount=pSrc[0];
    INT8U nDataLen=1+4*RhythmPeriodCount;
    if(RhythmNo>=10) continue;//0~9,���ʱ��
    if(RhythmPeriodCount>=10) continue;//0~9,ÿ�����������ʮ������
    //д��EEPRom����������TTTT ���ʺ�ֻ��Ϊ��������ַ����д��EEPROM
    if(!Para_Save(ADDR_USER_RHYTHM+RhythmNo*(DATALEN_USER_RHYTHM)*2+2 , pSrc, nDataLen))
    {
      isOKFlag= FALSE;
    }	
    pSrc+=nDataLen;
    *nDstLen-=(1+nDataLen);
    
  }
  
  pDst[*nDstLen]=(isOKFlag==TRUE)?(AFNXX_CONFIRM):(AFNXX_IGNORE);//��ȡ��������
  *nDstLen+=1;	
  return isOKFlag;
}
BOOL AFN04_F4_SetPara(INT8U *pSrc,INT16U *nSrcLen,INT8U *pDst,INT16U *nDstLen)		/*�豸���ò���*/
{
  BOOL isOKFlag=TRUE;
  
  //����Դ
  if(!Para_Save(ADDR_RHYTHM_SRC, pSrc, DATALEN_RHYTHM_SRC))
  {
    isOKFlag= FALSE;
  }
  DevInfo.RhythmSrc=*pSrc;
  pSrc+=DATALEN_RHYTHM_SRC;
  *nSrcLen-=DATALEN_RHYTHM_SRC;
  
  pDst[*nDstLen]=(isOKFlag==TRUE)?(AFNXX_CONFIRM):(AFNXX_IGNORE);//��ȡ��������
  *nDstLen+=1;
  return isOKFlag;
}

BOOL AFN04_F5_SetPara(INT8U *pSrc,INT16U *nSrcLen,INT8U *pDst,INT16U *nDstLen)		/*�豸���ò���*/
{
  BOOL isOKFlag=TRUE;
  
  //��������
  if(!Para_Save(ADDR_CELLS_COUNT, pSrc, DATALEN_CELLS_COUNT))
  {
    isOKFlag= FALSE;
  }	
  DevInfo.BattCount=*pSrc;
  pSrc+=DATALEN_CELLS_COUNT;
  *nSrcLen-=DATALEN_CELLS_COUNT;
  
  pDst[*nDstLen]=(isOKFlag==TRUE)?(AFNXX_CONFIRM):(AFNXX_IGNORE);//��ȡ��������
  *nDstLen+=1;
  return isOKFlag;
}

BOOL AFN04_F6_SetPara(INT8U *pSrc,INT16U *nSrcLen,INT8U *pDst,INT16U *nDstLen)		/*�豸���ò���*/
{
  BOOL isOKFlag=TRUE;
  
  //�������
  if(!Para_Save(ADDR_CELLS_SOC, pSrc, DATALEN_CELLS_SOC))
  {
    isOKFlag= FALSE;
  }
  DevInfo.BattSoc=pSrc[1];
  DevInfo.BattSoc<<=8;
  DevInfo.BattSoc+=pSrc[0];
  
  pSrc+=DATALEN_CELLS_SOC;	
  *nSrcLen-=DATALEN_CELLS_SOC;
  
  pDst[*nDstLen]=(isOKFlag==TRUE)?(AFNXX_CONFIRM):(AFNXX_IGNORE);//��ȡ��������
  *nDstLen+=1;
  return isOKFlag;
}

BOOL AFN04_F7_SetPara(INT8U *pSrc,INT16U *nSrcLen,INT8U *pDst,INT16U *nDstLen)		/*�豸���ò���*/
{
  BOOL isOKFlag=TRUE;
  
  //����ѹ
  if(!Para_Save(ADDR_CHG_VOLT, pSrc, DATALEN_CHG_VOLT))
  {
    isOKFlag= FALSE;
  }	
  DevInfo.MaxChgVolt=pSrc[1];
  DevInfo.MaxChgVolt<<=8;
  DevInfo.MaxChgVolt+=pSrc[0];
  
  pSrc+=DATALEN_CHG_VOLT;
  *nSrcLen-=DATALEN_CHG_VOLT;	
  
  //�ŵ��ѹ
  if(!Para_Save(ADDR_DISCHG_VOLT, pSrc, DATALEN_DISCHG_VOLT))
  {
    isOKFlag= FALSE;
  }	
  DevInfo.MinDischgVolt=pSrc[1];
  DevInfo.MinDischgVolt<<=8;
  DevInfo.MinDischgVolt+=pSrc[0];
  
  pSrc+=DATALEN_DISCHG_VOLT;
  *nSrcLen-=DATALEN_DISCHG_VOLT;	
  
  pDst[*nDstLen]=(isOKFlag==TRUE)?(AFNXX_CONFIRM):(AFNXX_IGNORE);//��ȡ��������
  *nDstLen+=1;
  return isOKFlag;
}


BOOL AFN04_F8_SetPara(INT8U *pSrc,INT16U *nSrcLen,INT8U *pDst,INT16U *nDstLen)		/*�豸���ò���*/
{
  BOOL isOKFlag=TRUE;
  
  //���ȵȼ�
  if(!Para_Save(ADDR_BRIGHTNESS, pSrc, DATALEN_BRIGHTNESS))
  {
    isOKFlag= FALSE;
  }
  DevInfo.BrightLevel=*pSrc;
  pSrc+=DATALEN_BRIGHTNESS;
  *nSrcLen-=DATALEN_BRIGHTNESS;		
  
  pDst[*nDstLen]=(isOKFlag==TRUE)?(AFNXX_CONFIRM):(AFNXX_IGNORE);//��ȡ��������
  *nDstLen+=1;
  return isOKFlag;
}

BOOL AFN04_F9_SetPara(INT8U *pSrc,INT16U *nSrcLen,INT8U *pDst,INT16U *nDstLen)		/*�豸���ò���*/
{
  BOOL isOKFlag=TRUE;
  
  //ǿ��ʹ��
  if(!Para_Save(ADDR_FORCE_CTRL_EN, pSrc, DATALEN_FORCE_CTRL_EN))
  {
    isOKFlag= FALSE;
  }	
  DevInfo.ForceRunEnFlag=*pSrc;
  pSrc+=DATALEN_FORCE_CTRL_EN;	
  *nSrcLen-=DATALEN_FORCE_CTRL_EN;	
  
  pDst[*nDstLen]=(isOKFlag==TRUE)?(AFNXX_CONFIRM):(AFNXX_IGNORE);//��ȡ��������
  *nDstLen+=1;
  return isOKFlag;
}


BOOL AFN04_F10_SetPara(INT8U *pSrc,INT16U *nSrcLen,INT8U *pDst,INT16U *nDstLen)		/*�豸���ò���*/
{
  BOOL isOKFlag=TRUE;
  
  //ͬ��ʹ��
  if(!Para_Save(ADDR_SYNC_RUN_EN, pSrc, DATALEN_SYNC_RUN_EN))
  {
    isOKFlag= FALSE;
  }	
  DevInfo.SyncRunEnFlag=*pSrc;
  pSrc+=DATALEN_SYNC_RUN_EN;
  *nSrcLen-=DATALEN_SYNC_RUN_EN;	
  
  pDst[*nDstLen]=(isOKFlag==TRUE)?(AFNXX_CONFIRM):(AFNXX_IGNORE);//��ȡ��������
  *nDstLen+=1;
  return isOKFlag;
}

BOOL AFN04_F11_SetPara(INT8U *pSrc,INT16U *nSrcLen,INT8U *pDst,INT16U *nDstLen)		/*�豸���ò���*/
{
  BOOL isOKFlag=TRUE;
  
  //���ص���ֵ������
  if(!Para_Save(ADDR_SENSE_VAL_H, pSrc, (DATALEN_SENSE_VAL_H+DATALEN_SENSE_VAL+DATALEN_SENSE_VAL_L)))
  {
    isOKFlag= FALSE;
  }	
  DevInfo.DstLuxValH=pSrc[1];
  DevInfo.DstLuxValH<<=8;
  DevInfo.DstLuxValH+=pSrc[0];
  pSrc+=DATALEN_SENSE_VAL_H;
  *nSrcLen-=DATALEN_SENSE_VAL_H;	
  
  
  DevInfo.DstLuxVal=pSrc[1];
  DevInfo.DstLuxVal<<=8;
  DevInfo.DstLuxVal+=pSrc[0];
  pSrc+=DATALEN_SENSE_VAL;
  *nSrcLen-=DATALEN_SENSE_VAL;	
  
  DevInfo.DstLuxValL=pSrc[1];
  DevInfo.DstLuxValL<<=8;
  DevInfo.DstLuxValL+=pSrc[0];
  pSrc+=DATALEN_SENSE_VAL_L;
  *nSrcLen-=DATALEN_SENSE_VAL_L;	
  
  pDst[*nDstLen]=(isOKFlag==TRUE)?(AFNXX_CONFIRM):(AFNXX_IGNORE);//��ȡ��������
  *nDstLen+=1;
  return isOKFlag;
}

BOOL AFN04_F12_SetPara(INT8U *pSrc,INT16U *nSrcLen,INT8U *pDst,INT16U *nDstLen)		/*�豸���ò���*/
{
  BOOL isOKFlag=TRUE;
  
  //GPRS����ģʽ��0-�����ϱ���1-��ѯӦ��
  if(!Para_Save(ADDR_GPRS_RUN_MODE, pSrc, DATALEN_GPRS_RUN_MODE))
  {
    isOKFlag= FALSE;
  }	
  DevInfo.GprsRunMode=*pSrc;
  pSrc+=DATALEN_GPRS_RUN_MODE;
  *nSrcLen-=DATALEN_GPRS_RUN_MODE;	
  
  pDst[*nDstLen]=(isOKFlag==TRUE)?(AFNXX_CONFIRM):(AFNXX_IGNORE);//��ȡ��������
  *nDstLen+=1;
  return isOKFlag;
}
BOOL AFN04_F20_SetPara(INT8U *pSrc,INT16U *nSrcLen,INT8U *pDst,INT16U *nDstLen)		/*�豸���ò���*/
{
  BOOL isOKFlag=TRUE;
  
  //����Դ
  if(!Para_Save(ADDR_RHYTHM_SRC, pSrc, DATALEN_RHYTHM_SRC))
  {
    isOKFlag= FALSE;
  }
  DevInfo.RhythmSrc=*pSrc++;	
  *nSrcLen-=DATALEN_RHYTHM_SRC;
  
  //��������
  if(!Para_Save(ADDR_CELLS_COUNT, pSrc, DATALEN_CELLS_COUNT))
  {
    isOKFlag= FALSE;
  }	
  DevInfo.BattCount=*pSrc++;
  *nSrcLen-=DATALEN_CELLS_COUNT;
  
  //�������
  if(!Para_Save(ADDR_CELLS_SOC, pSrc, DATALEN_CELLS_SOC))
  {
    isOKFlag= FALSE;
  }
  DevInfo.BattSoc=pSrc[1];
  DevInfo.BattSoc<<=8;
  DevInfo.BattSoc+=pSrc[0];
  pSrc+=DATALEN_CELLS_SOC;	
  *nSrcLen-=DATALEN_CELLS_SOC;
  
  //����ѹ
  if(!Para_Save(ADDR_CHG_VOLT, pSrc, DATALEN_CHG_VOLT))
  {
    isOKFlag= FALSE;
  }	
  DevInfo.MaxChgVolt=pSrc[1];
  DevInfo.MaxChgVolt<<=8;
  DevInfo.MaxChgVolt+=pSrc[0];	
  pSrc+=DATALEN_CHG_VOLT;
  *nSrcLen-=DATALEN_CHG_VOLT;	
  
  //�ŵ��ѹ
  if(!Para_Save(ADDR_DISCHG_VOLT, pSrc, DATALEN_DISCHG_VOLT))
  {
    isOKFlag= FALSE;
  }	
  DevInfo.MinDischgVolt=pSrc[1];
  DevInfo.MinDischgVolt<<=8;
  DevInfo.MinDischgVolt+=pSrc[0];	
  pSrc+=DATALEN_DISCHG_VOLT;
  *nSrcLen-=DATALEN_DISCHG_VOLT;	
  
  //���ȵȼ�
  if(!Para_Save(ADDR_BRIGHTNESS, pSrc, DATALEN_BRIGHTNESS))
  {
    isOKFlag= FALSE;
  }	
  DevInfo.BrightLevel=*pSrc;	
  pSrc+=DATALEN_BRIGHTNESS;
  *nSrcLen-=DATALEN_BRIGHTNESS;	
  
  //ǿ��ʹ��
  if(!Para_Save(ADDR_FORCE_CTRL_EN, pSrc, DATALEN_FORCE_CTRL_EN))
  {
    isOKFlag= FALSE;
  }	
  DevInfo.ForceRunEnFlag=*pSrc;
  pSrc+=DATALEN_FORCE_CTRL_EN;	
  *nSrcLen-=DATALEN_FORCE_CTRL_EN;	
  
  //ͬ��ʹ��
  if(!Para_Save(ADDR_SYNC_RUN_EN, pSrc, DATALEN_SYNC_RUN_EN))
  {
    isOKFlag= FALSE;
  }
  DevInfo.SyncRunEnFlag=*pSrc;
  pSrc+=DATALEN_SYNC_RUN_EN;
  *nSrcLen-=DATALEN_SYNC_RUN_EN;	
  
  pDst[*nDstLen]=(isOKFlag==TRUE)?(AFNXX_CONFIRM):(AFNXX_IGNORE);//��ȡ��������
  *nDstLen+=1;
  return isOKFlag;
}

BOOL AFN05_F1_DevRmtCtrl(INT8U *pSrc,INT16U *nSrcLen,INT8U *pDst,INT16U *nDstLen)	  /*�豸ң�ز���*/
{
  BOOL isOKFlag=TRUE;
  //Add Code here.
  INT8U CtrlVal=*pSrc++;
  *nSrcLen-=1;
  
  pDst[*nDstLen]=(isOKFlag==TRUE)?(AFNXX_CONFIRM):(AFNXX_IGNORE);//��ȡ��������
  *nDstLen+=1;
  return isOKFlag;
}


BOOL AFN05_F2_DevReset(INT8U *pSrc,INT16U *nSrcLen,INT8U *pDst,INT16U *nDstLen)	  /*�豸��λ*/
{
  BOOL isOKFlag=TRUE;
  //Add Code here.
  INT8U CtrlVal=*pSrc++;
  *nSrcLen-=1;
  
  *pDst++=(isOKFlag==TRUE)?(AFNXX_CONFIRM):(AFNXX_IGNORE);//��ȡ��������
  *nDstLen+=1;
  return isOKFlag;
}

BOOL AFN05_F3_DevSetRtc(INT8U *pSrc,INT16U *nSrcLen,INT8U *pDst,INT16U *nDstLen)	  /*ʵʱʱ��*/
{
  BOOL isOKFlag=TRUE;
  
  INT8U nSecond=*pSrc++;
  INT8U nMinute=*pSrc++;
  INT8U nHour=*pSrc++;
  INT8U nDay=*pSrc++;
  INT8U nMonth=*pSrc++;
  INT8U nYear=*pSrc++;
  *nSrcLen-=6;
  
  DevData.TimeInfo.year=((nYear>>4)&0x0F)*10+(nYear&0x0F)+2000;
  DevData.TimeInfo.month=((nMonth>>4)&0x0F)*10+(nMonth&0x0F);
  DevData.TimeInfo.day=((nDay>>4)&0x0F)*10+(nDay&0x0F);
  DevData.TimeInfo.hour=((nHour>>4)&0x0F)*10+(nHour&0x0F);
  DevData.TimeInfo.minute=((nMinute>>4)&0x0F)*10+(nMinute&0x0F);
  DevData.TimeInfo.second=((nSecond>>4)&0x0F)*10+(nSecond&0x0F);
  
  pDst[*nDstLen]=(isOKFlag==TRUE)?(AFNXX_CONFIRM):(AFNXX_IGNORE);//��ȡ��������
  *nDstLen+=1;
  return isOKFlag;
}


BOOL AFN09_F1_BasicInfo(INT8U *pSrc,INT16U *nSrcLen,INT8U *pDst,INT16U *nDstLen)	/*��ѯ�豸������Ϣ*/
{
  BOOL isOKFlag=TRUE;
  
  memcpy(&pDst[*nDstLen],DevBasicInfo.OEMCode,4);
  *nDstLen+=4;
  
  memcpy(&pDst[*nDstLen],DevInfo.CommAddr,8);
  *nDstLen+=8;
  
  memcpy(&pDst[*nDstLen],DevBasicInfo.SWVersion,2);
  *nDstLen+=2;
  
  pDst[*nDstLen]=DevBasicInfo.PTType;
  *nDstLen+=1;
  
  memcpy(&pDst[*nDstLen],DevBasicInfo.PTVersion,2);
  *nDstLen+=2;
  
  memcpy(&pDst[*nDstLen],DevBasicInfo.HWVersion,2);
  *nDstLen+=2;	
  return isOKFlag;
}

BOOL AFN0A_F1_GetPara(INT8U *pSrc,INT16U *nSrcLen,INT8U *pDst,INT16U *nDstLen)		/*IP��ַ�Ͷ˿�*/
{
  BOOL isOKFlag=FALSE;
  
  isOKFlag=Para_Read(ADDR_SVR_IP, &pDst[*nDstLen], DATALEN_SVR_IP);
  *nDstLen+=DATALEN_SVR_IP;
  
  isOKFlag=Para_Read(ADDR_SVR_PORT, &pDst[*nDstLen], DATALEN_SVR_PORT);
  *nDstLen+=DATALEN_SVR_PORT;
  
  isOKFlag=Para_Read(ADDR_SVR_APN, &pDst[*nDstLen], DATALEN_SVR_APN);
  
  printf( "\r\nAFN0A_F1_GetPara: isOKFlag=%d\r\n",isOKFlag);
  *nDstLen+=DATALEN_SVR_APN;	
  
  return isOKFlag;
}
BOOL AFN0A_F2_GetPara(INT8U *pSrc,INT16U *nSrcLen,INT8U *pDst,INT16U *nDstLen)		/*��������*/
{
  BOOL isOKFlag=TRUE;
  
  Para_Read(ADDR_DEV_TYPE, &pDst[*nDstLen], DATALEN_DEV_TYPE);
  *nDstLen+=DATALEN_DEV_TYPE;
  Para_Read(ADDR_COMMADDR, &pDst[*nDstLen], DATALEN_COMMADDR);
  *nDstLen+=DATALEN_COMMADDR;
  Para_Read(ADDR_LOC_LONG, &pDst[*nDstLen], DATALEN_LOC_LONG);
  *nDstLen+=DATALEN_LOC_LONG;	
  Para_Read(ADDR_LOC_LAT, &pDst[*nDstLen], DATALEN_LOC_LAT);
  *nDstLen+=DATALEN_LOC_LAT;		
  
  return isOKFlag;
}
/*
������������ʸ�������ʼ���ʺ�
����������
���ʸ��� ��һ�����ʺţ���һ���������ڣ���һ�������������ݡ�����
EEPROM���洢���ʺš����ص��Ƕ�����ʵ����Ч�ĵ���
*/
BOOL AFN0A_F3_GetPara(INT8U *pSrc,INT16U *nSrcLen,INT8U *pDst,INT16U *nDstLen)		/*�Զ������*/
{
  INT8U i,index=0;
  BOOL isOKFlag=TRUE;
  INT8U RhythmCount=*pSrc++;//���ʸ���
  INT8U RhythmStartNo=*pSrc++;//������ʼ��
  INT8U tmpIndex=0;
  
  printf( "\r\nAFN0A_F3_GetPara: RhythmCount=%d,RhythmStartNo=%d\r\n",RhythmCount,RhythmStartNo);	
  
  if((RhythmCount>MAX_RHYTHM_COUNT))
  {
    RhythmCount=MAX_RHYTHM_COUNT;
  }
  
  tmpIndex=*nDstLen;//��¼������ʼ��λ��
  pDst[*nDstLen]=0;
  *nDstLen+=1;//
  if(RhythmStartNo>=MAX_RHYTHM_COUNT)
  {
    return FALSE;
  }
  
  //PeriodCount P1 P1 P1 P1 ......
  for(i=0;i<RhythmCount;i++)
  {
    INT8U RhythmNo=RhythmStartNo+i;
    INT8U nDataLen=0;
    INT8U nPeriodCount=0;
    
    Para_Read(ADDR_USER_RHYTHM+RhythmNo*(DATALEN_USER_RHYTHM)*2+2, &pDst[*nDstLen], 1);//�����õ��ʵ�������
    nPeriodCount=pDst[*nDstLen];
    printf( "\r\nAFN0A_F3_GetPara: nPeriodCount=%d\r\n",nPeriodCount);				
    
    if((nPeriodCount>0)&&(nPeriodCount<10))//������Ϊ1~9,����Ч�ĵ���
    {
      index++;//��Ч�ĵ��ʸ���
      
      pDst[*nDstLen]=RhythmNo;//���ʺ�
      *nDstLen+=1;
      
      nDataLen=1+4*nPeriodCount;
      Para_Read(ADDR_USER_RHYTHM+RhythmNo*(DATALEN_USER_RHYTHM)*2+2, &pDst[*nDstLen], nDataLen);//��������
      *nDstLen+=nDataLen;
    }
    else
    {
      continue;
    }
  }
  pDst[tmpIndex]=index;
  return isOKFlag;
}

BOOL AFN0A_F4_GetPara(INT8U *pSrc,INT16U *nSrcLen,INT8U *pDst,INT16U *nDstLen)		/*����Դ*/
{
  BOOL isOKFlag=TRUE;
  
  Para_Read(ADDR_RHYTHM_SRC, &pDst[*nDstLen], DATALEN_RHYTHM_SRC);
  *nDstLen+=DATALEN_RHYTHM_SRC;
  
  return isOKFlag;
}

BOOL AFN0A_F5_GetPara(INT8U *pSrc,INT16U *nSrcLen,INT8U *pDst,INT16U *nDstLen)		/*��ѯ-��������*/
{
  BOOL isOKFlag=TRUE;
  
  Para_Read(ADDR_CELLS_COUNT, &pDst[*nDstLen], DATALEN_CELLS_COUNT);
  *nDstLen+=DATALEN_CELLS_COUNT;
  
  return isOKFlag;
}

BOOL AFN0A_F6_GetPara(INT8U *pSrc,INT16U *nSrcLen,INT8U *pDst,INT16U *nDstLen)		/*��ѯ-�������*/
{
  BOOL isOKFlag=TRUE;
  
  Para_Read(ADDR_CELLS_SOC, &pDst[*nDstLen], DATALEN_CELLS_SOC);
  *nDstLen+=DATALEN_CELLS_SOC;	
  
  return isOKFlag;
}

BOOL AFN0A_F7_GetPara(INT8U *pSrc,INT16U *nSrcLen,INT8U *pDst,INT16U *nDstLen)		/*��ѯ-��ŵ��ѹ*/
{
  BOOL isOKFlag=TRUE;
  
  Para_Read(ADDR_CHG_VOLT, &pDst[*nDstLen], DATALEN_CHG_VOLT);
  *nDstLen+=DATALEN_CHG_VOLT;		
  Para_Read(ADDR_DISCHG_VOLT, &pDst[*nDstLen], DATALEN_DISCHG_VOLT);
  *nDstLen+=DATALEN_DISCHG_VOLT;
  
  return isOKFlag;
}

BOOL AFN0A_F8_GetPara(INT8U *pSrc,INT16U *nSrcLen,INT8U *pDst,INT16U *nDstLen)		/*��ѯ-���ȵȼ�*/
{
  BOOL isOKFlag=TRUE;
  
  Para_Read(ADDR_BRIGHTNESS, &pDst[*nDstLen], DATALEN_BRIGHTNESS);
  *nDstLen+=DATALEN_BRIGHTNESS;
  
  return isOKFlag;
}

BOOL AFN0A_F9_GetPara(INT8U *pSrc,INT16U *nSrcLen,INT8U *pDst,INT16U *nDstLen)		/*��ѯ-ǿ��ʹ��*/
{
  BOOL isOKFlag=TRUE;
  
  Para_Read(ADDR_FORCE_CTRL_EN, &pDst[*nDstLen], DATALEN_FORCE_CTRL_EN);
  *nDstLen+=DATALEN_FORCE_CTRL_EN;	
  
  return isOKFlag;
}

BOOL AFN0A_F10_GetPara(INT8U *pSrc,INT16U *nSrcLen,INT8U *pDst,INT16U *nDstLen)		/*��ѯ-ͬ��ʱ��*/
{
  BOOL isOKFlag=TRUE;
  
  Para_Read(ADDR_SYNC_RUN_EN, &pDst[*nDstLen], DATALEN_SYNC_RUN_EN);
  *nDstLen+=DATALEN_SYNC_RUN_EN;		 
  return isOKFlag;
}

BOOL AFN0A_F11_GetPara(INT8U *pSrc,INT16U *nSrcLen,INT8U *pDst,INT16U *nDstLen)		/*��ѯ-������ֵ���ݲ�*/
{
  BOOL isOKFlag=TRUE;
  
  Para_Read(ADDR_SENSE_VAL_H, &pDst[*nDstLen], DATALEN_SENSE_VAL_L);
  *nDstLen+=DATALEN_SENSE_VAL_H+DATALEN_SENSE_VAL+DATALEN_SENSE_VAL_L;		
  
  return isOKFlag;
}

BOOL AFN0A_F12_GetPara(INT8U *pSrc,INT16U *nSrcLen,INT8U *pDst,INT16U *nDstLen)		/*��ѯ-ϵͳ����ģʽ*/
{
  BOOL isOKFlag=TRUE;
  
  Para_Read(ADDR_GPRS_RUN_MODE, &pDst[*nDstLen], DATALEN_GPRS_RUN_MODE);
  *nDstLen+=DATALEN_GPRS_RUN_MODE;		
  
  return isOKFlag;
}

BOOL AFN0A_F20_GetPara(INT8U *pSrc,INT16U *nSrcLen,INT8U *pDst,INT16U *nDstLen)		/*��ѯ-�������ò���*/
{
  BOOL isOKFlag=TRUE;
  
  Para_Read(ADDR_RHYTHM_SRC, &pDst[*nDstLen], DATALEN_RHYTHM_SRC);
  *nDstLen+=DATALEN_RHYTHM_SRC;
  
  Para_Read(ADDR_CELLS_COUNT, &pDst[*nDstLen], DATALEN_CELLS_COUNT);
  *nDstLen+=DATALEN_CELLS_COUNT;
  
  Para_Read(ADDR_CELLS_SOC, &pDst[*nDstLen], DATALEN_CELLS_SOC);
  *nDstLen+=DATALEN_CELLS_SOC;	
  
  Para_Read(ADDR_CHG_VOLT, &pDst[*nDstLen], DATALEN_CHG_VOLT);
  *nDstLen+=DATALEN_CHG_VOLT;		
  
  Para_Read(ADDR_DISCHG_VOLT, &pDst[*nDstLen], DATALEN_DISCHG_VOLT);
  *nDstLen+=DATALEN_DISCHG_VOLT;
  
  Para_Read(ADDR_BRIGHTNESS, &pDst[*nDstLen], DATALEN_BRIGHTNESS);
  *nDstLen+=DATALEN_BRIGHTNESS;
  
  Para_Read(ADDR_FORCE_CTRL_EN, &pDst[*nDstLen], DATALEN_FORCE_CTRL_EN);
  *nDstLen+=DATALEN_FORCE_CTRL_EN;
  
  Para_Read(ADDR_SYNC_RUN_EN, &pDst[*nDstLen], DATALEN_SYNC_RUN_EN);
  *nDstLen+=DATALEN_SYNC_RUN_EN;		
  
  return isOKFlag;
}



BOOL AFN0C_F1_DevStatus(INT8U *pSrc,INT16U *nSrcLen,INT8U *pDst,INT16U *nDstLen)   /*�豸����״̬*/
{
  /*
  ��ǰ����״̬	BIN	1
  ��ǰ�ź�ǿ��	BIN	1
  ��ǰ��λ��Ч��־	BS8	1
  ��ǰGPS����	BCD	5
  ��ǰGPSγ��	BCD	5
  ��ǰLBS����	BCD	5
  ��ǰLBSγ��	BCD	5
  ��վ����	BIN	1
  ��1����վMCC	BIN	2
  ��1����վMNC	BIN	2
  ��1����վLAC	BIN	2
  ��1����վCELL ID	BIN	2
  ��1����վBSIC	BIN	2
  ��1����վARFCN	BIN	2
  ��1����վRSSI	BIN	1
  ...
  */
  INT8U i=0;
  INT8U isOKFlag=TRUE;
  
  pDst[*nDstLen]=DevData.ErrCode;				//������Ϣ
  *nDstLen+=1;
  
  pDst[*nDstLen]=DevData.CSQ;					//CSQ
  *nDstLen+=1;
  
  pDst[*nDstLen]=DevData.GpsInfo.ValidFlag;			//��λ��Ч��־
  *nDstLen+=1;
  
  pDst[*nDstLen]=(INT8U)(DevData.GpsInfo.SiteInfo.Longitude);
  *nDstLen+=1;  
  pDst[*nDstLen]=(INT8U)(DevData.GpsInfo.SiteInfo.Longitude>>8);
  *nDstLen+=1;
  pDst[*nDstLen]=(INT8U)(DevData.GpsInfo.SiteInfo.Longitude>>16);
  *nDstLen+=1;
  pDst[*nDstLen]=(INT8U)(DevData.GpsInfo.SiteInfo.Longitude>>24);
  *nDstLen+=1;
  
  pDst[*nDstLen]=(INT8U)(DevData.GpsInfo.SiteInfo.Latitude);
  *nDstLen+=1;  
  pDst[*nDstLen]=(INT8U)(DevData.GpsInfo.SiteInfo.Latitude>>8);
  *nDstLen+=1;
  pDst[*nDstLen]=(INT8U)(DevData.GpsInfo.SiteInfo.Latitude>>16);
  *nDstLen+=1;
  pDst[*nDstLen]=(INT8U)(DevData.GpsInfo.SiteInfo.Latitude>>24);
  *nDstLen+=1;  
  //memcpy(&pDst[*nDstLen],DevData.GpsInfo.SiteInfo.Longitude,DATALEN_LOC_LONG);//����
  //*nDstLen+=DATALEN_LOC_LONG;
  
  //memcpy(&pDst[*nDstLen],DevData.GpsInfo.SiteInfo.Latitude,DATALEN_LOC_LAT);//γ��
  //*nDstLen+=DATALEN_LOC_LAT;
  
  //memcpy(&pDst[*nDstLen],DevData.LbsInfo.Longitude,DATALEN_LOC_LONG);	//����
  //*nDstLen+=DATALEN_LOC_LONG;
  
  //memcpy(&pDst[*nDstLen],DevData.LbsInfo.Latitude,DATALEN_LOC_LAT);	//γ��
  //*nDstLen+=DATALEN_LOC_LAT;
  
  pDst[*nDstLen]=MAX_BASE_COUNT;						//��վ����
  *nDstLen+=1;
  
  for(i=0;i<MAX_BASE_COUNT;i++)
  {
    pDst[*nDstLen+13*i+0]=(INT8U)(DevData.BaseInfo[i].MCC);
    pDst[*nDstLen+13*i+1]=(INT8U)(DevData.BaseInfo[i].MCC>>8);
    
    pDst[*nDstLen+13*i+2]=(INT8U)(DevData.BaseInfo[i].MNC);
    pDst[*nDstLen+13*i+3]=(INT8U)(DevData.BaseInfo[i].MNC>>8);
    
    pDst[*nDstLen+13*i+4]=(INT8U)(DevData.BaseInfo[i].LAC);
    pDst[*nDstLen+13*i+5]=(INT8U)(DevData.BaseInfo[i].LAC>>8);
    
    pDst[*nDstLen+13*i+6]=(INT8U)(DevData.BaseInfo[i].CELLID);
    pDst[*nDstLen+13*i+7]=(INT8U)(DevData.BaseInfo[i].CELLID>>8);
    
    pDst[*nDstLen+13*i+8]=(INT8U)(DevData.BaseInfo[i].BSIC);
    pDst[*nDstLen+13*i+9]=(INT8U)(DevData.BaseInfo[i].BSIC>>8);
    
    pDst[*nDstLen+13*i+10]=(INT8U)(DevData.BaseInfo[i].ARFCN);
    pDst[*nDstLen+13*i+11]=(INT8U)(DevData.BaseInfo[i].ARFCN>>8);
    
    pDst[*nDstLen+13*i+12]=(INT8U)(DevData.BaseInfo[i].RSSI);
    
  }
  
  *nDstLen+=(13*MAX_BASE_COUNT);	 //121
  return isOKFlag;
}

BOOL AFN0C_F2_BattStatus(INT8U *pSrc,INT16U *nSrcLen,INT8U *pDst,INT16U *nDstLen)   /*�豸���״̬*/
{
  /*
  ��������	BIN	1
  
  �����1���	BIN	1
  �����1��ǰ��ѹ	BIN	2
  �����1��ǰ״̬	BIN	1
  ...		
  �����n���	BIN	1
  �����n��ǰ��ѹ	BIN	2
  �����n��ǰ״̬	BIN	1
  */
  INT16U i=0;
  INT8U isOKFlag=TRUE;
  INT8U nBattCount=0;
  
  pDst[*nDstLen]=(DevInfo.BattCount>MAX_BATT_COUNT)?MAX_BATT_COUNT:DevInfo.BattCount;	//��ظ���
  nBattCount=pDst[*nDstLen];
  *nDstLen+=1;
  
  
  for(i=0;i<nBattCount;i++)
  {
    pDst[*nDstLen+3*i+0]=(INT8U)(DevData.BattInfo[i].Status);
    pDst[*nDstLen+3*i+1]=(INT8U)(DevData.BattInfo[i].Volt);
    pDst[*nDstLen+3*i+2]=(INT8U)(DevData.BattInfo[i].Volt>>8);
  }
  *nDstLen+=3*nBattCount;	 			//121
  return isOKFlag;
}

BOOL AFN0C_F3_RtcStatus(INT8U *pSrc,INT16U *nSrcLen,INT8U *pDst,INT16U *nDstLen)   /*Rtcʵʱʱ��*/
{
  /*
  �� �� ʱ �� �� �� BCD
  */
  INT8U isOKFlag=TRUE;
  
  pDst[*nDstLen]=((DevData.TimeInfo.second/10)<<4)+((DevData.TimeInfo.second%10));
  pDst[*nDstLen+1]=((DevData.TimeInfo.minute/10)<<4)+((DevData.TimeInfo.minute%10));
  pDst[*nDstLen+2]=((DevData.TimeInfo.hour/10)<<4)+((DevData.TimeInfo.hour%10));
  pDst[*nDstLen+3]=((DevData.TimeInfo.day/10)<<4)+((DevData.TimeInfo.day%10));
  pDst[*nDstLen+4]=((DevData.TimeInfo.month/10)<<4)+((DevData.TimeInfo.month%10));
  pDst[*nDstLen+5]=(((DevData.TimeInfo.year%100)/10)<<4)+(((DevData.TimeInfo.year%100)%10));
  
  *nDstLen+=6;	 			//
  return isOKFlag;
}

BOOL AFN0F_F1_ReportRT(INT8U channel)
{
  
  INT8U nDstLen;
  INT8U pDst[MAX_MSG_LEN-20]={0};
  pDst[0]=(INT8U)(AFNXX_FX_DA>>8);
  pDst[1]=(INT8U)(AFNXX_FX_DA);
  pDst[2]=(INT8U)(AFN0F_F1_DT>>8);
  pDst[3]=(INT8U)(AFN0F_F1_DT);
  
  pDst[4]=bin2bcd(DevData.TimeInfo.year);
  pDst[5]=bin2bcd(DevData.TimeInfo.month);
  pDst[6]=bin2bcd(DevData.TimeInfo.day);
  pDst[7]=bin2bcd(DevData.TimeInfo.hour);
  pDst[8]=bin2bcd(DevData.TimeInfo.minute);
  pDst[9]=bin2bcd(DevData.TimeInfo.second);   
  nDstLen=10;
  
//  memcpy(&pDst[nDstLen],DevData.GpsInfo.SiteInfo.Longitude,DATALEN_LOC_LONG);  
//  nDstLen+=DATALEN_LOC_LONG;
//  
//  memcpy(&pDst[nDstLen],DevData.GpsInfo.SiteInfo.Latitude,DATALEN_LOC_LAT);
//  nDstLen+=DATALEN_LOC_LAT;
  
  pDst[nDstLen]=(INT8U)(DevData.GpsInfo.SiteInfo.Longitude);
  nDstLen+=1;  
  pDst[nDstLen]=(INT8U)(DevData.GpsInfo.SiteInfo.Longitude>>8);
  nDstLen+=1;
  pDst[nDstLen]=(INT8U)(DevData.GpsInfo.SiteInfo.Longitude>>16);
  nDstLen+=1;
  pDst[nDstLen]=(INT8U)(DevData.GpsInfo.SiteInfo.Longitude>>24);
  nDstLen+=1;
  
  pDst[nDstLen]=(INT8U)(DevData.GpsInfo.SiteInfo.Latitude);
  nDstLen+=1;  
  pDst[nDstLen]=(INT8U)(DevData.GpsInfo.SiteInfo.Latitude>>8);
  nDstLen+=1;
  pDst[nDstLen]=(INT8U)(DevData.GpsInfo.SiteInfo.Latitude>>16);
  nDstLen+=1;
  pDst[nDstLen]=(INT8U)(DevData.GpsInfo.SiteInfo.Latitude>>24);
  nDstLen+=1;
  
  pDst[nDstLen]=DevData.GpsInfo.ValidFlag;//GPS��λ״̬
  nDstLen+=1;
  
  pDst[nDstLen]=DevData.IsLightOnFlag;//���ص�״̬
  nDstLen+=1;
  
  //��ŵ������Ϣ����λ��ǰ
  pDst[nDstLen]=(INT8U)(DevData.ChargeSoc);
  pDst[nDstLen+1]=(INT8U)(DevData.ChargeSoc>>8);
  pDst[nDstLen+2]=(INT8U)(DevData.ChargeSoc>>16);
  pDst[nDstLen+3]=(INT8U)(DevData.ChargeSoc>>24);
  nDstLen+=4;
  
  pDst[nDstLen]=(INT8U)(DevData.DisChargeSoc);
  pDst[nDstLen+1]=(INT8U)(DevData.DisChargeSoc>>8);
  pDst[nDstLen+2]=(INT8U)(DevData.DisChargeSoc>>16);
  pDst[nDstLen+3]=(INT8U)(DevData.DisChargeSoc>>24);
  nDstLen+=4;
   
  for(INT8U i=0;i<DevInfo.BattCount;i++)//�����Ϣ
  {
    pDst[2*i+nDstLen]=DevData.BattInfo[i].Volt;
    pDst[2*i+nDstLen+1]=DevData.BattInfo[i].Volt>>8;
  }  
  nDstLen+=DevInfo.BattCount*2;
   
  nDstLen=Build_General_Response(AFN_AUTO_REPORT,TRUE,0x00, pDst ,nDstLen);	
  return CtuToSrv(pDst,nDstLen,channel);
}


BOOL AFN0F_F2_ReportHIS(INT8U channel,INT8U nRecordCount,INT8U nRecordPoint)
{
  
  INT8U nDstLen;
  INT8U pDst[30]={0};
  pDst[0]=(INT8U)(AFNXX_FX_DA>>8);
  pDst[1]=(INT8U)(AFNXX_FX_DA);
  pDst[2]=(INT8U)(AFN0F_F2_DT>>8);
  pDst[3]=(INT8U)(AFN0F_F2_DT);
  nDstLen=21;
  
  nDstLen=Build_General_Response(AFN_AUTO_REPORT,TRUE,0x00, pDst ,nDstLen);	
  return CtuToSrv(pDst,nDstLen,channel);
}



void Hex2Asc(INT8U* dst,INT8U* src,INT16U len)
{
  INT8U res[16]="0123456789ABCDEF";
  INT16U i=0;
  for(i=0;i<len;i++)
  {
    dst[2*i]=res[(src[i]>>4)&0x0f];
    dst[2*i+1]=res[(src[i])&0x0f];
  }
  
}


//68 L_L L_H L_L L_H 68 ADDR4 AFN SEQ DA2 DT2 chk 16
BOOL CtuToSrv(INT8U *pBuffer,INT32U nSize,INT8U ch)
{
  INT16U i=0,crc,nLenth;
  INT16U _nSize=0;
  INT8U pDst[MAX_MSG_LEN]={0};
  _nSize=8+nSize;                               //����Ϊ��ַ+����
  
  
  pDst[0]=TELEGRAM_HEAD;			//head
  pDst[1]=(INT8U)(_nSize);		        //LL-L
  pDst[2]=(INT8U)(_nSize>>8); 	                //LL-H
  pDst[3]=(INT8U)(_nSize);		        //LL-L
  pDst[4]=(INT8U)(_nSize>>8); 	                //LL-H
  pDst[5]=TELEGRAM_HEAD;			//head
  
  for(i=0;i<8;i++)
  {
    pDst[i+6]=DevInfo.CommAddr[i];
  }
  for(i=0;i<_nSize-8;i++)
  {
    pDst[i+14]=*pBuffer++;
  }
  crc=GetCRC16(pDst+6,_nSize);
  pDst[_nSize+6]=(INT8U)(crc);
  pDst[_nSize+7]=(INT8U)(crc>>8);
  pDst[_nSize+8]=TELEGRAM_END;
  
  nLenth=_nSize+9;
  if(ch==CHANNEL_GPRS)
  {
    GPRS_OutData(pDst,nLenth);
  }
  else if(ch==CHANNEL_P485)
  {
    //P485B_OutData(pDst,nSize+8);	
  }
  else
  {
    //GPRS_OutData(pDst,nLenth);
    //P485B_OutData(pDst,nSize+8);	
  }
  return TRUE;
}

/*��pBuffer����ȡ��Э��ָ��*/	//GPRS_RxBuf  GPRS_RxCount
BOOL FetchGprsCmd(INT8U* pBuffer,INT16U* nSize)
{
  INT16U nDataLen = 0;
  INT16U i;
  
  if(GPRS_RxCount <4)	//���ݰ�̫�̷��ش���
  {
    GPRS_RxCount=0;
    memset(GPRS_RxBuf,0,MAX_GPRS_RxBUF_SIZE);
    printf( "\r\nFetchGprsCmd:Lenth Too Small.\r\n");
    return FALSE;
  }
  
  for(i=0; i<GPRS_RxCount; i++)//��Э����ʼ��־
  {
    if((GPRS_RxBuf[i]==TELEGRAM_HEAD))//��LHЭ�����ʼ��־
    {	
      break;
    }
  }
  if(i >= GPRS_RxCount)//һֱ��������û���ҵ�����ͷ,������ȫ�����
  {
    GPRS_RxCount=0;
    memset(GPRS_RxBuf,0,MAX_GPRS_RxBUF_SIZE);
    printf( "\r\nFetchGprsCmd:No Head.\r\n");
    return FALSE;
  }
  
  memcpy(GPRS_RxBuf, GPRS_RxBuf+i, GPRS_RxCount-i);	//�Ƴ���Э�����ʼ��־
  GPRS_RxCount -= i;
  if(GPRS_RxCount < 5)
  {
    GPRS_RxCount=0;
    memset(GPRS_RxBuf,0,MAX_GPRS_RxBUF_SIZE);
    printf( "\r\nFetchGprsCmd:Msg Too Short.\r\n");
    return FALSE;//���ݰ�̫�̷��ش���
  }
  if(GPRS_RxBuf[5]!= TELEGRAM_HEAD)
  {
    GPRS_RxCount=0;
    memset(GPRS_RxBuf,0,MAX_GPRS_RxBUF_SIZE);
    printf( "\r\nFetchGprsCmd:No Head2.\r\n");
    return FALSE;
  }
  nDataLen = GPRS_RxBuf[2];//ȡ�����ݳ���
  nDataLen <<= 8;
  nDataLen |=GPRS_RxBuf[1];
  if(nDataLen>MAX_GPRS_RxBUF_SIZE)//��ֹ����
  {
    GPRS_RxCount=0;
    memset(GPRS_RxBuf,0,MAX_GPRS_RxBUF_SIZE);
    printf( "\r\nFetchGprsCmd:Buffer Overflow.\r\n");
    return FALSE;
  }
  if(GPRS_RxCount>=nDataLen+9)//����Э��
  {
    if (GPRS_RxBuf[nDataLen+8] == TELEGRAM_END)
    {		
      INT16U crc1=GetCRC16(GPRS_RxBuf+6,nDataLen);
      INT16U CRC_Rx = GPRS_RxBuf[(nDataLen+8)-1];
      CRC_Rx<<=8;
      CRC_Rx+=GPRS_RxBuf[(nDataLen+8)-2];
      
      if (crc1==CRC_Rx)
      {
        *nSize=nDataLen+9;//һ�����������ݰ�
        memcpy(pBuffer,GPRS_RxBuf,*nSize);//copy �����ı��İ�
        GPRS_RxCount-=nDataLen+9;//�޸ĳ��� Modi!
        GPRS_RxCount=0;
        memset(GPRS_RxBuf,0,MAX_GPRS_RxBUF_SIZE);
        printf( "\r\nFetchGprsCmd:Fetch Cmd Success.nSize=%d\r\n",*nSize);
        return TRUE;	
        
      }
      else//crc ����
      {		
        GPRS_RxCount=0;
        memset(GPRS_RxBuf,0,MAX_GPRS_RxBUF_SIZE);
        printf( "\r\nFetchGprsCmd:CRC Error\r\n");
        return FALSE;
      }
    }
    else//END����
    {
      GPRS_RxCount=0;
      memset(GPRS_RxBuf,0,MAX_GPRS_RxBUF_SIZE);
      printf( "\r\nFetchGprsCmd:No End\r\n");
      return FALSE;
    }
  }
  else
  {
    GPRS_RxCount=0;
    memset(GPRS_RxBuf,0,MAX_GPRS_RxBUF_SIZE);
    return FALSE;
  }
}

