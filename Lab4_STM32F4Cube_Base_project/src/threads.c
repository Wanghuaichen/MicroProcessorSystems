#include "threads.h"
#include "modules/accelerometer.h"
#include "modules/keypad.h"
#include "modules/temperature.h"
#include "modules/led.h"
#include "modules/seven_segment.h"
#include <cmsis_os.h>
#include <stm32f4xx_hal.h>

bool accel_int_flag = false;
bool keypad_tim_flag = false;
bool temp_tim_flag = false;
bool led_tim_flag = false;
bool seven_segment_tim_flag = false;

osThreadId thread1;
osThreadId thread2;
osThreadId thread3;
osThreadId thread4;
osThreadId thread5;

/*Brief: Assign threads to their respective methods. Then run the threads.
**Params: None
**Return: None
*/
void run_threads(void) {
	
	osThreadDef(accelerometer_thread, osPriorityNormal, 1,0);
	osThreadDef(keypad_thread, osPriorityNormal, 1,0);
	osThreadDef(temperature_thread, osPriorityNormal, 1,0);
	osThreadDef(led_thread, osPriorityNormal, 1,0);
	osThreadDef(seven_segment_thread, osPriorityNormal, 1,0);
	
	thread1 = osThreadCreate(osThread(accelerometer_thread), NULL);
	thread2 = osThreadCreate(osThread(keypad_thread), NULL);
	thread3 = osThreadCreate(osThread(temperature_thread), NULL);
	thread4 = osThreadCreate(osThread(led_thread), NULL);
	thread5 = osThreadCreate(osThread(seven_segment_thread), NULL);
	
	if(thread1==NULL || thread2==NULL || thread3==NULL || thread4==NULL || thread5==NULL) {
		//Failed to create thread
		printf("thread init failed\n");
	}
}

void stop_threads(void) {
	osThreadTerminate(thread1);
	osThreadTerminate(thread2);
	osThreadTerminate(thread3);
	osThreadTerminate(thread4);
	osThreadTerminate(thread5);
}

/*Brief: Get normalized accelerometer data and write to accel_data variable.
**Params: None
**Return: None
*/
void accelerometer_thread(void const* args) {
	while(1) {
		if(accel_int_flag) {
			accel_int_flag = false;
			accel_get_data();
			printf("accel\n");
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
			keypad_tim_flag = false;
			keypad_get_key();
			printf("keypad\n");
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
			temp_tim_flag = false;
			temp_get_data();
			printf("temp\n");
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
			led_tim_flag = false;
			led_twirl_refresh();
			printf("led\n");
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
			//seven_segment_tim_flag = false;
			seven_segment_refresh();
			printf("7 seg\n");
		}
	}
}
