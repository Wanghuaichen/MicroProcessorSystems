////////////////////////////////////////////////////////////////////////////////
//	File Name					: main.c
//	Description				: header file for adc_example.h
//	Author						: Harsh Aurora
//	Date							: Oct 1st, 2016
////////////////////////////////////////////////////////////////////////////////

#ifndef _ADC
#define _ADC

#define POLL_TIMEOUT 10000

#define ADC ADC1
#define ADC_CHANNEL ADC_CHANNEL_16
#define ADC_CLK_EN 	__HAL_RCC_ADC1_CLK_ENABLE()

#define ADC_GPIO_PORT GPIOA
#define ADC_GPIO_PIN GPIO_PIN_1 // Pin A5 is connected to the ADC1 Channel 5 input
#define ADC_GPIO_PORT_CLK_EN __HAL_RCC_GPIOA_CLK_ENABLE();

void adc_init(void);
float adc_poll(void);
#endif
