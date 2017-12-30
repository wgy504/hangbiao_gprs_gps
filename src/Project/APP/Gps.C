#include "app.h"
/************************************************************
*文件名:GPS_DRV
*
*说明:GPS驱动调试函数
*实现功能，MCU 接收GPS传来的标准GPS格式的数据
*解析出其中的经度数据，纬度数据，格林威治时间参数，
*海拔高度参数，用户可以根据实际应用的需要来解析出
*其他的数据。
*

**************************************************************/
/*
*对外接口说明: 
*系统初始化的时候调用Init_GPS_module函数将该模块初始化
*然后只需要关注 gps_data_ok_flag，如果为真说明接收到一个
*有效的GPRMC数据段，然后调用Cal_gas_data_to_disp();
*进行数据转换，如果返回为真说明是定位后的数据，调用显示函数显示
*gps_time1[12]里面的时间信息和gps_longitude及gps_latitude定位信息。
*显示信息为ASCII码，以'|'为结束标志。
*
**************************************************************/


/***************************************************/
BOOL Gps_RMC_OK_Flag=FALSE;
BOOL Gps_VTG_OK_Flag=FALSE;     
//--------------------------------------
//unsigned char gps_height[10];              //海拔高度
//unsigned char rx_height_mode;           
//unsigned char rx_height_count;
//--------------------------------------
#define   MAX_RX_BUF  12
GPSBASEINFO GpsBaseInfo;
unsigned char com_rx_buf[MAX_RX_BUF+1];

//将经纬度转换为弧度
double LatToRad(INT8U *Lat);
double LonToRad(INT8U *Lon);
//已经两点的经纬度求解两点的距离
double DistanceCalc(double LatFrom,double LonFrom,double LatTo,double LonTo);
/*
Input:['1']['6']
Output:16
*/
INT8U str2bin(INT8U* pStr)
{
  return ((pStr[0]&0x0F)*10)+(pStr[1]&0x0F);
}

