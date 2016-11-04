////////////////////////////////////////////////////////////////////////////////
//	File Name					: main.c
//	Description				: program entry
//	Author						: Harsh Aurora
//	Date							: Oct 28, 2016
////////////////////////////////////////////////////////////////////////////////
	
//		Includes		//
#include <stm32f4xx_hal.h>
#include <supporting_functions.h>
#include <sysclk_config.h>
#include <lis3dsh.h>
#include <arm_math.h>
#include <LED_thread.h>
#include <cmsis_os.h>
#include "main.h"
#include "threads.h"
#include "system_init.h"

//Brief:	main program
//				
//Params:	None
//Return:	None

int main(void) {

	//accel_int_flag = 1;
	//seven_segment_tim_flag = 1;

	//		MCU Configuration		//
	HAL_Init();
	
	//	Configure the system clock	//
	SystemClock_Config();

	//	Configure peripherals //
	accelerometer_init();
	keypad_init();
	timer_init();
	gpio_clk_display_init();
	
//start_LED_thread(NULL);
//osKernelInitialize();
	osKernelStart();
	run_threads();
	osDelay(osWaitForever);

	return 0;
}
