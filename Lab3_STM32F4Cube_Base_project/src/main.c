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
	
	int counter=0;
	float piezo_val;
	
	system_init();

	while(1) {
		HAL_Delay(100);
		
		piezo_val=piezo_adc_poll();
		printf("piezo value is %f\n", piezo_val);
	
		}
		
	}
//}