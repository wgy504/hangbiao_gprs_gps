#ifndef __GPRS_H__
#define	__GPRS_H__

#define STATE_SUCCESS                   254

#define MAX_GPRS_RxBUF_SIZE             128
#define MAX_GPRS_RST_TIME		1	/*GPRS最长复位时间*/
#define MAX_GPRS_BOOT_TIME		3	/*GPRS最长启动时间*/
#define MAX_GPRS_ERR_COUNT		5	/*GPRS最大错误次数*/
#define MAX_GPRS_AT_TIMEOUT		5	/*AT指令超时时间，超时导致AT指令重发*/
#define MAX_GPRS_COMM_TIMEOUT	        10	/*GPRS报文交互超时时间，超时导致报文重发*/
#define MAX_GPRS_RST_COUNT              3       /*GPRS最大重启次数*/



//extern INT8U nStatus;
//extern INT64U	nTimeTick;
//extern INT8U	nErrCount;

extern BOOL GPRS_RxFlag;
extern INT8U GPRS_RxBuf[MAX_GPRS_RxBUF_SIZE];
extern INT16U GPRS_RxCount;
extern INT8U GPRS_TimeOut_Count;

extern void Task_Gprs(void);
extern void GPRS_Init(char* SvrIP,int SvrPort,char*SvrAPN);


extern INT8U GetGPRSStatus(void);
extern void GPRS_OutData(INT8U *Data,INT16U Len);




#endif // __GPRS_H__
