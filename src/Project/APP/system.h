#ifndef __SYSTEM_H__
#define	__SYSTEM_H__
//延时函数，IAR自带，经常使用到
#define CPU_F ((double)8000000)   //外部高频晶振8MHZ
//#define CPU_F ((double)32768)   //外部低频晶振32.768KHZ
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0)) 
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0)) 


//自定义数据结构，方便使用
#define uchar unsigned char
#define uint  unsigned int
#define ulong unsigned long

typedef unsigned char INT8U;
typedef unsigned int INT16U;
typedef unsigned char BOOL;
typedef unsigned long INT32U;

#define FALSE		0
#define TRUE		(!FALSE)


#define MAX_BATT_COUNT			16		                        /*最大电池数量*/
#define MAX_BASE_COUNT			7		                        /*最大基站数量，1个连接基站+6个邻近基站*/
#define MAX_RHYTHM_COUNT		10		                        /*最大自定义灯质数量*/


/////////////////////////////////////eep address assignment end////////////////////////////////
#define ABATT_COUNT		4                                               //主电池组
#define BBATT_COUNT		1                                               //备用电池组
#define MBATT_COUNT             0                                               //管理电池组

#define ALARM_B0_ERR	(1<<0)
#define ALARM_B1_ERR	(1<<1)
#define ALARM_B2_ERR	(1<<2)
#define ALARM_B3_ERR	(1<<3)
#define ALARM_ALL_LOW	(1<<7)


#define DBG_ERR	(1<<0)//错误信息日志
#define DBG_INF	(1<<1)//操作信息日志
#define DBG_MSG	(1<<2)//变量的信息
#define DBG_OFF (0)//全关

/*BasicInfo*/
typedef struct dev_basic_info
{
	INT8U	OEMCode[5];			                                /*厂商代号*/
	INT8U	SWVersion[3];		                                        /*软件版本*/
	INT8U	PTType;				                                /*协议类型*/
	INT8U	PTVersion[3];		                                        /*协议版本*/
	INT8U	HWVersion[3];		                                        /*硬件版本*/
}DEV_BASIC_INFO;



//enum battStatus
//{
//  Status_Idle=0,							        /*空闲*/
//  Status_Charge=1,						                /*充电*/
//  Status_Discharge=2,		 				                /*放电*/
//  Status_Bypass=3							        /*旁路*/
//};
//



typedef struct batt_info
{
  INT16U Volt;			                                                /*当前电压mV  */

  INT8U ErrCount;
  INT8U LastErrCount;


}BATT_INFO;


/*设备基本信息-配置信息*/
typedef struct device_info
{
  INT8U BattCount;			                                        /*电池组数量 ，最大16*/
  INT32U BattSoc;				                                /*电池组容量mAh*/
  INT16U MaxChgVolt;			                                        /*最大充电电压，mV*/
  INT16U MinDischgVolt;		                                                /*最小放电电压，mV*/
  INT16U ChargePollIntval;	                                                /*充电轮询周期，S*/
  INT16U DisChargePollIntval;	                                                /*放电轮询周期，S*/


  INT8U SysDebugLevel;		                                                /*日志打印开关*/			

  INT8U SubDebugLevel;		                                                /*日志打印开关*/	

  INT8U ChgDebugLevel;		                                                /*日志打印开关*/
  
}DEVICE_INFO;


/*设备实时数据-运行实时数据*/
typedef struct device_data
{
  
  //电池信息
  BATT_INFO	BattInfo[ABATT_COUNT+BBATT_COUNT+MBATT_COUNT];	                /*电池信息---实时*/
  INT16U ChargeCurr;                                                            /*充电电流---单位mA 实时*/
  INT16U DisChargeCurr;                                                         /*放电电流---单位mA 实时*/
  INT32U ChargeSoc;                                                             /*充电容量---单位mAh 实时*/
  INT32U DisChargeSoc;                                                          /*放电容量---单位mAh 实时*/
  
  INT8U ChargeBattNo;								/*当前充电电池序号---实时*/
  INT8U DischargeBattNo;							/*当前放电电池序号---实时*/
  INT8U LastChargeBattNo;							/*上次充电电池序号---实时*/
  INT8U LastDischargeBattNo;						        /*上次放电电池序号---实时*/
  INT8U BattEnFlag;                                                             /*电池有效标志---实时*/  
  
  INT8U CurrBackBattNo;                                                         /*当前启用的备用电池序号---实时*/  
}DEVICE_DATA;


#endif
