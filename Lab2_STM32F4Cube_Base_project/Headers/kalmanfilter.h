#ifndef _KALMANFILTER_H
#define _KALMANFILTER_H

typedef struct{
	float F[4];
	float H[4];
	float Q[4];
	float R[4];
	float X[2];
	float P[4];
	float K[4];
} kalman_state;

int kalmanfilter_c(float*InputArray, float*OutputArray, kalman_state*kstate, int Length, int State_dimension, int Measurement_dimension);

#endif