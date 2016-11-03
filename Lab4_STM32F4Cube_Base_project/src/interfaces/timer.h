#ifndef _TIMER
#define _TIMER

#include <stm32f4xx_hal.h>

extern TIM_HandleTypeDef TimerStructPiezo;
extern TIM_HandleTypeDef TimerStruct7seg;

void timer_init(void);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

#endif
