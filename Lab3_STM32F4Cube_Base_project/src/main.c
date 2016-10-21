////////////////////////////////////////////////////////////////////////////////
//	File Name					: main.c
//	Description				: program entry
//	Author						: Harsh Aurora
//	Date							: Oct 1st, 2016
////////////////////////////////////////////////////////////////////////////////
	
//		Includes		//
#include "main.h"

volatile int systick_flag; 

int main(void) {
	
	float piezo_val;
	
	system_init();

	while(1) {
		HAL_Delay(100);
		
		piezo_val=piezo_adc_poll();
		printf("piezo value is %f\n", piezo_val);

		float out[3];
		LIS3DSH_ReadACC(out);
		printf("x:%f y:%f z:%f\n",out[0],out[1],out[2]);
	}
	return 0;
}
