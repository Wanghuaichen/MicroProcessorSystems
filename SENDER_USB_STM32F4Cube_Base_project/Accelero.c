#include "Accelero.h"
/* Read/Write command */
#define READWRITE_CMD              ((uint8_t)0x80)
#define OFFSET 90
#define STD_ERROR 4.5

int i = 0;

//get reading from the MEMS accelerometer
ACC_Reading get_Reading(){
	ACC_Reading data;
	float buffer[3];
	
	LIS3DSH_ReadACC(buffer);
	
	data.x = buffer[0] + X_Offset;	//store x
	data.y = buffer[1] + Y_Offset;	//store y
	data.z = buffer[2] + Z_Offset;	//store z 
	
	convertToAngles(&data , data.x , data.y , data.z);
	
	printf("pitch: %f ",data.pitch);
	printf("roll: %f  ",data.roll);
	printf("yaw: %f\n ",data.yaw);
	
	return data;
};

//convert the XYZ reading achieved from the MEMS sensor to pitch and roll angles
void convertToAngles(ACC_Reading *data, float x, float y, float z){
	//formulas to the the angles
	float pitch,roll,yaw;
	
	roll = (atan((x) / (sqrt(y*y+z*z)))*(180.0/PI)) + OFFSET - STD_ERROR;
	pitch = (atan((y) / (sqrt(x*x+z*z)))*(180.0/PI)) + OFFSET - STD_ERROR;
	yaw = (atan((z))/ (sqrt(x*x+z*z))*(180.0/PI)) + OFFSET - STD_ERROR;
	
	if (pitch<0)
		pitch = 360+pitch;
	if(roll<0)
		roll = 360 +roll;
	if(yaw<0)
		yaw = 360 + yaw;
	
	data->pitch = pitch;
	data->roll = roll;
	data->yaw = yaw;
}

void ACC_init(){
	//structure definitions
	LIS3DSH_InitTypeDef acc_init_s;
	LIS3DSH_DRYInterruptConfigTypeDef acc_int_struct;
	GPIO_InitTypeDef acc_GPIO_strct;
	// set acc struct vars;
	acc_init_s.Power_Mode_Output_DataRate = LIS3DSH_DATARATE_25	;				//Set the data rate to 25Hz
	acc_init_s.Full_Scale = LIS3DSH_FULLSCALE_2;  											//Set read range, lower range for more precision (+-2g) using 16 bits(2 registers)				
	acc_init_s.AA_Filter_BW = LIS3DSH_AA_BW_50;													
	acc_init_s.Self_Test = LIS3DSH_SELFTEST_NORMAL ; 
	//acc_init_s.Continous_Update = LIS3DSH_ContinousUpdate_Enabled ;
	acc_init_s.Axes_Enable = LIS3DSH_XYZ_ENABLE;
	// set interrupt struct vars
	acc_int_struct.Dataready_Interrupt = LIS3DSH_DATA_READY_INTERRUPT_ENABLED ; 
	acc_int_struct.Interrupt_signal = LIS3DSH_ACTIVE_HIGH_INTERRUPT_SIGNAL ; 
	acc_int_struct.Interrupt_type = LIS3DSH_INTERRUPT_REQUEST_PULSED ; 
	
	LIS3DSH_Init(&acc_init_s);
	LIS3DSH_DataReadyInterruptConfig(&acc_int_struct);
	
	//*************GPIO INIT*******************
	__HAL_RCC_GPIOE_CLK_ENABLE() ;
	
	// MEMS interrupt pins initialization
	acc_GPIO_strct.Pin = GPIO_PIN_0;					// Select pin 0 from GPIOE for the interrupt signal
	acc_GPIO_strct.Mode = GPIO_MODE_IT_RISING;
	acc_GPIO_strct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;	//Set speed
	acc_GPIO_strct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOE,&acc_GPIO_strct);
	
	//NVIC
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
	HAL_NVIC_SetPriority(EXTI0_IRQn,0,0);
	
};
