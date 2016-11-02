#include "accelerometer.h"
#include "interfaces/accelerometer.h"

float accel_data = 0;

/*Brief: Get normalized accelerometer data and write to accel_data variable.
**Params: None
**Return: None
*/
void accel_get_data(void) {
	//printf("accel\n");
	LIS3DSH_ReadACC(out);
	acc_normalization(out,normal);
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