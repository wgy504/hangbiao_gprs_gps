#ifndef __PRPTOCOL_H__
#define	__PRPTOCOL_H__


//报文头尾标识
#define TELEGRAM_HEAD		0x68//
#define TELEGRAM_END		0x16

#define DIR_UP_FLAG		(INT8U)(1<<7ul)/*报文上行标识*/
#define PRM_MASTER_FLAG	(INT8U)(1<<6ul)
#define ACD_FLAG		(INT8U)(1<<5ul)

#define TPV_FLAG		(INT8U)(1<<7ul)
#define FIR_FLAG		(INT8U)(1<<6ul)
#define FIN_FLAG		(INT8U)(1<<5ul)
#define CON_FLAG		(INT8U)(1<<4ul)

//协议命令字
#define  LH_CMD_LOGIN		0x0100/*登录指令*/
#define  LH_CMD_HEART		0x0200/*心跳指令*/

//协议AFN
#define AFN_CONFIRM		0x00
#define AFN_RESET		0x01
#define AFN_LINK_DECT		0x02
#define AFN_PARA_SET		0x04
#define AFN_CMD_CTRL		0x05
#define AFN_RQST_CFG		0x09
#define AFN_INQURY_PARA		0x0A
#define AFN_RQST_RT_DATA	0x0C
#define AFN_RQST_HS_DATA	0x0D
#define AFN_RQST_EVNT_DATA	0x0E
#define AFN_AUTO_REPORT         0x0F
#define AFN_DATA_EXCHANGE	0x10


#define AFNXX_FX_DA			0x0000			//DA


#define AFNXX_F1_DT			0x0100			//DT
#define AFNXX_F2_DT			0x0200			//DT
#define AFNXX_F3_DT			0x0400			//DT
#define AFNXX_F4_DT			0x0800			//DT
#define AFNXX_F5_DT			0x1000			//DT
#define AFNXX_F6_DT			0x2000			//DT
#define AFNXX_F7_DT			0x4000			//DT
#define AFNXX_F8_DT			0x8000			//DT
#define AFNXX_F9_DT			0x0101			//DT
#define AFNXX_F10_DT		        0x0201			//DT
#define AFNXX_F11_DT		        0x0401			//DT
#define AFNXX_F12_DT		        0x0801			//DT
#define AFNXX_F13_DT		        0x1001			//DT
#define AFNXX_F14_DT		        0x2001			//DT
#define AFNXX_F15_DT		        0x4001			//DT
#define AFNXX_F16_DT		        0x8001			//DT
#define AFNXX_F17_DT		        0x0102			//DT
#define AFNXX_F18_DT		        0x0202			//DT
#define AFNXX_F19_DT		        0x0402			//DT
#define AFNXX_F20_DT		        0x0802			//DT

//
#define AFNXX_CONFIRM		        0x01				//确认
#define AFNXX_IGNORE		        0x00				//否认

// //AFN=00 确认、否认
// #define AFN00_F1_DT			AFNXX_F1_DT			//确认
// #define AFN00_F2_DT			AFNXX_F2_DT			//否认
	
//AFN=01 复位
#define	AFN01_F1_DT			AFNXX_F1_DT			//终端复位
#define	AFN01_F2_DT			AFNXX_F2_DT			//GPRS

//AFN=02 链路接口检测				   	
#define	AFN02_F1_DT			AFNXX_F1_DT			//登录
#define	AFN02_F3_DT			AFNXX_F3_DT			//心跳

//AFN=04 参数设置
#define	AFN04_F1_DT			AFNXX_F1_DT			//IP地址和端口	
#define	AFN04_F2_DT			AFNXX_F2_DT			//基本参数
#define	AFN04_F3_DT			AFNXX_F3_DT			//自定义灯质参数
#define	AFN04_F4_DT			AFNXX_F4_DT			//灯质源
#define	AFN04_F5_DT			AFNXX_F5_DT			//电池组个数
#define	AFN04_F6_DT			AFNXX_F6_DT			//电池容量
#define	AFN04_F7_DT			AFNXX_F7_DT			//电池充电放电电压
#define	AFN04_F8_DT			AFNXX_F8_DT			//亮度等级
#define	AFN04_F9_DT			AFNXX_F9_DT			//强开使能
#define	AFN04_F10_DT		AFNXX_F10_DT		//同步使能
#define	AFN04_F11_DT		AFNXX_F11_DT		//开关灯值
#define	AFN04_F12_DT		AFNXX_F12_DT		//开关灯容差
#define	AFN04_F13_DT		AFNXX_F13_DT		//预留
#define	AFN04_F14_DT		AFNXX_F14_DT		//预留
#define	AFN04_F15_DT		AFNXX_F15_DT		//预留
#define	AFN04_F16_DT		AFNXX_F16_DT		//预留
#define	AFN04_F17_DT		AFNXX_F17_DT		//预留
#define	AFN04_F18_DT		AFNXX_F18_DT		//预留
#define	AFN04_F19_DT		AFNXX_F19_DT		//预留
#define	AFN04_F20_DT		AFNXX_F20_DT		//基本配置参数

