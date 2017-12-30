#include "app.h"

//��ص�ѹ��������
void Get_BattInfo()
{

  for(INT8U i=0;i<(Num_of_Chs);i++)//
  {
    INT16U tmpVal=0;
    for(INT8U j=0;j<Num_of_Results;j++)
    {
      tmpVal+=AdcRes[i][j];
    }
    tmpVal=(INT16U)(tmpVal/Num_of_Results);
    //���ڵ�ѹ��Vref=2.5V,��Ϊ1/2��ѹ������x2
    //���ڵ�������ѹϵ��d��U=0.050R*I*50/d
    //d=1ʱ��I=U/2.5
    //d=0.5ʱ��I=U/5;
    switch(i)
    {
    case 0://B1��ѹ
    case 1://B2��ѹ
    case 2://B3��ѹ
    case 3://B4��ѹ
    case 4://B5��ѹ
      DevData.BattInfo[i].Volt=(INT16U)(1000*(float)(tmpVal*2.5*2/4096.0));     //��ѹmV
      DevData.BattInfo[i].Volt=(DevData.BattInfo[i].Volt<1000)? 0:DevData.BattInfo[i].Volt;//С��1V����Ϊ��û�е��
      break;      
    case 5://Charge����
      DevData.ChargeCurr=(INT16U)(1000*(float)(tmpVal/4096.0));
      break;
    case 6://DisCharge���� U/I=50
      DevData.DisChargeCurr=(INT16U)(1000*(float)(tmpVal/4096.0));
      break;
    default:
      break;
      
    }

  }
  
  for(INT8U i=0;i<(ABATT_COUNT+BBATT_COUNT+MBATT_COUNT);i++)
  {
    if(DevData.BattInfo[i].Volt<2500)//С��2.5V
    {
      DevData.BattEnFlag&=~(1<<i);
    }
    else
    {
      DevData.BattEnFlag|=(1<<i);
    }
  }
  
  if((DevInfo.ChgDebugLevel&DBG_MSG))//
  {
    printf("MSG-CHG:A0=%.2f,A1=%.2f,A2=%.2f,A3=%.2f,M0=%.2f,ChgCurr=%.3f,DisChgCurr=%.3f,ChgSoc(mAH)=%lu,DisSoc(mAH)=%lu.\r\n",
           (float)(DevData.BattInfo[0].Volt/(1000.0)),
           (float)(DevData.BattInfo[1].Volt/(1000.0)),
           (float)(DevData.BattInfo[2].Volt/(1000.0)),
           (float)(DevData.BattInfo[3].Volt/(1000.0)),
           (float)(DevData.BattInfo[4].Volt/(1000.0)),
           (float)(DevData.ChargeCurr/(1000.0)),
           (float)(DevData.DisChargeCurr/(1000.0)),
           DevData.ChargeSoc,
           DevData.DisChargeSoc
           );
  }  
}

/*
�����Ʋ���
���ݵ�ѹѡ��Ҫ�ŵ�ĵ�أ�����ȫ�ֱ���
*/
BOOL ChargeProcess()
{ 
//  static BOOL isChangeFlag=TRUE;//����л�
  static INT8U nErrCount=0;
  INT16U tmpVolt=0;
  static INT32U nTimeTick=0;
  BOOL IsFoundFlag=FALSE;
  
  if(nTimeTick==0)//û�е���ڳ�磬ϵͳ���ϵ���߳���������Ҫ�л���ʱ��
  {
    tmpVolt=DevInfo.MaxChgVolt;
       
    //����С����߷ŵ��ѹ��ص��е�ѹ��С�ĵ��
    for(INT8U i=0;i<(ABATT_COUNT+BBATT_COUNT);i++)
    {
      if((DevData.BattEnFlag>>i)&0x01)//�����Ч
      {
        if(DevData.BattInfo[i].Volt<tmpVolt)
        {
          if(i!=DevData.LastChargeBattNo)
          {
            tmpVolt=DevData.BattInfo[i].Volt;
            DevData.ChargeBattNo=i;
            IsFoundFlag=TRUE;
          }
        }	
      }	
    }  
    if(IsFoundFlag)
    {
      if((DevInfo.ChgDebugLevel&DBG_INF))
      {
        printf("\r\nINF-CHG:Charge Battery Found,No=%d.\r\n\r\n",DevData.ChargeBattNo);
      }    
      nTimeTick=OSTimeGet();//���³��ʱ��
      return TRUE;
    }
    //���е�ض�������
    nTimeTick=0;	
    return FALSE; 
  }
  else
  {
    if(OSTimeGet()-nTimeTick>=DevInfo.ChargePollIntval)//ʱ�䵽���л�
    {
      DevData.LastChargeBattNo=DevData.ChargeBattNo;			        //���汾�γ��ĵ�����
      nTimeTick=0;							//�л����
      if((DevInfo.ChgDebugLevel&DBG_INF))
      {
        printf("\r\nINF-CHG:Charge Circle Complete.\r\n\r\n");
      } 
    }
    else//ʱ�䲻��
    {	
      if(DevData.BattInfo[DevData.ChargeBattNo].Volt>=DevInfo.MaxChgVolt)//��س������л�
      {       
        if(++nErrCount>=10)//����10�ε�ѹ������߳���ѹ,���е���л�
        {
          DevData.LastChargeBattNo=DevData.ChargeBattNo;			//���汾�γ��ĵ�����
          nTimeTick=0;							//�л����
          if((DevInfo.ChgDebugLevel&DBG_INF))
          {
            printf("\r\nINF-CHG:BattVolt>MaxChgVolt.\r\n\r\n");
          } 
        }	
      }
      else
      {
        nErrCount=0;
      }    
    }   
  }
  return TRUE;
}

