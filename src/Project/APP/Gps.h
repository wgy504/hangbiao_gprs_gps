#ifndef __GPS_H__
#define __GPS_H__
#include <bsp.h>

typedef struct
{
  INT8U UtcTime[11]; //ʱ��
  INT8U State;   //��λ״̬
  INT8U  Latitude[10];	//γ��
  INT8U LatitudeNS; //γ�Ȱ���
  INT8U Longitude[11];	 //����	
  INT8U LongitudeEW;//���Ȱ���
  //INT8U Speed[6];	     //��������
  //INT8U Azimuth[6];	 //����
  INT8U UtcDate[7]; //����	  
  
  //INT8U Altitude[8];   //�߶�
  //INT8U SatelliteNum;//������ 
}GPSBASEINFO;


//extern GPSBASEINFO GpsBaseInfo;
extern void Rcv_Gps_Data(unsigned char com_rx_byte);
//extern BOOL Gps_RMC_OK_Flag;
extern void Task_Gps();


#endif
