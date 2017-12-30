#ifndef __DEBUG_H__
#define __DEBUG_H__
#include <bsp.h>
#define MAX_DBG_RxBUF_SIZE           20
extern INT8U DBG_RxBuf[MAX_DBG_RxBUF_SIZE];
extern INT8U DBG_RxCount;
extern INT8U DBG_RxFlag;
extern INT8U DBG_TimeOut_Count;//串口通信超时时间，超时意味着一帧接收完成。

extern void Task_Dbg();



#endif
