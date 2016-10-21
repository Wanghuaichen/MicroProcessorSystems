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

int main(void) {
	
	system_init();

	while(1) {

		int tp = __HAL_TIM_GET_COUNTER(&TimerStructPiezo);
		int t7 = __HAL_TIM_GET_COUNTER(&TimerStruct7seg);

		//peak and hold for piezo
		if(tp == 0) {
			piezo_adc_poll();
			piezo_peak();
		}
		if(t7 == 0) {
			display(24.6f);
		}
		float out[3];
		LIS3DSH_ReadACC(out);
		//printf("x:%f y:%f z:%f\n",out[0],out[1],out[2]);
		
		keypad_scan();
		if(delay_flag) {
			HAL_Delay(500);
			delay_flag = 0;
		}
	}
	return 0;
}
