#include <stdio.h>
#include "arm_math.h"
#include "acc_normalization.h"

void acc_normalization(float *input, float *output){
	
	float accbuf1[3*3]={ 2100.789063,   58.499001,    -19.520002, //calibration parameters buffer
	                         10.675001, 2084.187012,   -7.929996,
	                         -2.135000,   29.219000, 2055.090088};
										
	float accbuf2[3*1]={0,0,0};
	float buf1[3*1];
	
	arm_matrix_instance_f32 IN = {3, 1, input}; //initialize input and output matrix
	arm_matrix_instance_f32 OUT = {3, 1, output};
	arm_matrix_instance_f32 ACC1 = {3, 3, accbuf1};
	arm_matrix_instance_f32 ACC2 = {3, 1, accbuf2};
	arm_matrix_instance_f32 B1 = {3, 1, buf1};
	
	arm_mat_mult_f32(&ACC1,&IN,&B1); //buffer 1 = ACC1*IN
	arm_mat_add_f32(&B1,&ACC2,&OUT); //OUT = ACC1*IN+ACC2
	
}
