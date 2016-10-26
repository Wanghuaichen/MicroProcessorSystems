#include "timer.h"
#include "bool.h"

extern int piezo_counter;
bool piezo_tim_flag = 0;
bool seg_tim_flag = 0;

TIM_HandleTypeDef TimerStructPiezo = { 
    .Instance = TIM2 				/*!< Register base address             */
};

TIM_HandleTypeDef TimerStruct7seg = { 
    .Instance = TIM3 				/*!< Register base address             */
};

void timer_init(void) {
	//Piezo 1ms timer init (1kHz)
	__TIM2_CLK_ENABLE(); //enable timer clock
	//TIM2 is 32-bits
	TimerStructPiezo.Init.Prescaler = 9000; /*!< prescaler value used to divide the TIM clock. Can be between Min_Data = 0x0000U and Max_Data = 0xFFFFU */
	TimerStructPiezo.Init.CounterMode = TIM_COUNTERMODE_UP; /*!< Specifies the counter mode. This parameter can be a value of @ref TIM_Counter_Mode */
	TimerStructPiezo.Init.Period = 1;  //sysclk config /*!< Specifies the period value. Can be a number between Min_Data = 0x0000U and Max_Data = 0xFFFF.  */
	TimerStructPiezo.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;  //slows the frequency of the slow by a factor of divisor /*!< Specifies the clock division. This parameter can be a value of @ref TIM_ClockDivision */
	TimerStructPiezo.Init.RepetitionCounter = 0; /*!< Specifies the repetition counter value. Each time the RCR downcounter reaches zero, an update event is generated and counting restarts from the RCR value (N). This means in PWM mode that (N+1) corresponds to:     - the number of PWM periods in edge-aligned mode     - the number of half PWM period in center-aligned mode  This parameter must be a number between Min_Data = 0x00 and Max_Data = 0xFF.   @note This parameter is valid only for TIM1 and TIM8. */
	HAL_TIM_Base_Init(&TimerStructPiezo);
	HAL_TIM_Base_Start_IT(&TimerStructPiezo);
	//7 segment 2ms period for 5 digits is 10ms timer init (100Hz)
	__TIM3_CLK_ENABLE();
	//TIM3 is 16-bits
	//sysclk is 160MHz - TIM3 clk is 168/4 = 42MHz
	//Prescaler counts up before incrementing the counter
	//Prescaler*Period = 42000 - one second
	TimerStruct7seg.Init.Prescaler = 9000; /*!< prescaler value used to divide the TIM clock. Can be between Min_Data = 0x0000U and Max_Data = 0xFFFFU */
	TimerStruct7seg.Init.CounterMode = TIM_COUNTERMODE_UP; /*!< Specifies the counter mode. This parameter can be a value of @ref TIM_Counter_Mode */
	TimerStruct7seg.Init.Period = 1;  /*!< Specifies the period value. Can be a number between Min_Data = 0x0000U and Max_Data = 0xFFFF.  */
	TimerStruct7seg.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;  /*!< Specifies the clock division. This parameter can be a value of @ref TIM_ClockDivision */
	TimerStruct7seg.Init.RepetitionCounter = 0; /*!< Specifies the repetition counter value. Each time the RCR downcounter reaches zero, an update event is generated and counting restarts from the RCR value (N). This means in PWM mode that (N+1) corresponds to:     - the number of PWM periods in edge-aligned mode     - the number of half PWM period in center-aligned mode  This parameter must be a number between Min_Data = 0x00 and Max_Data = 0xFF.   @note This parameter is valid only for TIM1 and TIM8. */
	HAL_TIM_Base_Init(&TimerStruct7seg);
	HAL_TIM_Base_Start_IT(&TimerStruct7seg);

	// enable interupts on timers
	HAL_NVIC_SetPriority(TIM2_IRQn, 2, 0);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);

	HAL_NVIC_SetPriority(TIM3_IRQn, 3, 0);
	HAL_NVIC_EnableIRQ(TIM3_IRQn);
}
//interupt handler
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
