#include "stm32f4xx_conf.h"
extern uint8_t shutdown;

				////////////////////////////////////////////DMA 1 Interupt is used////////////////////////////////////////
			 //  Stream0  //  Stream1  //  Stream2  //  Stream3  //  Stream4  //  Stream5  //  Stream6  //  Stream7  //                                      
			//////////////////////////////////////////////////////////////////////////////////////////////////////////
		 // Channel1  //  Channel4 //  Channel7 //  Channel4 // 				  //  Channel1 //  Channel1 //  Channel7 //                                        
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
	 // I2C1-RX   //  UART3-RX //  I2C2-RX  //  UART3-TX // 				  //  UART1-RX //  I2C1-TX	//  I2C2-TX  //                                     
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

			////////////////////////////////////////////DMA 1 Interupt is used////////////////////////////////////////
		 //  Stream0  //  Stream1  //  Stream2  //  Stream3  //  Stream4  //  Stream5  //  Stream6  //  Stream7  //                                      
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
	 //           // 	 ADC3	 	 //           //           //           //           //           //           //                                      
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
 //           // 	Channel4 //           //           //           //           //           //           //                                        
//////////////////////////////////////////////////////////////////////////////////////////////////////////


#define NVIC_GROUP_DEFAULT 													NVIC_PriorityGroup_3		//3 bit pre-emption


#define NVIC_Prepriority_systick 						6
#define NVIC_Subpriority_systick 						0

#define NVIC_Prepriority_SerialLink					7
#define NVIC_Subpriority_SerialLink 				0

#define NVIC_Prepriority_GPS		 						0
#define NVIC_Subpriority_GPS 								0

#define NVIC_Prepriority_SENSOR 						4
#define NVIC_Subpriority_SENSOR 						0

#define NVIC_Prepriority_SDIO		 						3
#define NVIC_Subpriority_SDIO		 						0

#define NVIC_Prepriority_EPPROM	 						2
#define NVIC_Subpriority_EPPROM 						0

  /////////////////////////////////////////////////////////////////
 //										ADC_Voltage_Feedback											//	
/////////////////////////////////////////////////////////////////
#define VoltFB_GPIO_PORT						GPIOC
#define VoltFB_GPIO_PIN 						GPIO_Pin_0
#define VoltFB_GPIO_CLK  						RCC_AHB1Periph_GPIOC

#define VoltFB_ADC  					  		ADC1
#define VoltFB_ADC_CLK  						RCC_APB2Periph_ADC1
#define VoltFB_ADC_CHANNEL  				ADC_Channel_10

/////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////
 //														USART1														//									
/////////////////////////////////////////////////////////////////
#define UART1_GPIO_PORT					GPIOB
#define UART1_GPIO_CLK					RCC_AHB1Periph_GPIOB

#define UART1_IRQn							USART1_IRQn
#define UART1_IT								USART1_IRQHandler

#define UART1_TX_PIN						GPIO_Pin_6
#define UART1_TX_SOURCE					GPIO_PinSource6
#define UART1_RX_PIN						GPIO_Pin_7
#define UART1_RX_SOURCE					GPIO_PinSource7

#define UART1_TX_DMA_CH					DMA_Channel_4
#define UART1_TX_DMA_ST					DMA2_Stream7
#define UART1_TX_DMA_IRQn				DMA2_Stream7_IRQn
#define UART1_TX_DMA_FLAG				DMA_IT_TCIF7
#define UART1_TX_DMA_IT					DMA2_Stream7_IRQHandler

#define UART1_RX_DMA_CH					DMA_Channel_4
#define UART1_RX_DMA_ST					DMA2_Stream2
#define UART1_RX_DMA_IRQn				DMA2_Stream2_IRQn
#define UART1_RX_DMA_FLAG				DMA_IT_TCIF2
#define UART1_RX_DMA_IT					DMA2_Stream2_IRQHandler
/////////////////////////////////////////////////////////////////
									
  /////////////////////////////////////////////////////////////////
 //														USART3														//									
/////////////////////////////////////////////////////////////////
#define UART3_GPIO_PORT					GPIOD
#define UART3_GPIO_CLK					RCC_AHB1Periph_GPIOD

#define UART3_IRQn							USART3_IRQn
#define UART3_IT								USART3_IRQHandler

#define UART3_RX_PIN						GPIO_Pin_9
#define UART3_RX_SOURCE					GPIO_PinSource9
#define UART3_TX_PIN						GPIO_Pin_8
#define UART3_TX_SOURCE					GPIO_PinSource8

#define UART3_RX_DMA_CH					DMA_Channel_4
#define UART3_RX_DMA_ST					DMA1_Stream1
#define UART3_RX_DMA_IRQn				DMA1_Stream1_IRQn
#define UART3_RX_DMA_FLAG				DMA_IT_TCIF1
#define UART3_RX_DMA_IT					DMA1_Stream1_IRQHandler

