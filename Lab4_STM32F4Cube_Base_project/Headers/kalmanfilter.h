#ifndef _KALMANFILTER_H
#define _KALMANFILTER_H

typedef struct{
	float F[1];
	float H[1];
	float Q[1];
	float R[1];
	float X[1];
	float P[1];
	float K[1];
} kalman_state;

int kalmanfilter_c(float*InputArray, float*OutputArray, kalman_state*kstate, int Length, int State_dimension, int Measurement_dimension);

#endif