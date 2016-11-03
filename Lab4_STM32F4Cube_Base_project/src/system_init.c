#include "system_init.h"

/*Brief: calls all the init methods
**Params: None
**Return: None
*/
void system_init() {
	//		MCU Configuration		//
	//	Reset of all peripherals, Initializes the Flash interface and the Systick	//
	HAL_Init();

	//	Configure the system clock	//
	SystemClock_Config();

	//	Configure peripherals //
	accelerometer_init();
	keypad_init();
	timer_init();
	gpio_clk_display_init();
}