//�ŵ���Ʋ���
/*
�л��ɹ�������TRUE�������ſ���
�л�ʧ�ܣ�����FALSE, ������=255

*/
BOOL DischargeProcess()
{
  //static BOOL isChangeFlag=TRUE;//����л�
  static INT8U nErrCount=0;
  INT16U tmpVolt=0;
  static INT32U nTimeTick=0; 
  
  BOOL IsFoundFlag=FALSE;

  if(nTimeTick==0)//û�е���ڷŵ磬ϵͳ���ϵ���߷ŵ��������Ҫ�л���ʱ��
  {
    tmpVolt=DevInfo.MinDischgVolt;
    //���Ҵ�����ͷŵ��ѹ��ص��е�ѹ���ĵ�أ��ų��ոշŹ�����Ǹ���
    for(INT8U i=0;i<(ABATT_COUNT);i++)
    {	
      if((DevData.BattEnFlag>>i)&0x01)//�����Ч
      {        
        if(DevData.BattInfo[i].Volt>tmpVolt)
        {
          if(i!=DevData.LastDischargeBattNo)
          {
            tmpVolt=DevData.BattInfo[i].Volt;
            DevData.DischargeBattNo=i;	
            IsFoundFlag=TRUE;
          }
        }       
      }
    }

    if(IsFoundFlag)
    {
      if((DevInfo.ChgDebugLevel&DBG_INF))
      {
        printf("\r\nINF-CHG:DisCharge Battery Found,No=%d.\r\n\r\n",DevData.DischargeBattNo);
      }             
      nTimeTick=OSTimeGet();//���·ŵ�ʱ��
      return TRUE;
    }

    
    tmpVolt=DevInfo.MinDischgVolt;
    for(INT8U i=0;i<(ABATT_COUNT);i++)//�Ҳ�������ѡȡ����һ��
    {
      if((DevData.BattEnFlag>>i)&0x01)//�����Ч
      {
        if(DevData.BattInfo[i].Volt>tmpVolt)
        {
          tmpVolt=DevData.BattInfo[i].Volt;
          DevData.DischargeBattNo=i;	
          IsFoundFlag=TRUE;
        }
      }						
    } 
    
    if(IsFoundFlag)
    {
      if((DevInfo.ChgDebugLevel&DBG_INF))
      {
        printf("\r\nINF-CHG:DisCharge Battery Not Found,Find MaxVolt One.SelectNo=%d.\r\n\r\n",DevData.DischargeBattNo);
      }     
      nTimeTick=OSTimeGet();//���·ŵ�ʱ��      
      return TRUE;
    }
    
    nTimeTick=0;//��շŵ�ʱ��
    if((DevInfo.ChgDebugLevel&DBG_INF))
    {
      printf("\r\nINF-CHG:DisCharge Battery Not Found.\r\n\r\n");
    }   
    return FALSE;//���е�ض�������Ҫ���л�ʧ��
  }
  else//������������
  {
    if(OSTimeGet()-nTimeTick>=DevInfo.DisChargePollIntval)//ʱ�䵽
    {	
      DevData.LastDischargeBattNo=DevData.DischargeBattNo;	//���汾�ηŵ�ĵ�����
      nTimeTick=0;
      if((DevInfo.ChgDebugLevel&DBG_INF))
      {
        printf("\r\nINF-CHG:DisCharge Circle Complete.\r\n\r\n");
      } 
    }
    else//ʱ�䲻��
    {	
      if(DevData.BattInfo[DevData.DischargeBattNo].Volt<=DevInfo.MinDischgVolt)//��ѹ���л�
      {
        if(++nErrCount>=5)//����5�ε�ѹ������ͷŵ��ѹ
        {
          DevData.LastDischargeBattNo=DevData.DischargeBattNo;	//���汾�ηŵ�ĵ�����
          nTimeTick=0;
          
          if((DevInfo.ChgDebugLevel&DBG_INF))
          {
            printf("\r\nINF-CHG:BattVolt<MinDischgVolt.\r\n\r\n");
          } 
        }				
      }
      else
      {
        nErrCount=0;
      }
    }
  }
  return TRUE;
}



void Task_PwrMng()
{
  INT8U tmpBattNo=0;
  BOOL retVal=FALSE;
  
  //�ŵ����
  retVal=DischargeProcess();//����ŵ�ĵ�����
  
  if(retVal)
  {
    tmpBattNo=DevData.DischargeBattNo;
  }
  else//���е�ص�ѹ��
  {
    tmpBattNo=ABATT_COUNT+DevData.CurrBackBattNo;//���ñ��õ��
  } 
  DischgCtrl(tmpBattNo);
  
  //������
  retVal=ChargeProcess();  
  if(retVal)
  {
    tmpBattNo=DevData.ChargeBattNo;
  }
  else//���е�ض�����
  {
    tmpBattNo=ABATT_COUNT;//����ǿ�ƿ��ƿ���
  }  
  ChgCtrl(tmpBattNo);
  
  
  if((DevInfo.ChgDebugLevel&DBG_MSG))//
  {
    printf("\r\nMSG-CHG:ChargNo=%d,DisChargeNo=%d,BattEnFlag=0x%02X.\r\n\r\n",
           DevData.ChargeBattNo,DevData.DischargeBattNo,DevData.BattEnFlag);
  }  
  
}