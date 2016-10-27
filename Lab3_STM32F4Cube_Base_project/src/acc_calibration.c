/* Program description: this program is only used once for 
*                       calibration before using acc_sensor 
*                       to determine 12 calibration parameters
*/

#include <stdio.h>
#include "arm_math.h"
#include "acc_calibration.h"

int acc_calibration(void){

	float ybuf[6*3]={ 0, 0, 1, //stationary positions buffer
									  0, 0,-1,
									  0, 1, 0,
									  0,-1, 0,
									  1, 0, 0,
									 -1, 0, 0};
	float wbuf[6*4]={ 1, 0, 1, 1, //acclerometer raw data buffer
									  2, 0,-1, 1,
									  0, 1, 0, 1,
									  0,-1, 5, 1,
									  1, 0, 0, 1,
									 -1, 0, 7, 1};
	float wtransbuf[4*6];
	float xbuf[4*3]; //12 calibration parameters buffer
									 
	float buf1[4*4];
	float buf2[4*6];
									 
	arm_matrix_instance_f32 Y = {6, 3, ybuf};
	arm_matrix_instance_f32 W = {6, 4, wbuf};
	arm_matrix_instance_f32 WT = {4, 6, wtransbuf};
	arm_matrix_instance_f32 X = {4, 3, xbuf};
	
	arm_matrix_instance_f32 B1 = {4, 4, buf1};
	arm_matrix_instance_f32 B2 = {4, 6, buf2};
	
	arm_mat_trans_f32(&W,&WT); //W transpose
  arm_mat_mult_f32(&WT,&W,&B1); //buffer 1 = W^T*W
	arm_mat_inverse_f32(&B1,&B1); //W^T*W inverse
	arm_mat_mult_f32(&B1,&WT,&B2); //B2 = (W^T*W)^(-1)*W^T
	arm_mat_mult_f32(&B2,&Y,&X); //X = (W^T*W)^(-1)*W^T*Y

	printf("12 calibration parameters:\n");
	printf("%f %f %f\n", xbuf[0],xbuf[1],xbuf[2]); //display 12 calibration parameter result
	printf("%f %f %f\n", xbuf[3],xbuf[4],xbuf[5]);
	printf("%f %f %f\n", xbuf[6],xbuf[7],xbuf[8]);
	printf("%f %f %f\n", xbuf[9],xbuf[10],xbuf[11]);
	
}