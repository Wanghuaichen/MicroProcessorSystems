////////////////////////////////////////////////////////////////////////////////
//	File Name					: timer.c
//	Description				: program that set up two timers, os signals and flag
//	Author						: Alex Bhandari, Tianming Zhang
//	Date							: Nov 6, 2016
////////////////////////////////////////////////////////////////////////////////

//include
#include <stm32f4xx_hal.h>
#include <cmsis_os.h>
#include "timer.h"
#include "utils.h"

extern osThreadId temperature_thread_ID;
extern osThreadId display_thread_ID;
extern osThreadId keypad_thread_ID;

//Global
int blink_flag;

TIM_HandleTypeDef TIM3_handle, TIM2_handle;

//Brief: initialize timers
//Params: None
//Return: None
void timer_init(void){
	//initialize TIM3
	__TIM3_CLK_ENABLE();
	TIM3_handle.Instance=TIM3;
	TIM3_handle.Init.Prescaler					= 20999;        
	TIM3_handle.Init.CounterMode				= TIM_COUNTERMODE_DOWN;     
	TIM3_handle.Init.Period							= 3999;           
	TIM3_handle.Init.ClockDivision			= TIM_CLOCKDIVISION_DIV1;    
	TIM3_handle.Init.RepetitionCounter	= 0;
	
	HAL_TIM_Base_Init(&TIM3_handle);
	HAL_TIM_Base_Start_IT(&TIM3_handle);
	
	//initialize TIM2
	__TIM2_CLK_ENABLE();
	TIM2_handle.Instance=TIM2;
	TIM2_handle.Init.Prescaler					= 20999;        
	TIM2_handle.Init.CounterMode				= TIM_COUNTERMODE_DOWN;     
	TIM2_handle.Init.Period							= 10;           
	TIM2_handle.Init.ClockDivision			= TIM_CLOCKDIVISION_DIV1;    
	TIM2_handle.Init.RepetitionCounter	= 0;
	
	HAL_TIM_Base_Init(&TIM2_handle);
	HAL_TIM_Base_Start_IT(&TIM2_handle);
	
	//enable IRQ, and set up NVIC priority
	HAL_NVIC_EnableIRQ(TIM3_IRQn);
	HAL_NVIC_SetPriority(TIM3_IRQn, 2, 0);

	HAL_NVIC_EnableIRQ(TIM2_IRQn);
	HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
}

//Brief: TIM callback function for threads
//Params: TIM Time Base Handle
//Return: None
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim->Instance == TIM2) {
		osSignalSet(display_thread_ID,0x00000001);		
		osSignalSet(keypad_thread_ID,0x00000001);		
	} else if (htim->Instance == TIM3) { 
		osSignalSet(temperature_thread_ID,0x00000001);
		blink_flag = !blink_flag;
	} 
}