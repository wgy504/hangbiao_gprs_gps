#ifndef __APP_H__
#define	__APP_H__


#include "stdio.h"//
#include "string.h"
#include "bsp.h"//包含硬件驱动层
#include "system.h"//

#include "math.h"

#include "msp430f_it.h"



#include "chgdischg.h"
#include "debug.h"

extern INT32U SysTimeTick;
extern INT32U OSTimeGet(void);
extern void Task_SysCtrl();

extern void Set_DftPara_To_E2p(void);
extern void Load_Para_From_E2p();
extern void Load_DevData_From_E2p();

extern INT8U bcd2bin(INT8U ch);
extern INT8U bin2bcd(INT8U ch);
extern char int2char(INT8U ch);
extern INT8U findCh(INT8U *pSrc,INT8U nLen,INT8U ch);



extern DEVICE_INFO      DevInfo;            //设备信息
extern DEVICE_DATA	DevData;            //设备实时数据
extern DEV_BASIC_INFO	DevBasicInfo;
#endif
