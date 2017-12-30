#ifndef __GPS_H__
#define __GPS_H__
#include <bsp.h>

typedef struct
{
  INT8U UtcTime[11]; //时间
  INT8U State;   //定位状态
  INT8U  Latitude[10];	//纬度
  INT8U LatitudeNS; //纬度半球
  INT8U Longitude[11];	 //经度	
  INT8U LongitudeEW;//经度半球
  //INT8U Speed[6];	     //地面速率
  //INT8U Azimuth[6];	 //航向
  INT8U UtcDate[7]; //日期	  
  
  //INT8U Altitude[8];   //高度
  //INT8U SatelliteNum;//卫星数 
}GPSBASEINFO;


//extern GPSBASEINFO GpsBaseInfo;
extern void Rcv_Gps_Data(unsigned char com_rx_byte);
//extern BOOL Gps_RMC_OK_Flag;
extern void Task_Gps();


#endif
