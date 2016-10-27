////////////////////////////////////////////////////////////////////////////////
//	File Name					: main.c
//	Description				: program entry
//	Author						: Harsh Aurora
//	Date							: Oct 1st, 2016
////////////////////////////////////////////////////////////////////////////////
	
//		Includes		//
#include "main.h"
#include "utils.h"
#include "acc_normalization.h"
#include <kalmanfilter.h>
#include "game.h"

/*kalman_state kstate = { .F = {1}, //kalmanfilter states
                        .H = {1},
                        .Q = {.1},
						            .R = {0.7707},
						            .X = {0},
						            .P = {0.1},
						            .K = {1},
					            };*/

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

int main(void) {
	
	/*//initialize input length, state matrix, and measurement dimensions for kalmanfilter
	const int length=1;
  const int state_dimension=1;
  const int measurement_dimension=1;
	
	float out[3],normal[3],pitch_kalman_in[1], output[1];
  float pitch_raw, roll;*/
	
	system_init();

	while(1) {
		//display_2(0, DASHES);
		play();

		/*int tp = __HAL_TIM_GET_COUNTER(&TimerStructPiezo);
		int t7 = __HAL_TIM_GET_COUNTER(&TimerStruct7seg);
	
		
		//peak and hold for piezo
		if(piezo_tim_flag) {
			piezo_tim_flag = 0;
			piezo_counter++;
			piezo_adc_poll();
			piezo_peak_update();
			printf("piezo_val:%f\n",piezo_peak());
		}
		
		//accelerometer
		if(acc_flag) {
			LIS3DSH_ReadACC(out);
			acc_normalization(out,normal);
			printf("x:%f y:%f z:%f \n", normal[0], normal[1], normal[2]);
			pitch_raw=atan((normal[0])/sqrt(pow((normal[1]),2)+pow((normal[2]),2)))*(180/3.1415926);
			pitch_kalman_in[0]=pitch_raw;
			kalmanfilter_c(pitch_kalman_in, output, &kstate, length, state_dimension, measurement_dimension);
			roll=atan((out[1])/sqrt(pow((out[0]),2)+pow((out[2]),2)))*(180/3.1415926);
			//printf("pitch:%f roll:%f \n",kstate.X[0],roll);
			acc_flag=0;
		}
		
		 if(keypad_scan_flag) {
			key = get_key();
			if(key!=999) {
					printf("Digit: %d\n", key);
				if(monitor_for_change((int)key,&mem[1])) {
					//printf("Digit: %d\n", key);
					//keypad_scan_flag = 0;
				}
			}
		} 
		
		if(seg_tim_flag) {
			seg_tim_flag = 0;
			if(kstate.X[0]>=0){
				display(kstate.X[0]);
			} else {
				display(-1*kstate.X[0]);
			}
		}
		
		// if(delay_flag) {
		// HAL_Delay(1000);
		//	delay_flag = 0;
		//}*/
		
	} 
	return 0; 
}