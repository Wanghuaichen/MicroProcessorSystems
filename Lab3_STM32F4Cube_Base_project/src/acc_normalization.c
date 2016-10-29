#include <stdio.h>
#include "arm_math.h"
#include "acc_normalization.h"
/*Brief: Normalize the accel values
**Params: float *input, float *output
**Return: None
*/
void acc_normalization(float *input, float *output){
	
	float accbuf1[3*3]={ 9.50816464e-04, -2.48012682e-06,  4.69005436e-06, //calibration parameters buffer
	                    -3.32538324e-05,  9.68788084e-04, -9.85398525e-07,
	                     1.37968711e-05, -1.09784720e-06,  9.71540051e-04};
										
	float accbuf2[3*1]={-0.01655845,0.02670086,-0.03848403};
	float buf1[3*1];
	
	arm_matrix_instance_f32 IN = {3, 1, input}; //initialize input and output matrix
	arm_matrix_instance_f32 OUT = {3, 1, output};
	arm_matrix_instance_f32 ACC1 = {3, 3, accbuf1};
	arm_matrix_instance_f32 ACC2 = {3, 1, accbuf2};
	arm_matrix_instance_f32 B1 = {3, 1, buf1};
	
	arm_mat_mult_f32(&ACC1,&IN,&B1); //buffer 1 = ACC1*IN
	arm_mat_add_f32(&B1,&ACC2,&OUT); //OUT = ACC1*IN+ACC2
	
}
