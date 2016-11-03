#include <math.h>
#include "modules/accelerometer.h"
#include "modules/keypad.h"
#include "interfaces/LIS3DSH.h"
#include "interfaces/accelerometer.h"
#include "interfaces/kalmanfilter.h"
#include "utils/utils.h"
#include "threads.h"

float accel_data = 0;

kalman_state kstate = { .F = {1}, //kalmanfilter states
                        .H = {1},
                        .Q = {.1},
                        .R = {0.7707},
                        .X = {0},
                        .P = {0.1},
                        .K = {1},
                      };

/*Brief: Get normalized accelerometer data and write to accel_data variable.
**Params: None
**Return: None
*/
void accel_get_data(void) {
	
	float pitch, roll, length=1, state_dimension=1, measurement_dimension=1;
	float pitch_kalman_in[1], output[1], roll_kalman_in[1], out[3], normal[3];
	
	//get raw acc data
	LIS3DSH_ReadACC(out);
	//normalize raw acc data
	acc_normalization(out,normal);
	
	//calculate pitch and roll
	pitch=atan((normal[0])/sqrt(pow((normal[1]),2)+pow((normal[2]),2)))*(180/3.1415926);
	//roll=atan((normal[1])/sqrt(pow((normal[0]),2)+pow((normal[2]),2)))*(180/3.1415926);
	//printf("pitch:%f roll:%f \n",pitch,roll);
	
	if(monitor_for_change(pitch,&mem[MEM_ACCEL])) {
		pitch_kalman_in[0]=pitch;
		kalmanfilter_c(pitch_kalman_in, output, &kstate, length, state_dimension, measurement_dimension);
		if(kstate.X[0]>=0){
			accel_data = kstate.X[0]+90;
		} else {
			accel_data = 90-fabsf(kstate.X[0]);
		}
		if(fabsf(accel_data-value) < 5) {
			accel_data = value;
		}
		printf("Tilt: %f Target: %f Diff: %f\n",accel_data,value,value-kstate.X[0]); 
	}
}

/*Brief: callback from stm32f4xx_hal_gpio.c
**Params: uint16_t GPIO_Pin
**Return: None
*/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if(GPIO_Pin == GPIO_PIN_0) {
		accel_int_flag = 1;
	} else {
		//keypad_scan_flag = 1;
	}
}