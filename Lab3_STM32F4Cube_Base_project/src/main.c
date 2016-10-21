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
	
	int counter =0;
	float piezo_val = 0;
	float max = 0;
	
	system_init();

	while(1) {
		//HAL_Delay(100);
		
		//peak and hold thingy
		piezo_val = piezo_adc_poll();
		counter++;
		if(piezo_val>max){
			max = piezo_val;
		}
		if (counter==1000){
			printf("piezo value is %f\n", max);
			max=0;
			counter=0;
		}
		//float out[3];
		//LIS3DSH_ReadACC(out);
		//printf("x:%f y:%f z:%f\n",out[0],out[1],out[2]);
	}
	return 0;
}
