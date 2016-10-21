#ifndef _PIEZO_ADC
#define _PIEZO_ADC

#define POLL_TIMEOUT 10000

#define ADC ADC1
#define ADC_CHANNEL ADC_CHANNEL_1
#define ADC_CLK_EN 	__HAL_RCC_ADC1_CLK_ENABLE()

#define ADC_GPIO_PORT GPIOA
#define ADC_GPIO_PIN GPIO_PIN_1 // Pin A1 is connected to the ADC1 Channel 1 input
#define ADC_GPIO_PORT_CLK_EN __HAL_RCC_GPIOA_CLK_ENABLE();

void piezo_adc_init(void);
float piezo_adc_poll(void);
#endif
