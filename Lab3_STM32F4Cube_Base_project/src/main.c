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

int main(void) {
	
	system_init();

	while(1) {
		
		
		//peak and hold thingy
		//printf("piezo value is %f\n", piezo_peak());

		//float out[3];
		//LIS3DSH_ReadACC(out);
		//printf("x:%f y:%f z:%f\n",out[0],out[1],out[2]);
		
		keypad_scan();
		if(delay_flag) {
			HAL_Delay(500);
			delay_flag = 0;
		}
	}
	return 0;
}
