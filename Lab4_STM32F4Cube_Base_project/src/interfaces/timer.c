#include "timer.h"
#include "utils/utils.h"
#include <cmsis_os.h>

//import extern data variables
#include "threads.h"
#include "LED_thread.h"

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
	//360/4 = 90Hz
	TimerStructPiezo.Init.Prescaler = 20999;
	TimerStructPiezo.Init.CounterMode = TIM_COUNTERMODE_UP;
	TimerStructPiezo.Init.Period = 10;
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
	TimerStruct7seg.Init.Prescaler = 20999;
	TimerStruct7seg.Init.CounterMode = TIM_COUNTERMODE_UP;
	TimerStruct7seg.Init.Period = 3999;
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
	if(htim->Instance == TIM2) { //7 segment refresh
		osSignalSet(thread3, 0x00000001); //temp
		osSignalSet(thread5, 0x00000001); //7 seg
		//seven_segment_tim_flag = 1;
		//printf("tick\n");
	} 
	else if(htim->Instance == TIM3) { //Keypad poll
		//keypad_tim_flag = 1;
		osSignalSet(LED_thread_ID, 0x00000001); //LED
		osSignalSet(thread2, 0x00000001); //keypad
		osSignalSet(thread4, 0x00000001); //led
		//printf("tock\n");
	} 
	else {
		//printf("timer::HAL_TIM_PeriodElapsedCallback: Timer interupt triggered that is not accounted for.\n");
	}
}
