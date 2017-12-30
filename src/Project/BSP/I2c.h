#ifndef __I2C_H__
#define __I2C_H__
#include <bsp.h>

#define SDA_PIN BIT0
#define SCL_PIN BIT1


#define SDA_IN()  (P4DIR &=~SDA_PIN)
#define SDA_OUT() (P4DIR |=SDA_PIN)
#define SCL_OUT() (P4DIR |=SCL_PIN)

#define SDA_HIGH()   (P4OUT |= SDA_PIN)
#define SDA_LOW()   (P4OUT &=~SDA_PIN)
#define SCL_HIGH()   (P4OUT |= SCL_PIN)
#define SCL_LOW()   (P4OUT &=~SCL_PIN)

#define SDA_READ()  ((P4IN>>0)&0x01)

extern uchar ErrorBit; //读写错误标志 1错误 0 正确

extern void i2c_init();
extern BOOL i2c_start(void);
extern void i2c_stop(void);
extern void i2c_ack(void);
extern void i2c_no_ack(void);
extern BOOL i2c_wait_ack(void );
extern void i2c_send_byte(uchar data);
extern uchar i2c_read_byte(void);

#endif
