#include "adc.h"

u16 adc_buff[9];
u16 adc_buff_max[9];

void adc_init(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA |RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;
	
	ADC_CommonInit(&ADC_CommonInitStructure);
	
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;		//ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 9;

	ADC_Init(ADC1, &ADC_InitStructure);
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0 ,1 , ADC_SampleTime_480Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4 ,2 , ADC_SampleTime_480Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5 ,3 , ADC_SampleTime_480Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10 ,4 , ADC_SampleTime_480Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_11 ,5 , ADC_SampleTime_480Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_12 ,6 , ADC_SampleTime_480Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_13 ,7 , ADC_SampleTime_480Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_14 ,8 , ADC_SampleTime_480Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_15 ,9 , ADC_SampleTime_480Cycles);
	
	ADC_Cmd(ADC1, ENABLE);
	//ADC_SoftwareStartConv(ADC1);
	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
	ADC_DMACmd(ADC1, ENABLE);

}


void dma_init(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	while(DMA_GetCmdStatus(DMA2_Stream0) != DISABLE)
		;
	
	DMA_InitStructure.DMA_Channel = DMA_Channel_0;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->DR;
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)adc_buff;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = 9;															//9*100;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;										//DMA_Mode_Circular;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	
	DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, ENABLE);
	DMA_Init(DMA2_Stream0, &DMA_InitStructure);
	DMA_Cmd(DMA2_Stream0, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;
	NVIC_Init(&NVIC_InitStructure);
	
}

void DMA2_Stream0_IRQHandler(void)
{
	u8 i ;
	
	if(DMA_GetITStatus(DMA2_Stream0, DMA_IT_TCIF0))
	{
		DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TCIF0);
		DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_HTIF0);
		
		DMA_Cmd(DMA2_Stream0, DISABLE);
		DMA_SetCurrDataCounter(DMA2_Stream0, 9);
		DMA_Cmd(DMA2_Stream0, ENABLE);
		
		for( i = 0; i < 9; i++ )
		{
			if(adc_buff[i] > adc_buff_max[i])
				adc_buff_max[i] = adc_buff[i];
		}
		
	}
}


