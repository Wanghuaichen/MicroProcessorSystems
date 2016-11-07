#include <math.h>
#include <stm32f4xx_hal.h>
#include "LIS3DSH.h"
#include "acc_normalization.h"
#include "kalmanfilter.h"
#include "utils.h"
#include "accelerometer_thread.h"

kalman_state pstate = { .F = {1}, //kalmanfilter states for pitch
                        .H = {1},
                        .Q = {.1},
						            .R = {0.7707},
						            .X = {0},
						            .P = {0.1},
						            .K = {1},
					            };

kalman_state rstate = { .F = {1}, //kalmanfilter states for roll
                        .H = {1},
                        .Q = {.1},
                        .R = {0.7707},
                        .X = {0},
                        .P = {0.1},
                        .K = {1},
                      };
float accel_data_pitch, accel_data_roll;	
int thread_flag;											
int length = 1;
int state_dimension = 1;
int measurement_dimension = 1;
int accel_sleep = 0;											

//Global
osThreadId accelerometer_thread_ID;
osThreadDef(accelerometer_thread, osPriorityAboveNormal, 1,0);
osSemaphoreId sem_accel;
osSemaphoreDef(sem_accel);


void accelerometer_thread_init(void) {
  LIS3DSH_InitTypeDef LIS3DSHStruct;
	LIS3DSH_DRYInterruptConfigTypeDef LIS3DSH_InterruptConfigStruct;
	GPIO_InitTypeDef acc_int_GPIO_struct;

	LIS3DSHStruct.Power_Mode_Output_DataRate = LIS3DSH_DATARATE_25; //25Hz
	LIS3DSHStruct.Axes_Enable = LIS3DSH_XYZ_ENABLE; // enable all axes
	LIS3DSHStruct.Continous_Update = LIS3DSH_ContinousUpdate_Disabled; // block data update on (lower and upper written when both are ready)
	LIS3DSHStruct.AA_Filter_BW = LIS3DSH_AA_BW_200; // cutoff 200Hz
	LIS3DSHStruct.Full_Scale = LIS3DSH_FULLSCALE_2; // +/-2 G - set to max sensitivity
	LIS3DSHStruct.Self_Test = LIS3DSH_SELFTEST_NORMAL; // selftest off (00)


	LIS3DSH_InterruptConfigStruct.Dataready_Interrupt = LIS3DSH_DATA_READY_INTERRUPT_ENABLED;            /* Enable/Disable data ready interrupt */
	LIS3DSH_InterruptConfigStruct.Interrupt_signal = LIS3DSH_ACTIVE_HIGH_INTERRUPT_SIGNAL;               /* Interrupt Signal Active Low / Active High */
	LIS3DSH_InterruptConfigStruct.Interrupt_type = LIS3DSH_INTERRUPT_REQUEST_LATCHED;                     /* Interrupt type as latched or pulsed */

	LIS3DSH_Init(&LIS3DSHStruct);
	LIS3DSH_DataReadyInterruptConfig(&LIS3DSH_InterruptConfigStruct);

	__HAL_RCC_GPIOE_CLK_ENABLE();

	acc_int_GPIO_struct.Pin = GPIO_PIN_0;
	acc_int_GPIO_struct.Mode = GPIO_MODE_IT_RISING;   			
	acc_int_GPIO_struct.Pull = GPIO_NOPULL;
	acc_int_GPIO_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(GPIOE, &acc_int_GPIO_struct);

  HAL_NVIC_EnableIRQ(EXTI0_IRQn);
	HAL_NVIC_SetPriority(EXTI0_IRQn, 1, 0);
	
	osSemaphoreCreate(osSemaphore(sem_accel), 1);
}			

void start_accelerometer_thread(void *args) {
	accelerometer_thread_ID = osThreadCreate(osThread(accelerometer_thread), args);
}

void accelerometer_thread(void const *args) {
	accelerometer_thread_init();
	while(1) {
		osSignalWait(0x00000001, osWaitForever);
		get_pitch_value();
		get_roll_value();
	}
}

void get_pitch_value(void){
	
	float pitch; 
	float	pitch_kalman_in[1], output[1];
	float out[3], normal[3];
	
	LIS3DSH_ReadACC(out);
	acc_normalization(out,normal);
	if(normal[1]==0&&normal[2]==0) {
		osSemaphoreWait(sem_accel, osWaitForever);
		accel_data_pitch = 999;
		osSemaphoreRelease(sem_accel);
		return;
	}
	pitch=atan( (normal[0])/sqrt(pow((normal[1]),2)+pow((normal[2]),2)) ) * (180/3.1415926); // x/sqrt(y^2+z^2)
	
  if(monitor_for_change(pitch,&mem[MEM_ACCEL])) {
		pitch_kalman_in[0]=pitch;
		kalmanfilter_c(pitch_kalman_in, output, &pstate, length, state_dimension, measurement_dimension);
		if(pstate.X[0]>=0){
			osSemaphoreWait(sem_accel, osWaitForever);
			accel_data_pitch = pstate.X[0]+90;
			osSemaphoreRelease(sem_accel);
		} else {
			osSemaphoreWait(sem_accel, osWaitForever);
			accel_data_pitch = 90-fabsf(pstate.X[0]);
			osSemaphoreRelease(sem_accel);
		}
	}
}

void get_roll_value(void){
	
	float roll; 
	float	roll_kalman_in[1], output[1];
	float out[3], normal[3];
	
	LIS3DSH_ReadACC(out);
	acc_normalization(out,normal);
	osSemaphoreWait(sem_accel, osWaitForever);
	roll = atan( (normal[1])/sqrt( pow((normal[0]),2) + pow((normal[2]),2) ) ) * (180/3.1415926);  
	osSemaphoreRelease(sem_accel);
	
	if(monitor_for_change(roll,&mem[MEM_ACCEL])) {
		roll_kalman_in[0]=roll;
		kalmanfilter_c(roll_kalman_in, output, &rstate, length, state_dimension, measurement_dimension);
		if(rstate.X[0]>=0){
			accel_data_roll = rstate.X[0]+90;
		} else {
			accel_data_roll = 90-fabsf(rstate.X[0]);
		}
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if(GPIO_Pin == GPIO_PIN_0 && !accel_sleep) {
		osSignalSet(accelerometer_thread_ID, 0x00000001);
	}
}