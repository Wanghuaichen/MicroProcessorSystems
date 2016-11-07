////////////////////////////////////////////////////////////////////////////////
//	File Name					: kalmanfilter_c.c
//	Description				: this program contains the algorithm for kalmanfilter
//                      raw values
//	Author						: Tianming Zhang, Alex Bhandari  
//	Date							: Nov 6, 2016
////////////////////////////////////////////////////////////////////////////////

//include
#include "arm_math.h"
#include <stdio.h>
#include <kalmanfilter.h>

/*Brief: multi-dimensional kalmanfilter
**Params: float*InputArray, float*OutputArray, kalman_state*kstate, int Length, int State_dimension, int Measurement_dimension
**Return: None
*/
int kalmanfilter_c(float*InputArray, float*OutputArray, kalman_state*kstate, int Length, int State_dimension, int Measurement_dimension){
	
	int i;
	
	float FTrans[State_dimension*State_dimension]; //initialize state array
	float HTrans[State_dimension*State_dimension];

	float residuals[Measurement_dimension*Length]; //initialize residuals array
	
	float inputBuffer[Measurement_dimension]; //initialize buffer array
	float buffer1[State_dimension*State_dimension]; 
	float buffer2[State_dimension*State_dimension];
	float buffer3[State_dimension*State_dimension];
	float buffer4[State_dimension*State_dimension];
	float buffer5[State_dimension*State_dimension]; 
	float ColArray[State_dimension];
	float inverse[State_dimension*State_dimension];
	float identity[1]={1}; //initialize identity matrix
	
	float32_t c[Measurement_dimension]; //initialize correlation, mean, and standard deviation arrays
	float32_t mean[Measurement_dimension];
	float32_t std[Measurement_dimension];
	float32_t c1[Measurement_dimension];
	float32_t mean1[Measurement_dimension];
	float32_t std1[Measurement_dimension];
	
	arm_matrix_instance_f32 F = {State_dimension, State_dimension, kstate->F}; //initialize state matrices
	arm_matrix_instance_f32 FT = {State_dimension, State_dimension, FTrans};
	arm_matrix_instance_f32 X = {State_dimension, 1, kstate->X};
	arm_matrix_instance_f32 P = {State_dimension, State_dimension, kstate->P};
	arm_matrix_instance_f32 Q = {State_dimension, State_dimension, kstate->Q};
	arm_matrix_instance_f32 H = {State_dimension, State_dimension, kstate->H};
	arm_matrix_instance_f32 HT = {State_dimension, State_dimension, HTrans};
	arm_matrix_instance_f32 R = {State_dimension, State_dimension, kstate->R};
	arm_matrix_instance_f32 K = {State_dimension, State_dimension, kstate->K};
	
	arm_matrix_instance_f32 rd = {State_dimension, Length, residuals}; //initialize residual matrices
	
	arm_matrix_instance_f32 B1 = {State_dimension, State_dimension, buffer1}; //initialize buffer matrics
	arm_matrix_instance_f32 B2 = {State_dimension, State_dimension, buffer2};
	arm_matrix_instance_f32 B3 = {State_dimension, State_dimension, buffer3};
	arm_matrix_instance_f32 B4 = {State_dimension, State_dimension, buffer4};
	arm_matrix_instance_f32 B5 = {State_dimension, Length, buffer5};
	arm_matrix_instance_f32 Col = {State_dimension, 1, ColArray};
	arm_matrix_instance_f32 INV = {State_dimension, State_dimension, inverse};
	arm_matrix_instance_f32 I = {State_dimension, State_dimension, identity};
	
	for(i=0; i<Length; i++){ //set up a loop to compute column vectors one by one
		
		for (int k=0; k<Measurement_dimension; k++){ //split input array to column vectors 
			inputBuffer[k]=InputArray[i*Measurement_dimension+k];
		}
		
		/*printf("input column vector:-\n"); //print current column vector for debugging
		for (int j = 0; j < (Measurement_dimension); j++){
			printf("%f\t", inputBuffer[j]);
			printf("\n");
		}*/
		
		arm_matrix_instance_f32 Z = {Measurement_dimension, 1, inputBuffer}; //initialize column vector as arm_matrix
	  
		/*kalmanfilter algorithm*/
		arm_mat_mult_f32(&F,&X,&X);//predict X = F*X
		
    /*predict P*/
		arm_mat_mult_f32(&F,&P,&B1);//buffer 1 = F*P
		arm_mat_trans_f32(&F,&FT);//F transpose
		arm_mat_mult_f32(&B1,&FT,&B2);//buffer 2 = F*P*(F transpose)
		arm_mat_add_f32(&B2,&Q,&P);//predict P = F*P*(F transpose)+ Q
			
		/*update K*/
		arm_mat_mult_f32(&H,&P,&B1);//buffer 1 = H*P
		arm_mat_trans_f32(&H,&HT);//H transpose
		arm_mat_mult_f32(&B1,&HT,&B2);//buffer 2 = H*P*(H transpose)
		arm_mat_add_f32(&B2,&R,&B3);//buffer 3 = H*P*(H transpose)+R
		arm_mat_inverse_f32(&B3,&INV);//inverse = buffer 3 inverse
		arm_mat_mult_f32(&P,&HT,&B2);//buffer 2 = P(predict)*H(transpose)
		arm_mat_mult_f32(&B2,&INV,&K);//update K = P(predict)*H(transpose)*(H*P*(H transpose)+R)^(-1)
		
		/*update P*/
		arm_mat_mult_f32(&K,&H,&B2);//buffer 2 = (K update)*H
		arm_mat_sub_f32(&I,&B2,&B4);//buffer 4 = I-(K update)*H
		arm_mat_mult_f32(&B4,&P,&P);//update P = (I-(K update)*H)*(predict p)
		
		/*update x*/
		arm_mat_mult_f32(&H,&X,&Col);//buffer column = H*(X predict)
		arm_mat_sub_f32(&Z,&Col,&Col);//buffer column = Z - H*(X predict)
		arm_mat_mult_f32(&K,&Col,&Col);//buffer column = K*(Z - H*(X predict))
		arm_mat_add_f32(&X,&Col,&X);//update X = (X pedict) + K*(Z - H*(X predict))
		
	  /*printf("output column vector from c:-\n"); //print current output column vector for debugging
		for (int j = 0; j < (State_dimension); j++){
			printf("%f\t", kstate->X[j]);
			printf("\n");
			OutputArray[i*Measurement_dimension+j] = kstate->X[j];//combine output column vector to output array
		}*/
	}
	
	/*printf("input           output          \n");//print input array, output array, and residual array
	for (int i = 0; i < (State_dimension*Length); i++){
		printf("%f\t", InputArray[i]);
		printf("%f\t", OutputArray[i]);
		printf("\n");
	}*/

	return 0;
}
