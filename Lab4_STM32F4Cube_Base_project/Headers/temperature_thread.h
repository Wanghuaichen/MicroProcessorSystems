#ifndef _TEMPERATURE_THREAD
#define _TEMPERATURE_THREAD

#define POLL_TIMEOUT 10000

#define ADC ADC1
#define ADC_CHANNEL ADC_CHANNEL_16
#define ADC_CLK_EN 	__HAL_RCC_ADC1_CLK_ENABLE()

#define ADC_GPIO_PORT GPIOA
#define ADC_GPIO_PIN GPIO_PIN_1 // Pin A5 is connected to the ADC1 Channel 5 input
#define ADC_GPIO_PORT_CLK_EN __HAL_RCC_GPIOA_CLK_ENABLE();

#include <cmsis_os.h>

extern float temp_data;
extern osSemaphoreId sem_temp;

void temperature_thread(void const *args);
void temperature_init(void);
void start_temperature_thread(void *args);
float temperature_poll(void);

#endif
