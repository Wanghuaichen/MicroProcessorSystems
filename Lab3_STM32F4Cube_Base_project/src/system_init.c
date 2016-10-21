#include <system_init.h>
#include <piezo_adc.h>

void accelerometer_init(void);
void system_init() {
	//		MCU Configuration		//
	//	Reset of all peripherals, Initializes the Flash interface and the Systick	//
	HAL_Init();
	
	//	Configure the system clock	//
	SystemClock_Config();
	
	//piezo initializatin
	piezo_adc_init();
	accelerometer_init();
	keypad_init();
}

void accelerometer_init() {

	LIS3DSH_InitTypeDef LIS3DSHStruct;
	LIS3DSH_DRYInterruptConfigTypeDef LIS3DSH_InterruptConfigStruct;

	LIS3DSHStruct.Power_Mode_Output_DataRate = LIS3DSH_DATARATE_25; //25Hz
	LIS3DSHStruct.Axes_Enable = LIS3DSH_XYZ_ENABLE; // enable all axes
	LIS3DSHStruct.Continous_Update = LIS3DSH_ContinousUpdate_Disabled; // block data update off
	LIS3DSHStruct.AA_Filter_BW = LIS3DSH_AA_BW_200; // cutoff 200Hz
	LIS3DSHStruct.Full_Scale = LIS3DSH_FULLSCALE_2; // +/-2 G - set to max sensitivity
	LIS3DSHStruct.Self_Test = LIS3DSH_SELFTEST_NORMAL; // selftest off (00)


	LIS3DSH_InterruptConfigStruct.Dataready_Interrupt = LIS3DSH_DATA_READY_INTERRUPT_ENABLED;            /* Enable/Disable data ready interrupt */
	LIS3DSH_InterruptConfigStruct.Interrupt_signal = LIS3DSH_ACTIVE_HIGH_INTERRUPT_SIGNAL;               /* Interrupt Signal Active Low / Active High */
	LIS3DSH_InterruptConfigStruct.Interrupt_type = LIS3DSH_INTERRUPT_REQUEST_PULSED;                     /* Interrupt type as latched or pulsed */

	LIS3DSH_Init(&LIS3DSHStruct);
	LIS3DSH_DataReadyInterruptConfig(&LIS3DSH_InterruptConfigStruct);
	//LIS3DSH_DataRateCmd(LIS3DSH_DATARATE_25);
	//HAL_GPIO_DeInit(GPIOE, GPIO_PIN_0);
}
