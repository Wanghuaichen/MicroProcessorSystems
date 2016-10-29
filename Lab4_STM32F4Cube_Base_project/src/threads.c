#include "threads.h"
#include "modules/accelerometer.h"
#include "modules/keypad.h"
#include "modules/temperature.h"
#include "modules/led.h"
#include "modules/seven_segment.h"
#include <cmsis_os.h>

osThreadId thread1;
osThreadId thread2;
osThreadId thread3;
osThreadId thread4;
osThreadId thread5;

/*Brief: Get normalized accelerometer data and write to accel_data variable.
**Params: None
**Return: None
*/
void threads_init(void) {
	osThreadDef(thread1, osPriorityNormal, 1, 0);
	osThreadDef(thread2, osPriorityNormal, 1, 0);
	osThreadDef(thread3, osPriorityNormal, 1, 0);
	osThreadDef(thread4, osPriorityNormal, 1, 0);
	osThreadDef(thread5, osPriorityNormal, 1, 0);
}

/*Brief: Get normalized accelerometer data and write to accel_data variable.
**Params: None
**Return: None
*/
void run_threads(void) {
	thread1 = osThreadCreate(osThread(accelerometer_thread), NULL);
	thread2 = osThreadCreate(osThread(keypad_thread), NULL);
	thread3 = osThreadCreate(osThread(temperature_thread), NULL);
	thread4 = osThreadCreate(osThread(led_thread), NULL);
	thread5 = osThreadCreate(osThread(seven_segment_thread), NULL);
}

/*Brief: Get normalized accelerometer data and write to accel_data variable.
**Params: None
**Return: None
*/
void accelerometer_thread(void const* args) {
	while(1) {
		if(accel_int_flag) {
			accel_get_data();
		}
	}
}
/*Brief: Get key on timer flag high. If no key is pressed the value of key_data will not be updated.
**Params: None
**Return: None
*/
void keypad_thread(void const* args) {
	while(1) {
		if(keypad_tim_flag) {
			keypad_get_key();
		}
	}
}
/*Brief: Get filtered temperature data and write to temp_data variable.
**Params: None
**Return: None
*/
void temperature_thread(void const* args) {
	while(1) {
		if(temp_tim_flag) {
			temp_get_data();
		}
	}
}
/*Brief: Twirl the led every tim flag.
**Params: None
**Return: None
*/
void led_thread(void const* args) {
	while(1) {
		if(led_tim_flag) {
			led_twirl_refresh();
		}
	}
}
/*Brief: Refresh the display every tim flag.
**Params: None
**Return: None
*/
void seven_segment_thread(void const* args) {
	while(1) {
		if(seven_segment_tim_flag) {
			seven_segment_refresh();
		}
	}
}