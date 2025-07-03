#ifndef __MYIIC_H__
#define __MYIIC_H__

#include <stdint.h>

#define I2C_BuffSize 60

#define  I2C_PORT GPIOA
#define  SDA  SDA_Pin
#define  SCL  SCL_Pin
#define  DLY  delay_us(1)


#define FAILURE 1
#define SUCCEED 0

void SDA_Input_Mode(void);
void SDA_Output_Mode(void);
void SDA_Output(uint8_t val );
void SCL_Output(uint8_t val );
uint8_t SDA_Input(void);
void delay(uint8_t n);
void IICStart(void);
void IICStop(void);
uint8_t IICWaitAck(void);
void IICSendAck(void);
void IICSendNotAck(void);
void IICSendByte(uint8_t cSendByte);
uint8_t IICReceiveByte(void);

void IIC_WR(uint8_t addr, uint8_t len);
void IIC_RD(uint8_t addr, uint8_t len);
void IIC_RD2(uint8_t addr, uint8_t len);
void IIC_WR2(uint8_t addr, uint8_t data);
void IIC_WR3(uint8_t addr, uint8_t data);
#endif