void Task_Gps()
{  
  static BOOL isWriteRtcFlag=FALSE;//定位成功后只写一次RTC
  static BOOL isOperFlag=FALSE;//只操作一次
    
  static INT8U nGpsOKCount=0;
  static INT32U nTimeTick=0;
  
  static BOOL IsFirstStartFlag=TRUE;
  
  if((OSTimeGet()-nTimeTick>=DevInfo.GpsIntval)||(IsFirstStartFlag))
  {
    GPS_PWR_EN(TRUE);
    DevInfo.IsGpsOnFlag=TRUE;//表示gps已经上电
    
    nTimeTick=OSTimeGet();
    nGpsOKCount=0;
    
    isWriteRtcFlag=FALSE;
    isOperFlag=FALSE;
    
    IsFirstStartFlag=FALSE;
    
    
    
    if((DevInfo.GpsDebugLevel&DBG_INF))//
    {     
      printf("INF-GPS:20%02d-%02d-%02d %02d:%02d:%02d Gps Power On.\r\n",
           DevData.TimeInfo.year,DevData.TimeInfo.month,DevData.TimeInfo.day,DevData.TimeInfo.hour,DevData.TimeInfo.minute,DevData.TimeInfo.second);
    }
  }
  else
  {

    if(Gps_RMC_OK_Flag)
    {
      Gps_RMC_OK_Flag=FALSE;
      
      if((DevInfo.GpsDebugLevel&DBG_MSG))//
      {
        printf("MSG-GPS:Date(DDMMYY)=%s,Time=%s,Lati=%s%c,Lon=%s%c,Valid=%c.\r\n",
               GpsBaseInfo.UtcDate,
               GpsBaseInfo.UtcTime,
               GpsBaseInfo.Latitude,
               GpsBaseInfo.LatitudeNS,
               GpsBaseInfo.Longitude,
               GpsBaseInfo.LongitudeEW,
               GpsBaseInfo.State
                 );
      }     

      if(GpsBaseInfo.State=='1')//已经定位
      {
        nGpsOKCount++;
      }
      else
      {
        nGpsOKCount=0;
      }
    } 
    
    if(nGpsOKCount>=3)//连续三次定位成功
    {
      //更新rtc时间，关闭gps,置标志位为TRUE
      //Date(DDMMYY)=030516,Time=052805.000,Lati=3104.5232N,Lon=12124.503E,Valid=1.
      if(isWriteRtcFlag==FALSE)
      {
        isWriteRtcFlag=TRUE;
        
        
        //保存下来最后定位的时间
        DevData.GpsInfo.Time.day=str2bin(&GpsBaseInfo.UtcDate[0]);
        DevData.GpsInfo.Time.month=str2bin(&GpsBaseInfo.UtcDate[2]);
        DevData.GpsInfo.Time.year=str2bin(&GpsBaseInfo.UtcDate[4]);      
        
        DevData.GpsInfo.Time.hour=((str2bin(&GpsBaseInfo.UtcTime[0])+8)%24);//差8个时区
        DevData.GpsInfo.Time.minute=str2bin(&GpsBaseInfo.UtcTime[2]);
        DevData.GpsInfo.Time.second=str2bin(&GpsBaseInfo.UtcTime[4]);
        
        //保存定位状态
        DevData.GpsInfo.ValidFlag=TRUE;
        
        //保存经纬度信息
        DevData.GpsInfo.SiteInfo.Longitude=(INT32U)(LonToRad(GpsBaseInfo.Longitude)*1000000);
        DevData.GpsInfo.SiteInfo.Latitude=(INT32U)(LatToRad(GpsBaseInfo.Latitude)*1000000);
        
        
        //同步RTC时间
        DS1337_Set_Time(DevData.GpsInfo.Time);     
              
        //关闭GPS
        GPS_PWR_EN(FALSE);
        DevInfo.IsGpsOnFlag=FALSE;
        
        if((DevInfo.GpsDebugLevel&DBG_INF))//
        {     
          printf("INF-GPS:20%02d-%02d-%02d %02d:%02d:%02d Positioning OK,LON=%6f,LAT=%6f,GPS Power Off.\r\n",
                 DevData.TimeInfo.year,DevData.TimeInfo.month,DevData.TimeInfo.day,DevData.TimeInfo.hour,DevData.TimeInfo.minute,DevData.TimeInfo.second,
                 ((float)DevData.GpsInfo.SiteInfo.Longitude)/1000000.0,((float)DevData.GpsInfo.SiteInfo.Latitude/1000000.0));
        }        
      }
    }
    else
    {
      if(OSTimeGet()-nTimeTick>=DevInfo.GpsMaxRunTime)//超过规定的GPS运行时间，还没有定位，则关闭GPS，同时认定本次定位失败
      {
        if(isOperFlag==FALSE)
        {
          isOperFlag=TRUE;
          //关闭gps,清标志位
          GPS_PWR_EN(FALSE);  
          DevInfo.IsGpsOnFlag=FALSE;
          
          //修改定位状态为FALSE
          DevData.GpsInfo.ValidFlag=FALSE;
          
          if((DevInfo.GpsDebugLevel&DBG_INF))//
          {     
            printf("INF-GPS:20%02d-%02d-%02d %02d:%02d:%02d Positioning Error,GPS Power Off.\r\n",
                   DevData.TimeInfo.year,DevData.TimeInfo.month,DevData.TimeInfo.day,DevData.TimeInfo.hour,DevData.TimeInfo.minute,DevData.TimeInfo.second);
          }          
        }
      }
    }   
  }
}


double LatToRad(INT8U *Lat)
{
  double Rad;
  INT16U Data;
  Data=(Lat[0]&0x0F)*10+(Lat[1]&0x0F);
  Rad=(Lat[2]&0x0F)*10+(Lat[3]&0x0F)+(Lat[5]&0x0F)*0.1+(Lat[6]&0x0F)*0.01+(Lat[7]&0x0F)*0.001+(Lat[8]&0x0F)*0.0001;
  Rad=Rad/60;
  Rad=Rad+Data;
  return Rad;
  
}
double LonToRad(INT8U *Lon)
{
  float Rad;
  INT16U Data;
  Data=(Lon[0]&0x0F)*100+(Lon[1]&0x0F)*10+(Lon[2]&0x0F);
  Rad=(Lon[3]&0x0F)*10+(Lon[4]&0x0F)+(Lon[6]&0x0F)*0.1+(Lon[7]&0x0F)*0.01+(Lon[8]&0x0F)*0.001+(Lon[9]&0x0F)*0.0001;
  Rad=Rad/60;
  Rad=Rad+Data;
  return Rad;
}
//double DistanceCalc(double LatFrom,double LonFrom,double LatTo,double LonTo)
//{
//  double LatFrom1,LonFrom1,LatTo1,LonTo1,LonDiff;
//  double Temp1,Temp2,Temp3;
//  double Distance;
//  LatFrom1=LatFrom*PI/180;
//  LonFrom1=LonFrom*PI/180;
//  LatTo1=LatTo*PI/180;
//  LonTo1=LonTo*PI/180;
//  LonDiff=LonTo1-LonFrom1;
//  Temp1=cos(LatTo1)*sin(LonDiff);	
//  Temp1=Temp1*Temp1;
//  Temp2=cos(LatFrom1)*sin(LatTo1)-sin(LatFrom1)*cos(LatTo1)*cos(LonDiff);
//  Temp2=Temp2*Temp2;
//  Temp3=sin(LatFrom1)*sin(LatTo1)+cos(LatFrom1)*cos(LatTo1)*cos(LonDiff);
//  Distance=(Temp3==0)?0:atan(sqrt(Temp1+Temp2)/Temp3);
//  Distance=EARTHR*Distance;
//  return Distance ;		
//}

