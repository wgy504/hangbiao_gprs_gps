#ifndef __E2PROM_H__
#define __E2PROM_H__
#include <bsp.h>

#define EEP_TYPE    XX24C128      /*EEP µÄÐÍºÅ*/
//#define EEP_TYPE    XX24C256
//#define EEP_TYPE    XX24C512

//#define EEP_ADDRESS 0xA0	//1010 A2 A1 A0 R/W for HW3.10
#define EEP_ADDRESS 0xA8	//1010 A2 A1 A0 R/W for HW3.20

#if   (EEP_TYPE==XX24C128)  
#define EEP_MAX_SIZE  16*1024   //16K
#elif (EEP_TYPE==XX24C256) 
#define EEP_MAX_SIZE  32*1024   //32K
#elif (EEP_TYPE==XX24C512) 
#define EEP_MAX_SIZE  64*1024   //32K
#endif



#define delay_MS(x) delay_ms(x)

extern void E2p_Init();

extern BOOL Para_Save(INT16U ee_addr, INT8U *dat, INT16U len);
extern BOOL Para_Read(INT16U ee_addr, INT8U *data, INT16U len);

extern void E2p_Test();





#endif
