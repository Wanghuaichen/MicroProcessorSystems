#include "timer.h"
#include "utils/utils.h"

extern int piezo_counter;
bool piezo_tim_flag = 0;
bool seg_tim_flag = 0;

TIM_HandleTypeDef TimerStructPiezo = { 
    .Instance = TIM2 				/*!< Register base address             */
};

TIM_HandleTypeDef TimerStruct7seg = { 
    .Instance = TIM3 				/*!< Register base address             */
};

/*Brief: Inits 2 timers for the piezo and the 7seg
**Params: None
**Return: None
*/
void timer_init(void) {
	//Piezo 1ms timer init (1kHz)
	__TIM2_CLK_ENABLE(); //enable timer clock
	//TIM2 is 32-bits
	TimerStructPiezo.Init.Prescaler = 9000;
	TimerStructPiezo.Init.CounterMode = TIM_COUNTERMODE_UP;
	TimerStructPiezo.Init.Period = 1;
	TimerStructPiezo.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	TimerStructPiezo.Init.RepetitionCounter = 0;
	HAL_TIM_Base_Init(&TimerStructPiezo);
	HAL_TIM_Base_Start_IT(&TimerStructPiezo);
	//7 segment 2ms period for 5 digits is 10ms timer init (100Hz)
	__TIM3_CLK_ENABLE();
	//TIM3 is 16-bits
	//sysclk is 160MHz - TIM3 clk is 168/4 = 42MHz
	//Prescaler counts up before incrementing the counter
	//Prescaler*Period = 42000 - one second
	TimerStruct7seg.Init.Prescaler = 9000;
	TimerStruct7seg.Init.CounterMode = TIM_COUNTERMODE_UP;
	TimerStruct7seg.Init.Period = 1;
	TimerStruct7seg.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	TimerStruct7seg.Init.RepetitionCounter = 0;
	HAL_TIM_Base_Init(&TimerStruct7seg);
	HAL_TIM_Base_Start_IT(&TimerStruct7seg);

	// enable interupts on timers
	HAL_NVIC_SetPriority(TIM2_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);

	HAL_NVIC_SetPriority(TIM3_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ(TIM3_IRQn);
}

/*Brief: Timer interupt handler
**Params: None
**Return: None
*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM2) { //Update the peak value of the piezo
		piezo_tim_flag = 1;
	} 
	else if(htim->Instance == TIM3) { //7 segment refresh
		seg_tim_flag = 1;
	} 
	else {
		printf("wut");
	}
}