#define UART3_TX_DMA_CH					DMA_Channel_4
#define UART3_TX_DMA_ST					DMA1_Stream3
#define UART3_TX_DMA_IRQn				DMA1_Stream3_IRQn
#define UART3_TX_DMA_FLAG				DMA_IT_TCIF3
#define UART3_TX_DMA_IT					DMA1_Stream3_IRQHandler


#define HC05_KEY_PORT						GPIOD
#define HC05_KEY_PIN						GPIO_Pin_10
#define HC05_KEY_CLK						RCC_AHB1Periph_GPIOD
/////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////
 //														I2C1															//									
/////////////////////////////////////////////////////////////////
#define I2C1_GPIO_PORT					GPIOB
#define I2C1_GPIO_CLK						RCC_AHB1Periph_GPIOB
#define I2C1_AF									GPIO_AF_I2C1

#define I2C1_SCL_PIN						GPIO_Pin_8
#define I2C1_SCL_SOURCE					GPIO_PinSource8
#define I2C1_SDA_PIN						GPIO_Pin_9
#define I2C1_SDA_SOURCE					GPIO_PinSource9

#define I2C1_RX_DMA_CH					DMA_Channel_1
#define I2C1_RX_DMA_ST					DMA1_Stream0
#define I2C1_RX_DMA_IRQn				DMA1_Stream0_IRQn
#define I2C1_RX_DMA_FLAG				DMA_IT_TCIF0
#define I2C1_RX_DMA_IT					DMA1_Stream0_IRQHandler

#define I2C1_TX_DMA_CH					DMA_Channel_1
#define I2C1_TX_DMA_ST					DMA1_Stream6
#define I2C1_TX_DMA_IRQn				DMA1_Stream6_IRQn
#define I2C1_TX_DMA_FLAG				DMA_IT_TCIF6
#define I2C1_TX_DMA_IT					DMA1_Stream6_IRQHandler
/////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////
 //														I2C2															//									
/////////////////////////////////////////////////////////////////
#define I2C2_GPIO_PORT					GPIOB
#define I2C2_GPIO_CLK						RCC_AHB1Periph_GPIOB
#define I2C2_AF									GPIO_AF_I2C2

#define I2C2_SCL_PIN						GPIO_Pin_10
#define I2C2_SCL_SOURCE					GPIO_PinSource10
#define I2C2_SDA_PIN						GPIO_Pin_11
#define I2C2_SDA_SOURCE					GPIO_PinSource11

#define I2C2_RX_DMA_CH					DMA_Channel_7
#define I2C2_RX_DMA_ST					DMA1_Stream2
#define I2C2_RX_DMA_IRQn				DMA1_Stream2_IRQn
#define I2C2_RX_DMA_FLAG				DMA_IT_TCIF2
#define I2C2_RX_DMA_IT					DMA1_Stream2_IRQHandler

#define I2C2_TX_DMA_CH					DMA_Channel_7
#define I2C2_TX_DMA_ST					DMA1_Stream7
#define I2C2_TX_DMA_IRQn				DMA1_Stream7_IRQn
#define I2C2_TX_DMA_FLAG				DMA_IT_TCIF7
#define I2C2_TX_DMA_IT					DMA1_Stream7_IRQHandler
/////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////
 //												RGB_LOOP			 												//									
/////////////////////////////////////////////////////////////////
#define RED_PORT									GPIOA
#define RED_PIN										GPIO_Pin_1
#define RED_CLK										RCC_AHB1Periph_GPIOA
#define RED_PinSource							GPIO_PinSource1
#define RED_GPIO_AF								GPIO_AF_TIM5
#define RED_PERIOD_UPDATE					TIM_SetCompare2

#define GREEN_PORT								GPIOA
#define GREEN_PIN									GPIO_Pin_2
#define GREEN_CLK									RCC_AHB1Periph_GPIOA 
#define GREEN_PinSource						GPIO_PinSource2
#define GREEN_GPIO_AF							GPIO_AF_TIM5
#define GREEN_PERIOD_UPDATE				TIM_SetCompare3

#define BLUE_PORT									GPIOA
#define BLUE_PIN									GPIO_Pin_0
#define BLUE_CLK									RCC_AHB1Periph_GPIOA
#define BLUE_PinSource						GPIO_PinSource0
#define BLUE_GPIO_AF							GPIO_AF_TIM5
#define BLUE_PERIOD_UPDATE				TIM_SetCompare1

#define RGB_PORT									GPIOA
#define RGB_PIN 				    			BLUE_PIN | RED_PIN | GREEN_PIN
#define RGB_GPIO_CLK							RCC_AHB1Periph_GPIOA
#define RGB_TIM										TIM5
/////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////
 //														BUTTONs	  												//									
