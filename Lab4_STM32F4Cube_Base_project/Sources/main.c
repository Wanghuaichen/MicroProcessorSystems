////////////////////////////////////////////////////////////////////////////////
//	File Name					: main.c
//	Description				: program entry
//	Author						: Tianming Zhang, Alex Bhandari
//	Date							: Nov 6, 2016
////////////////////////////////////////////////////////////////////////////////
	
//		Includes		//
#include <stm32f4xx_hal.h>
#include <supporting_functions.h>
#include <sysclk_config.h>
#include <lis3dsh.h>
#include <arm_math.h>
#include <timer.h>
#include <accelerometer_thread.h>
#include <temperature_thread.h>
#include <display_thread.h>
#include <keypad_thread.h>
#include <cmsis_os.h>

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
	
	// start program threads
	start_accelerometer_thread(NULL);
	//start_temperature_thread(NULL);
	//start_display_thread(NULL);
	//start_keypad_thread(NULL);
	
	osKernelStart();
	osDelay(osWaitForever);

	return 0;
}
