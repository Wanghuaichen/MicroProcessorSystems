#include <system_init.h>
#include <piezo_adc.h>


void system_init() {
	//		MCU Configuration		//
	//	Reset of all peripherals, Initializes the Flash interface and the Systick	//
	HAL_Init();
	
	//	Configure the system clock	//
	SystemClock_Config();
	
	//piezo initializatin
	piezo_adc_init();
}