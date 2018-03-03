#include "DEV_ADC.h"
#include "Timing_Manage.h"
#include "Serial_Link.h"

uint16_t ADCBUFF[ADC_BuffSize];


 
void ADC_Iinitialize(void)
{
	ADC_InitTypeDef       ADC_InitStruct;
	ADC_CommonInitTypeDef ADC_CommonInitStruct;
	DMA_InitTypeDef       DMA_InitStruct;
	GPIO_InitTypeDef      GPIO_InitStruct;

	/* Enable ADC3, DMA2 and GPIO clocks ****************************************/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2 | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);//ADC3 is connected to the APB2 peripheral bus

	/* DMA2 Stream0 channel0 configuration **************************************/
	DMA_InitStruct.DMA_Channel 									= DMA_Channel_2;
	DMA_InitStruct.DMA_PeripheralBaseAddr 			= (uint32_t)&ADCX->DR;//ADC3's data register
	DMA_InitStruct.DMA_Memory0BaseAddr 					= (uint32_t)&ADCBUFF;
	DMA_InitStruct.DMA_DIR 											= DMA_DIR_PeripheralToMemory;
	DMA_InitStruct.DMA_BufferSize 							= ADC_BuffSize;
	DMA_InitStruct.DMA_PeripheralInc 						= DMA_PeripheralInc_Disable;
	DMA_InitStruct.DMA_MemoryInc 								= DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_PeripheralDataSize 			= DMA_PeripheralDataSize_HalfWord;//Reads 16 bit values
	DMA_InitStruct.DMA_MemoryDataSize 					= DMA_MemoryDataSize_HalfWord;//Stores 16 bit values
	DMA_InitStruct.DMA_Mode 										= DMA_Mode_Circular;
	DMA_InitStruct.DMA_Priority 								= DMA_Priority_High;
	DMA_InitStruct.DMA_FIFOMode 								= DMA_FIFOMode_Disable;
	DMA_InitStruct.DMA_FIFOThreshold 						= DMA_FIFOThreshold_HalfFull;
	DMA_InitStruct.DMA_MemoryBurst 							= DMA_MemoryBurst_Single;
	DMA_InitStruct.DMA_PeripheralBurst 					= DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream0, &DMA_InitStruct);
	DMA_Cmd(DMA2_Stream0, ENABLE);

	/* Configure GPIO pins ******************************************************/
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;// PC0, PC1, PC2, PC3
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;//The pins are configured in analog mode
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL ;//We don't need any pull up or pull down
	GPIO_Init(GPIOC, &GPIO_InitStruct);//Initialize GPIOC pins with the configuration
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;//PA1
	GPIO_Init(GPIOA, &GPIO_InitStruct);//Initialize GPIOA pins with the configuration

	/* ADC Common Init **********************************************************/
	ADC_CommonInitStruct.ADC_Mode 						= ADC_Mode_Independent;
	ADC_CommonInitStruct.ADC_Prescaler 				= ADC_Prescaler_Div8;
	ADC_CommonInitStruct.ADC_DMAAccessMode 		= ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;
	ADC_CommonInit(&ADC_CommonInitStruct);

	/* ADC3 Init ****************************************************************/
	ADC_DeInit();
	ADC_InitStruct.ADC_Resolution							= ADC_Resolution_12b;
	ADC_InitStruct.ADC_ScanConvMode 					= ENABLE;
	ADC_InitStruct.ADC_ContinuousConvMode			= ENABLE;
	ADC_InitStruct.ADC_ExternalTrigConv				= 0;
	ADC_InitStruct.ADC_ExternalTrigConvEdge 	= ADC_ExternalTrigConvEdge_None;
	ADC_InitStruct.ADC_DataAlign 							= ADC_DataAlign_Right;
	ADC_InitStruct.ADC_NbrOfConversion 				= ADC_CoversionSize;
	ADC_Init(ADC3, &ADC_InitStruct);

	/* Select the channels to be read from **************************************/
	ADC_RegularChannelConfig(ADCX, ADC_Channel_10, 1, ADC_SampleTime_480Cycles);//PC0
	ADC_RegularChannelConfig(ADCX, ADC_Channel_11, 2, ADC_SampleTime_480Cycles);//PC1
	ADC_RegularChannelConfig(ADCX, ADC_Channel_12, 3, ADC_SampleTime_480Cycles);//PC2
	ADC_RegularChannelConfig(ADCX, ADC_Channel_13, 4, ADC_SampleTime_480Cycles);//PC3
	ADC_RegularChannelConfig(ADCX, ADC_Channel_1,  5, ADC_SampleTime_480Cycles);//PA1

	/* Enable DMA request after last transfer (Single-ADC mode) */
	ADC_DMARequestAfterLastTransferCmd(ADCX, ENABLE);

	/* Enable ADC3 DMA */
	ADC_DMACmd(ADCX, ENABLE);

	/* Enable ADC3 */
	ADC_Cmd(ADCX, ENABLE);

	/* Start ADC3 Software Conversion */
	ADC_SoftwareStartConv(ADCX);
}

uint16_t ADC_Get(uint8_t INDEX, uint8_t size)
{
	uint8_t cnt;
	uint16_t Result = 0;
	
	if (size> ADC_CoversionSize)	size = ADC_CoversionSize;
	 for (cnt = 0; cnt < size; cnt++)
		 Result += ADCBUFF[size * cnt + INDEX];
		 
	return 	Result/size;
}