//AFN=05 控制命令
#define	AFN05_F1_DT			AFNXX_F1_DT			//设备遥控操作
#define	AFN05_F2_DT			AFNXX_F2_DT			//设备复位
#define	AFN05_F3_DT			AFNXX_F3_DT			//实时时钟

//AFN=09 请求终端配置及信息
#define	AFN09_F1_DT			AFNXX_F1_DT			//终端信息

//AFN=0A 查询参数,同AFN04设置参数
#define	AFN0A_F1_DT			AFN04_F1_DT		//IP地址和端口	
#define	AFN0A_F2_DT			AFN04_F2_DT		//基本参数
#define	AFN0A_F3_DT			AFN04_F3_DT		//自定义灯质参数
#define	AFN0A_F4_DT			AFNXX_F4_DT			//灯质源
#define	AFN0A_F5_DT			AFNXX_F5_DT			//电池组个数
#define	AFN0A_F6_DT			AFNXX_F6_DT			//电池容量
#define	AFN0A_F7_DT			AFNXX_F7_DT			//电池充电放电电压
#define	AFN0A_F8_DT			AFNXX_F8_DT			//亮度等级
#define	AFN0A_F9_DT			AFNXX_F9_DT			//强开使能
#define	AFN0A_F10_DT		AFNXX_F10_DT		//同步使能
#define	AFN0A_F11_DT		AFNXX_F11_DT		//开关灯值
#define	AFN0A_F12_DT		AFNXX_F12_DT		//开关灯容差
#define	AFN0A_F13_DT		AFNXX_F13_DT		//预留
#define	AFN0A_F14_DT		AFNXX_F14_DT		//预留
#define	AFN0A_F15_DT		AFNXX_F15_DT		//预留
#define	AFN0A_F16_DT		AFNXX_F16_DT		//预留
#define	AFN0A_F17_DT		AFNXX_F17_DT		//预留
#define	AFN0A_F18_DT		AFNXX_F18_DT		//预留
#define	AFN0A_F19_DT		AFNXX_F19_DT		//预留
#define	AFN0A_F20_DT		AFNXX_F20_DT		//基本配置参数

//AFN=0C 请求实时数据
#define	AFN0C_F1_DT			AFNXX_F1_DT			//设备工作状态
#define	AFN0C_F2_DT			AFNXX_F2_DT			//设备电池状态
#define	AFN0C_F3_DT			AFNXX_F3_DT			//实时时钟

//AFN=0E 请求事件数据
#define	AFN0E_F1_DT			AFNXX_F1_DT			//设备工作状态
#define	AFN0E_F2_DT			AFNXX_F2_DT			//设备电池状态

//AFN=0F 主动上报数据
#define	AFN0F_F1_DT			AFNXX_F1_DT			//实时数据上报
#define	AFN0F_F2_DT			AFNXX_F2_DT			//历史数据上报

extern BOOL AFN02_F1_Login(INT8U ch);	
extern BOOL AFN02_F3_Heart(INT8U ch);
extern BOOL AFN0F_F1_ReportRT(INT8U channel);
extern BOOL AFN0F_F2_ReportHIS(INT8U channel,INT8U nRecordCount,INT8U nRecordPoint);

extern BOOL CtuToSrv(INT8U *pBuffer,INT32U nSize,INT8U ch);
extern INT8U ProcessProtocol(INT8U* pBuffer,INT16U nSize,INT8U channel);
extern BOOL FetchGprsCmd(INT8U* pBuffer,INT16U* nSize);
extern INT8U SvrToCtu(INT8U* pBuffer,INT16U nSize,INT8U channel);
extern void Hex2Asc(INT8U* dst,INT8U* src,INT16U len);
#endif 
