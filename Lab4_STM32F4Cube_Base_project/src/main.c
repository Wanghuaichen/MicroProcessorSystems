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
#include "theads.h"

//Brief:	main program
//				
//Params:	None
//Return:	None
int main(void) {

	//		MCU Configuration		//
	//	Reset of all peripherals, Initializes the Flash interface and the Systick	//
	HAL_Init();	

	//	Configure the system clock	//
	SystemClock_Config();

	threads_init();
	
	start_LED_thread(NULL);
	osKernelStart();
	run_threads();
	osDelay(osWaitForever);


	return 0;
}