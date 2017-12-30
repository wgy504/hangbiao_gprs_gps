#ifndef __RTC_H__
#define __RTC_H__
#include <bsp.h>

#define DS1337_ADDRESS 0xD0	//1101 A2 A1 A0 R/W
#define REG_SEC                 0x00
#define REG_MIN                 0x01
#define REG_HOUR                0x02
#define REG_WEEK                 0x03
#define REG_DAY                0x04
#define REG_MON                 0x05
#define REG_YEAR                0x06

#define REG_CTRL                 0x0E
#define REG_STATUS               0x0F




extern void Get_RtcTime();
extern void DS1337_Set_Time(TIME_INFO Times);



#endif
