#ifndef _TIMER
#define _TIMER

#include <stm32f4xx_hal.h>
#include "piezo_adc.h"

void timer_init(void);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

#endif
