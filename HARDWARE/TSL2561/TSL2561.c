#include "TSL2561.h"

void tsl2561_set_timing(u8 gain, u8 time);
void tsl2561_set_powerup(void);

u8 tsl2561_init(void)
{
	u8 dev_id = 0;
	
  GPIO_InitTypeDef  GPIO_InitStructure;
	I2C_InitTypeDef 	I2C_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOA时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);//TIM2时钟使能 

  //GPIOA1  复用功能,上拉
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//上拉
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource8,GPIO_AF_I2C1); 
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource9,GPIO_AF_I2C1); //GPIOA6复用为TIM13
	
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = 100000;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_OwnAddress1 = 0x72;
	
	I2C_Cmd(I2C1, ENABLE);
	I2C_Init(I2C1, &I2C_InitStructure);
	
	I2C_AcknowledgeConfig(I2C1, ENABLE);
	
	while(1)
	{
		dev_id = tsl2561_read(0x0A);
		
		if(dev_id == 0x50)
			break;
		delay_ms(1000);
	}
	
	tsl2561_set_timing(0, 1);
	tsl2561_set_powerup();
	
	return 0;
}


u8 tsl2561_read(u8 reg)
{
	u8 val ;
	
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
	{
		I2C_GenerateSTOP(I2C1, ENABLE);
	}
	
	I2C_GenerateSTART(I2C1, ENABLE);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
	
	I2C_Send7bitAddress(I2C1, (0x72 ), I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	I2C_SendData(I2C1, ((reg & 0x0F) | 0x80));
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	
	I2C_GenerateSTART(I2C1, ENABLE);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

	I2C_Send7bitAddress(I2C1, (0x72 ), I2C_Direction_Receiver);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
	
	I2C_AcknowledgeConfig(I2C1, DISABLE);
	I2C_GenerateSTOP(I2C1, ENABLE);
	while(!(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)));
	
	val = I2C_ReceiveData(I2C1);
	I2C_AcknowledgeConfig(I2C1, ENABLE);
	
	return val;
}


void tsl2561_write(u8 reg, u8 val)
{
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
	{
		I2C_GenerateSTOP(I2C1, ENABLE);
	}	
	
	I2C_GenerateSTART(I2C1, ENABLE);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

	I2C_Send7bitAddress(I2C1, (0x72 ), I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	I2C_SendData(I2C1, ((reg & 0x0F) | 0x80));
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	I2C_SendData(I2C1, val);
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	I2C_GenerateSTOP(I2C1, ENABLE);
	
}

void tsl2561_set_timing(u8 gain, u8 time)
{
	u8 timing;
	
	timing = tsl2561_read(0x01);
	
	if(gain)
		timing |= 0x10;
	else
		timing &= ~0x10;
	
	timing &= ~0x03;
	timing |= (time & 0x03);
	
	tsl2561_write(0x01, timing);
}

void tsl2561_set_powerup(void)
{
	tsl2561_write(0x00,0x03);
}

u16 tsl2561_get_ch1(void) 
{
	u8 dat_low,dat_high;
	u16 dat;
	
	dat_low = tsl2561_read(0x0E);
	dat_high = tsl2561_read(0x0F);
	
	dat = (dat_high << 8) | dat_low;
	
	return dat;
}
