////////////////////////////////////////////////////////////////////////////////
//	File Name					: main.c
//	Description				: program entry
//	Author						: Harsh Aurora
//	Date							: Oct 1st, 2016
////////////////////////////////////////////////////////////////////////////////
	
//		Includes		//
#include "main.h"

volatile int systick_flag;
extern int delay_flag;
volatile int systick_flag; 
extern float piezo_max;
extern float piezo_counter;
extern TIM_HandleTypeDef TimerStructPiezo;
extern TIM_HandleTypeDef TimerStruct7seg;
extern bool keypad_scan_flag;
extern bool piezo_tim_flag;
extern bool seg_tim_flag;
extern bool acc_flag;
unsigned short key;
extern int mem[3];

int main(void) {
	
	system_init();

	while(1) {

		int tp = __HAL_TIM_GET_COUNTER(&TimerStructPiezo);
		int t7 = __HAL_TIM_GET_COUNTER(&TimerStruct7seg);

		//peak and hold for piezo
		if(piezo_tim_flag) {
			piezo_tim_flag = 0;
			piezo_counter++;
			piezo_adc_poll();
			piezo_peak();
		}
		if(seg_tim_flag) {
			seg_tim_flag = 0;
			display(24.6f);
		}
		if(acc_flag) {
			float out[3];
			LIS3DSH_ReadACC(out);
			//printf("x:%f y:%f z:%f\n",out[0],out[1],out[2]);
		}
		if(keypad_scan_flag) {
			key = get_key();
			if(key!=999) {
				if(monitor_for_change((int)key,&mem[1])) {
					printf("Digit: %d\n", key);
					keypad_scan_flag = 0;
				}
			}
		}
		//if(delay_flag) {
		//	HAL_Delay(1000);
		//	delay_flag = 0;
		//}
	}
	return 0;
}

//callback from stm32f4xx_hal_gpio.c
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if(GPIO_Pin == GPIO_PIN_0) {
		acc_flag = 1;
	} 
	else {
	keypad_scan_flag = 1;
	}
}