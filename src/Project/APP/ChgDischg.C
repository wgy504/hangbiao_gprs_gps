#include "app.h"

//电池电压电流采样
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
    //对于电压，Vref=2.5V,因为1/2分压，所以x2
    //对于电流，分压系数d，U=0.050R*I*50/d
    //d=1时，I=U/2.5
    //d=0.5时，I=U/5;
    switch(i)
    {
    case 0://B1电压
    case 1://B2电压
    case 2://B3电压
    case 3://B4电压
    case 4://B5电压
      DevData.BattInfo[i].Volt=(INT16U)(1000*(float)(tmpVal*2.5*2/4096.0));     //电压mV
      DevData.BattInfo[i].Volt=(DevData.BattInfo[i].Volt<1000)? 0:DevData.BattInfo[i].Volt;//小于1V，认为是没有电池
      break;      
    case 5://Charge电流
      DevData.ChargeCurr=(INT16U)(1000*(float)(tmpVal/4096.0));
      break;
    case 6://DisCharge电流 U/I=50
      DevData.DisChargeCurr=(INT16U)(1000*(float)(tmpVal/4096.0));
      break;
    default:
      break;
      
    }

  }
  
  for(INT8U i=0;i<(ABATT_COUNT+BBATT_COUNT+MBATT_COUNT);i++)
  {
    if(DevData.BattInfo[i].Volt<2500)//小于2.5V
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
充电控制策略
根据电压选择要放电的电池，更新全局变量
*/
BOOL ChargeProcess()
{ 
//  static BOOL isChangeFlag=TRUE;//电池切换
  static INT8U nErrCount=0;
  INT16U tmpVolt=0;
  static INT32U nTimeTick=0;
  BOOL IsFoundFlag=FALSE;
  
  if(nTimeTick==0)//没有电池在充电，系统刚上电或者充电结束，需要切换的时刻
  {
    tmpVolt=DevInfo.MaxChgVolt;
       
    //查找小于最高放电电压电池当中电压最小的电池
    for(INT8U i=0;i<(ABATT_COUNT+BBATT_COUNT);i++)
    {
      if((DevData.BattEnFlag>>i)&0x01)//电池有效
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
      nTimeTick=OSTimeGet();//更新充电时间
      return TRUE;
    }
    //所有电池都是满的
    nTimeTick=0;	
    return FALSE; 
  }
  else
  {
    if(OSTimeGet()-nTimeTick>=DevInfo.ChargePollIntval)//时间到，切换
    {
      DevData.LastChargeBattNo=DevData.ChargeBattNo;			        //保存本次充电的电池序号
      nTimeTick=0;							//切换电池
      if((DevInfo.ChgDebugLevel&DBG_INF))
      {
        printf("\r\nINF-CHG:Charge Circle Complete.\r\n\r\n");
      } 
    }
    else//时间不到
    {	
      if(DevData.BattInfo[DevData.ChargeBattNo].Volt>=DevInfo.MaxChgVolt)//电池充满，切换
      {       
        if(++nErrCount>=10)//连续10次电压高于最高充电电压,进行电池切换
        {
          DevData.LastChargeBattNo=DevData.ChargeBattNo;			//保存本次充电的电池序号
          nTimeTick=0;							//切换电池
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

//放电控制策略
/*
切换成功，返回TRUE，电池序号可用
切换失败，返回FALSE, 电池序号=255

*/
BOOL DischargeProcess()
{
  //static BOOL isChangeFlag=TRUE;//电池切换
  static INT8U nErrCount=0;
  INT16U tmpVolt=0;
  static INT32U nTimeTick=0; 
  
  BOOL IsFoundFlag=FALSE;

  if(nTimeTick==0)//没有电池在放电，系统刚上电或者放电结束，需要切换的时刻
  {
    tmpVolt=DevInfo.MinDischgVolt;
    //查找大于最低放电电压电池当中电压最大的电池，排除刚刚放过电的那个。
    for(INT8U i=0;i<(ABATT_COUNT);i++)
    {	
      if((DevData.BattEnFlag>>i)&0x01)//电池有效
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
      nTimeTick=OSTimeGet();//更新放电时间
      return TRUE;
    }

    
    tmpVolt=DevInfo.MinDischgVolt;
    for(INT8U i=0;i<(ABATT_COUNT);i++)//找不到，则选取最大的一个
    {
      if((DevData.BattEnFlag>>i)&0x01)//电池有效
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
      nTimeTick=OSTimeGet();//更新放电时间      
      return TRUE;
    }
    
    nTimeTick=0;//清空放电时间
    if((DevInfo.ChgDebugLevel&DBG_INF))
    {
      printf("\r\nINF-CHG:DisCharge Battery Not Found.\r\n\r\n");
    }   
    return FALSE;//所有电池都不满足要求，切换失败
  }
  else//正常工作过程
  {
    if(OSTimeGet()-nTimeTick>=DevInfo.DisChargePollIntval)//时间到
    {	
      DevData.LastDischargeBattNo=DevData.DischargeBattNo;	//保存本次放电的电池序号
      nTimeTick=0;
      if((DevInfo.ChgDebugLevel&DBG_INF))
      {
        printf("\r\nINF-CHG:DisCharge Circle Complete.\r\n\r\n");
      } 
    }
    else//时间不到
    {	
      if(DevData.BattInfo[DevData.DischargeBattNo].Volt<=DevInfo.MinDischgVolt)//电压低切换
      {
        if(++nErrCount>=5)//连续5次电压低于最低放电电压
        {
          DevData.LastDischargeBattNo=DevData.DischargeBattNo;	//保存本次放电的电池序号
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
  
  //放电控制
  retVal=DischargeProcess();//计算放电的电池序号
  
  if(retVal)
  {
    tmpBattNo=DevData.DischargeBattNo;
  }
  else//所有电池电压低
  {
    tmpBattNo=ABATT_COUNT+DevData.CurrBackBattNo;//启用备用电池
  } 
  DischgCtrl(tmpBattNo);
  
  //充电控制
  retVal=ChargeProcess();  
  if(retVal)
  {
    tmpBattNo=DevData.ChargeBattNo;
  }
  else//所有电池都充满
  {
    tmpBattNo=ABATT_COUNT;//启用强制开灯开关
  }  
  ChgCtrl(tmpBattNo);
  
  
  if((DevInfo.ChgDebugLevel&DBG_MSG))//
  {
    printf("\r\nMSG-CHG:ChargNo=%d,DisChargeNo=%d,BattEnFlag=0x%02X.\r\n\r\n",
           DevData.ChargeBattNo,DevData.DischargeBattNo,DevData.BattEnFlag);
  }  
  
}