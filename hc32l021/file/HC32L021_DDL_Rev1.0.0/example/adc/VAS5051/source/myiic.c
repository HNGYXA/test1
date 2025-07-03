#include "myiic.h"
#include "gpio.h"

uint8_t I2C_ADDR_WR = 0x4E;
uint8_t I2C_ADDR_RD = 0x4F;

uint8_t WR_Buffer[2*I2C_BuffSize] = {0};
uint8_t RD_Buffer[2*I2C_BuffSize] = {0};

void SDA_Input_Mode()
{
    stc_gpio_init_t stcGpioInit = {0};

    SYSCTRL_PeriphClockEnable(PeriphClockGpio); /*����GPIOʱ��*/

    GPIO_StcInit(&stcGpioInit);                   /* �ṹ�������ʼֵ��ʼ�� */
    stcGpioInit.bOutputValue = TRUE;              /* ����GPIO�����ʼֵΪ�� */
    stcGpioInit.u32Mode      = GPIO_MD_INPUT; 		/* �˿ڷ������� */
    stcGpioInit.u32PullUp    = GPIO_PULL_NONE;    /* �˿��������� */
    stcGpioInit.u32Pin       = GPIO_PIN_00;       /* �˿��������� */
    GPIOA_Init(&stcGpioInit);                     /* GPIO�˿ڳ�ʼ�� */
}
void SDA_Output_Mode()
{
		stc_gpio_init_t stcGpioInit = {0};

    SYSCTRL_PeriphClockEnable(PeriphClockGpio); /*����GPIOʱ��*/

    GPIO_StcInit(&stcGpioInit);                   /* �ṹ�������ʼֵ��ʼ�� */
    stcGpioInit.bOutputValue = TRUE;              /* ����GPIO�����ʼֵΪ�� */
    stcGpioInit.u32Mode      = GPIO_MD_OUTPUT_OD; 		/* �˿ڷ������� */
    stcGpioInit.u32PullUp    = GPIO_PULL_NONE;    /* �˿��������� */
    stcGpioInit.u32Pin       = GPIO_PIN_00;       /* �˿��������� */
    GPIOA_Init(&stcGpioInit);                     /* GPIO�˿ڳ�ʼ�� */
}

void SDA_Output(uint8_t val)
{
	if(val) {
		GPIO_PA00_SET();
	} else {
		GPIO_PA00_RESET();
	}
}
void SCL_Output(uint8_t val)
{
	if(val) {
		GPIO_PA15_SET();
	} else {
		GPIO_PA15_RESET();
	}
}
uint8_t SDA_Input()
{
	return GPIO_PA00_READ();
}

void delay(uint8_t n)
{
	uint8_t i;
	for ( i=0;i<n;++i);
}

//	IIC start
void IICStart()
{
	SCL_Output(1);delay(10);
	SDA_Output(1);delay(10);
	SDA_Output(0);delay(10);
	SCL_Output(0);delay(10);
}
//	IIC stop
void IICStop()
{
	delay(10);
	SCL_Output(1); 
	delay(10);
	SDA_Output(0);
	SDA_Output(1); 
	delay(10);

}

//	Wait ACK
uint8_t IICWaitAck()
{
	unsigned short cErrTime = 5;
	SDA_Input_Mode(); 
	SCL_Output(1);
	while(SDA_Input())
	{
		cErrTime--;
		delay(1);
		if (0 == cErrTime)
		{
			SDA_Output_Mode();
			SDA_Output(0);
			IICStop();
			return FAILURE;
		}
	}
	SDA_Output_Mode();
	SCL_Output(0);
	return SUCCEED;
}

//	send ACK
void IICSendAck()
{
	SDA_Output(0); delay(1);
	SCL_Output(1); delay(10);
	SCL_Output(0); delay(10);
	SDA_Output(1); delay(1);
}

//	Send no ACK
void IICSendNotAck()
{
	SDA_Output(1);   delay(1);
	SCL_Output(1);   delay(10);
	SCL_Output(0);   delay(10);
	SDA_Output(0);   delay(1);
}

//	Send one byte data
void IICSendByte(uint8_t cSendByte)
{
	uint8_t  i = 8;
	while (i--) {
		SCL_Output(0);
		delay(1); 
		SDA_Output(cSendByte & 0x80); 
		cSendByte += cSendByte;
		delay(10); 
		SCL_Output(1);
		delay(10); 
	}
	SCL_Output(0);
	SDA_Output(1);
	delay(10); 
}

//	Receive one byte data
uint8_t IICReceiveByte()
{
	uint8_t i = 8;
	uint8_t cR_Byte = 0;
	SDA_Input_Mode(); 
	while (i--) {
		cR_Byte += cR_Byte;
		SCL_Output(0);
		delay(10); 
		SCL_Output(1);
		delay(10); 
		cR_Byte |=  SDA_Input(); 
	}
	SCL_Output(0);
	delay(10); 
	SDA_Output_Mode();
	return cR_Byte;
}

void IIC_WR(uint8_t addr, uint8_t len){
	IICStart();
	IICSendByte(I2C_ADDR_WR);
	IICWaitAck();
	IICSendByte(addr);
	IICWaitAck();
	IICSendByte(len);
	IICWaitAck();
	IICStop();
}

void IIC_RD(uint8_t addr, uint8_t len){
	uint8_t i = 1;
	IICStart();
	IICSendByte(I2C_ADDR_WR);
	IICWaitAck();
	IICSendByte(addr);
	IICWaitAck();
	IICStop();
	delay(10);
	IICStart();
	IICSendByte(I2C_ADDR_RD);
	IICWaitAck();
	RD_Buffer[0] = IICReceiveByte();
	for(i=1; i<len; i++){
		IICSendAck();
		RD_Buffer[i] = IICReceiveByte();
		if(i >= I2C_BuffSize)
			break;
	}
	IICSendNotAck();
	IICStop();
}