/////////////////////////////////////////////////////////////////
#define Button_WHITE_PORT									GPIOD
#define Button_WHITE_PIN 					  	  	GPIO_Pin_6
#define Button_WHITE_CLK  					  	  RCC_AHB1Periph_GPIOD
#define Button_WHITE_Pin_source 					GPIO_PinSource6
#define Button_WHITE_Port_source					EXTI_PortSourceGPIOD
#define Button_WHITE_EXTI_Line						EXTI_Line6
#define Button_WHITE_IRQChannel						EXTI9_5_IRQn	
												
#define Button_RED_PORT								GPIOD
#define Button_RED_PIN								GPIO_Pin_7
#define Button_RED_CLK								RCC_AHB1Periph_GPIOD
#define Button_RED_Pin_source 				GPIO_PinSource7
#define Button_RED_Port_source				EXTI_PortSourceGPIOD
#define Button_RED_EXTI_Line					EXTI_Line7
#define Button_RED_IRQChannel					EXTI9_5_IRQn

#define Button_Interupt  					EXTI9_5_IRQHandler
/////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////
 //														SD CARD														//									
/////////////////////////////////////////////////////////////////
#define SDIO_CLK								RCC_APB2Periph_SDIO

#define SDIO_GPIO_DETECT_PORT		GPIOB
#define SDIO_GPIO_DETECT_CLK		RCC_AHB1Periph_GPIOB
#define SDIO_GPIO_DETECT_PIN		GPIO_Pin_15

#define SDIO_GPIO_D0_PORT				GPIOC
#define SDIO_GPIO_D0_CLK				RCC_AHB1Periph_GPIOC
#define SDIO_GPIO_D0_PIN				GPIO_Pin_8
#define SDIO_GPIO_D0_SOURCE			GPIO_PinSource8

#define SDIO_GPIO_D1_PORT				GPIOC
#define SDIO_GPIO_D1_CLK				RCC_AHB1Periph_GPIOC
#define SDIO_GPIO_D1_PIN				GPIO_Pin_9
#define SDIO_GPIO_D1_SOURCE			GPIO_PinSource9

#define SDIO_GPIO_D2_PORT				GPIOC
#define SDIO_GPIO_D2_CLK				RCC_AHB1Periph_GPIOC
#define SDIO_GPIO_D2_PIN				GPIO_Pin_10
#define SDIO_GPIO_D2_SOURCE			GPIO_PinSource10

#define SDIO_GPIO_D3_PORT				GPIOC
#define SDIO_GPIO_D3_CLK				RCC_AHB1Periph_GPIOC
#define SDIO_GPIO_D3_PIN				GPIO_Pin_11
#define SDIO_GPIO_D3_SOURCE			GPIO_PinSource11

#define SDIO_GPIO_CLK_PORT			GPIOC
#define SDIO_GPIO_CLK_CLK				RCC_AHB1Periph_GPIOC
#define SDIO_GPIO_CLK_PIN				GPIO_Pin_12
#define SDIO_GPIO_CLK_SOURCE		GPIO_PinSource12

#define SDIO_GPIO_CMD_PORT			GPIOD
#define SDIO_GPIO_CMD_CLK				RCC_AHB1Periph_GPIOD
#define SDIO_GPIO_CMD_PIN				GPIO_Pin_2
#define SDIO_GPIO_CMD_SOURCE		GPIO_PinSource2

#define SDIO_GPIO_DATA_PORT			GPIOC
#define SDIO_GPIO_DATA_CLK			RCC_AHB1Periph_GPIOC
#define SDIO_GPIO_DATA_PIN			SDIO_GPIO_D0_PIN|SDIO_GPIO_D1_PIN|SDIO_GPIO_D2_PIN|SDIO_GPIO_D3_PIN
#define SDIO_GPIO_DATA_SOURCE		GPIO_PinSource11


#define SDIO_DMA                   DMA2
#define SDIO_DMA_CLK               RCC_AHB1Periph_DMA2
#define SDIO_DMA_STREAM            DMA2_Stream3
#define SDIO_DMA_CHANNEL           DMA_Channel_4
#define SDIO_DMA_FLAG_FEIF         DMA_FLAG_FEIF3
#define SDIO_DMA_FLAG_DMEIF        DMA_FLAG_DMEIF3
#define SDIO_DMA_FLAG_TEIF         DMA_FLAG_TEIF3
#define SDIO_DMA_FLAG_HTIF         DMA_FLAG_HTIF3
#define SDIO_DMA_FLAG_TCIF         DMA_FLAG_TCIF3
#define SDIO_DMA_IRQn              DMA2_Stream3_IRQn
#define SDIO_DMA_IRQHANDLER        DMA2_Stream3_IRQHandler
/////////////////////////////////////////////////////////////////

	
