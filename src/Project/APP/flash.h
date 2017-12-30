#ifndef __FLASH_H__
#define	__FLASH_H__


#include "bsp.h"

//extern INT8U Flash_Rhythm_Index;
//extern INT8U Flash_Switch_Index;
extern BOOL Light_On_Flag;
extern INT8U Get_Rhythm_Index(void);
extern INT8U Get_Switch_Index(void);

extern const INT16U Flash_Rhythm_Chat[256][20];
extern void Flash_Ctrl(BOOL Flash_En_Flag,unsigned char Flash_Rhythm_Index);

#endif
