////////////////////////////////////////////////////////////////////////////////
//	File Name					: main.c
//	Description				: program entry
//	Author						: Harsh Aurora
//	Date							: Oct 1st, 2016
////////////////////////////////////////////////////////////////////////////////
	
//		Includes		//
#include "main.h"
#include "utils.h"

volatile int systick_flag;
extern int delay_flag;
volatile int systick_flag; 
extern float piezo_max;
extern TIM_HandleTypeDef TimerStructPiezo;
extern TIM_HandleTypeDef TimerStruct7seg;
extern bool keypad_scan_flag;
extern bool piezo_tim_flag;
extern bool seg_tim_flag;
extern bool acc_flag;
extern unsigned short key;

int main(void) {
	
	float out[3];
  float pitch, roll;
	
	system_init();
	printf("hi");

	while(1) {

		//peak and hold for piezo
		if(piezo_tim_flag) {
			piezo_tim_flag = 0;
			piezo_adc_poll();
			piezo_peak_update();
			//if(monitor_for_change(piezo_peak(),&mem[MEM_PIEZO])) printf("piezo: %f \n",piezo_peak());
			//printf("piezo: %f \n",piezo_peak());
		}
		
		//accelerameter
		if(acc_flag) {
			LIS3DSH_ReadACC(out);
			pitch=atan((out[0])/sqrt(pow((out[1]),2)+pow((out[2]),2)))*(180/3.1415926);
			roll=atan((out[1])/sqrt(pow((out[0]),2)+pow((out[2]),2)))*(180/3.1415926);
			//if(monitor_for_change(pitch,&mem[MEM_ACCEL])) printf("pitch:%f roll:%f \n",pitch,roll);
			acc_flag=0;
		}
		
		if(keypad_scan_flag) {
			key = get_key();
			if(key!=999) {
				if(monitor_for_change((int)key,&mem[MEM_KEY])) {
					printf("Digit: %d\n", key);
					keypad_scan_flag = 0;
				}
			}
		}
		
		if(seg_tim_flag) {
			seg_tim_flag = 0;
			if(pitch>=0){
				display(pitch);
			} else {
				display(-1*pitch);
			}
		}
		
		// if(delay_flag) {
		// HAL_Delay(1000);
		//	delay_flag = 0;
		//}*/
	}
	return 0;
}