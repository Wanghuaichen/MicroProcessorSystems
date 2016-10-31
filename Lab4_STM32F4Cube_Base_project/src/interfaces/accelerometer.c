#include <stdio.h>
#include "arm_math.h"
#include "accelerometer.h"
#include "interfaces/LIS3DSH.h"

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

/*Brief: Inits the accelerometer
**Params: None
**Return: None
*/
void accelerometer_init() {

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
	//LIS3DSH_DataRateCmd(LIS3DSH_DATARATE_25);
	//HAL_GPIO_DeInit(GPIOE, GPIO_PIN_0);

	__HAL_RCC_GPIOE_CLK_ENABLE();

	acc_int_GPIO_struct.Pin = GPIO_PIN_0;
	acc_int_GPIO_struct.Mode = GPIO_MODE_IT_RISING;   			
	acc_int_GPIO_struct.Pull = GPIO_NOPULL;
	acc_int_GPIO_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(GPIOE, &acc_int_GPIO_struct);

	HAL_NVIC_SetPriority(EXTI0_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}