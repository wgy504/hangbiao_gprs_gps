#ifndef __GPRS_H__
#define	__GPRS_H__

#define STATE_SUCCESS                   254

#define MAX_GPRS_RxBUF_SIZE             128
#define MAX_GPRS_RST_TIME		1	/*GPRS���λʱ��*/
#define MAX_GPRS_BOOT_TIME		3	/*GPRS�����ʱ��*/
#define MAX_GPRS_ERR_COUNT		5	/*GPRS���������*/
#define MAX_GPRS_AT_TIMEOUT		5	/*ATָ�ʱʱ�䣬��ʱ����ATָ���ط�*/
#define MAX_GPRS_COMM_TIMEOUT	        10	/*GPRS���Ľ�����ʱʱ�䣬��ʱ���±����ط�*/
#define MAX_GPRS_RST_COUNT              3       /*GPRS�����������*/



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
