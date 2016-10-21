#include "system_init.h"

void accelerometer_init();
void system_init() {
	//		MCU Configuration		//
	//	Reset of all peripherals, Initializes the Flash interface and the Systick	//
	HAL_Init();
	
	//	Configure the system clock	//
	SystemClock_Config();
	accelerometer_init();
}

void accelerometer_init() {

	LIS3DSH_InitTypeDef LIS3DSHStruct;

	LIS3DSHStruct.Power_Mode_Output_DataRate = LIS3DSH_DATARATE_25; //25Hz
	LIS3DSHStruct.Axes_Enable = LIS3DSH_XYZ_ENABLE; // enable all axes
	LIS3DSHStruct.Continous_Update = LIS3DSH_ContinousUpdate_Disabled; // block data update off
	LIS3DSHStruct.AA_Filter_BW = LIS3DSH_AA_BW_200; // cutoff 200Hz
	LIS3DSHStruct.Full_Scale = LIS3DSH_FULLSCALE_2; // +/-2 G - set to max sensitivity
	LIS3DSHStruct.Self_Test = LIS3DSH_SELFTEST_NORMAL; // selftest off (00)

	LIS3DSH_Init(&LIS3DSHStruct);
	//LIS3DSH_DataRateCmd(LIS3DSH_DATARATE_25);
	//HAL_GPIO_DeInit(GPIOE, GPIO_PIN_0);
}