/********************************************************
*函数名:rx_gps_data
*
*函数功能:解析出GPRMC信息的数据
*同样可以按这种方式解析出像GPGGA等
*这些信息的数据
例如:
$GPRMC,1,2,3,4,5,6,7,8,9,10,11,A*66
其中各参数的意义:
例：$GPRMC,024813.640,A,3158.4608,N,11848.3737,E,10.05,324.27,150706,,,A*50
字段0：$GPRMC，语句ID，表明该语句为Recommended Minimum Specific GPS/TRANSIT Data（RMC）推荐最小定位信息
字段1：UTC时间，hhmmss.sss格式
字段2：状态，A=定位，V=未定位
字段3：纬度ddmm.mmmm，度分格式（前导位数不足则补0）
字段4：纬度N（北纬）或S（南纬）
字段5：经度dddmm.mmmm，度分格式（前导位数不足则补0）
字段6：经度E（东经）或W（西经）
字段7：速度，节，Knots
字段8：方位角，度
字段9：UTC日期，DDMMYY格式
字段10：磁偏角，（000 - 180）度（前导位数不足则补0）
字段11：磁偏角方向，E=东W=西
字段12：校验值


*********************************************************/
/*
$GNGGA,052306.000,3104.5270,N,12124.5039,E,1,7,1.13,-6.5,M,8.1,M,,*5E
$GPGSA,A,3,14,23,27,32,29,193,,,,,,,1.48,1.13,0.95*30
$BDGSA,A,3,08,,,,,,,,,,,,1.48,1.13,0.95*19
$GPGSV,4,1,13,26,72,281,19,31,56,033,17,42,48,146,27,16,45,242,*75
$GPGSV,4,2,13,32,42,148,38,14,41,142,32,29,29,057,23,22,26,253,*79
$GPGSV,4,3,13,03,20,289,,193,14,177,38,27,08,191,38,23,07,319,27*4D
$GPGSV,4,4,13,25,04,048,33*44
$BDGSV,1,1,02,08,19,165,35,03,,,30*5E
$GNRMC,052306.000,A,3104.5270,N,12124.5039,E,0.22,185.72,090416,,,A*7C
$GPTXT,01,01,02,ANTSTATUS=OK*3B
*/
void Rcv_Gps_Data(unsigned char com_rx_byte)
{
  unsigned char i;
  static unsigned char nState;
  static unsigned char nRcvCount;
  if(com_rx_byte=='$')
  {
    nState=0;
    nRcvCount=0;
    for(i=0;i<5;i++)           //清除标志字段字节
      com_rx_buf[i]=0;
  }
  else
  {
    if(nRcvCount<MAX_RX_BUF)   //防止无效数据造成数据溢出
      com_rx_buf[nRcvCount++]=com_rx_byte;
  } 
  
  switch(nState)
  {
  case 0:                         //接收GPRMC字段  
    if(com_rx_byte==',')
    {
      if(((com_rx_buf[0]=='G')&&(com_rx_buf[1]=='P')&&(com_rx_buf[2]=='R')&&(com_rx_buf[3]=='M')&&(com_rx_buf[4]=='C'))||//GPS
         ((com_rx_buf[0]=='G')&&(com_rx_buf[1]=='N')&&(com_rx_buf[2]=='R')&&(com_rx_buf[3]=='M')&&(com_rx_buf[4]=='C')))//beidou
      {
        nState++;//转到下一字段的接收
        nRcvCount=0;
      }
      //---------------------------//接收GPVTG字段，解析出速度信息 
      else if((com_rx_buf[0]=='G')&&(com_rx_buf[1]=='P')&&(com_rx_buf[2]=='V')&&
              (com_rx_buf[3]=='T')&&(com_rx_buf[4]=='G'))
      {
        nState=12;//转到VTG字段的接收
        nRcvCount=0;
      }
      
      //---------------------------
      
      nRcvCount=0; 
    }
    break;
    
  case 1:                       //接收时间字段  
    if(com_rx_byte==',')
    {
      for(i=0;i<10;i++)
      {
	GpsBaseInfo.UtcTime[i]=com_rx_buf[i];
      }
      GpsBaseInfo.UtcTime[10]='\0';
      nState++;//转到下一字段的接收
      nRcvCount=0; 
    }
    break;
    
  case 2:                           //接收定位状态字段  
    if(com_rx_byte==',')
    {
      if(com_rx_buf[0]=='A')	//已经定位
      {
	GpsBaseInfo.State='1';
      }
      else
      {
	GpsBaseInfo.State='0';
      }
      nState++;//转到下一字段的接收
      nRcvCount=0; 
    }
    break;
    
  case 3:                            //纬度ddmm.mmmm，度分格式
    if(com_rx_byte==',')
    {
      for(i=0;i<9;i++)
      {
	GpsBaseInfo.Latitude[i]=com_rx_buf[i];
      }
      GpsBaseInfo.Latitude[9]='\0';
      nState++;//转到下一字段的接收
      nRcvCount=0; 
    }
    break;
    
  case 4:                          //纬度N（北纬）或S（南纬）
    if(com_rx_byte==',')
    {
      GpsBaseInfo.LatitudeNS=com_rx_buf[0];
      nState++;//转到下一字段的接收
      nRcvCount=0; 
    }
    break;
    
  case 5:                             //经度dddmm.mmmm，度分格式
    if(com_rx_byte==',')
    {
      for(i=0;i<10;i++)
      {
	GpsBaseInfo.Longitude[i]=com_rx_buf[i];
      }
      GpsBaseInfo.Longitude[9]='\0';
      nState++;//转到下一字段的接收
      nRcvCount=0; 
    }
    break;
    
  case 6:                            //经度E（东经）或W（西经）
    if(com_rx_byte==',')
    {
      GpsBaseInfo.LongitudeEW=com_rx_buf[0];
      nState++;//转到下一字段的接收
      nRcvCount=0; 
    }
    break;
    
  case 7:                               //速度，节，Knots
    if(com_rx_byte==',')
    {
      nState++;//转到下一字段的接收
      nRcvCount=0; 
    }
    break;
    
  case 8:                        //速度方位角，度
    if(com_rx_byte==',')
    {
      nState++;//转到下一字段的接收
      nRcvCount=0; 
    }
    break;
    
  case 9:                     //UTC日期，DDMMYY格式
    if(com_rx_byte==',')
    {
      for(i=0;i<6;i++)
      {
	GpsBaseInfo.UtcDate[i]=com_rx_buf[i];
      }
      GpsBaseInfo.UtcDate[6]=0;
      nState++;//转到下一字段的接收
      nRcvCount=0; 
    }
    break;
    
  case 10:        //字段10和11的两个参数普通的GPS模块无此参数
    if(com_rx_byte==',')
    {
      //
      nState++;//转到下一字段的接收
      nRcvCount=0; 
    }
    break;
    
  case 11:        //接收一个完整的GPRMC信息段完成置相应的结标志
    if(com_rx_byte==',')
    {
      Gps_RMC_OK_Flag=TRUE;
      nState=0;//回到初始状态
      nRcvCount=0; 
    }
    break;
    
    
    //==================================================================
  case 12:    //接收GPVTG报文的运动角度，真北参考系
    if(com_rx_byte==',')
    {
      nState++;//转到下一字段的接收
      nRcvCount=0; 
    }
    
    break;
  case 13:    //
    if(com_rx_byte==',')
    {
      nState++;//转到下一字段的接收
      nRcvCount=0; 
    }
    break;
    
  case 14:    //接收GPVTG报文的运动角度，磁北参考系
    if(com_rx_byte==',')
    {
      nState++;//转到下一字段的接收
      nRcvCount=0; 
    }
    break;
    
  case 15:    //
    if(com_rx_byte==',')
    {
      nState++;//转到下一字段的接收
      nRcvCount=0; 
    }
    break;
    
  case 16:    //接收GPVTG报文的水平运动速度	，单位：节
    if(com_rx_byte==',')
    {
      nState++;//转到下一字段的接收
      nRcvCount=0; 
    }
    break;
    
  case 17:    //
    if(com_rx_byte==',')
    {
      nState++;//转到下一字段的接收
      nRcvCount=0; 
    }
    break;
    
  case 18:    //接收GPVTG报文的水平运动速度	，单位：千米每小时
    if(com_rx_byte==',')
    {
      Gps_VTG_OK_Flag=TRUE;
      nState=0;//转到初始状态
      nRcvCount=0; 
    }
    
    break;
    //----------------------------  
  default:
    nState=0;
    nRcvCount=0; 
    break;
  }
  com_rx_byte=0xff; 
}
