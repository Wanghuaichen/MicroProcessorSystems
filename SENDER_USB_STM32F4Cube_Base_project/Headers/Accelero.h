#include <Math.h>           
#include "stm32f4xx_it.h"
#include "LIS3DSH.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_conf.h"

#define PI 3.14159265
//these offset values are determined by calibrating the mems board using the simple method
#define X_Offset 61.24
#define Y_Offset 24.61
#define Z_Offset 30.86

//ACC_Reading struct is used to keep all the xyz pitch roll values of a single read together
typedef struct{
	float x;
	float y;
	float z;
	float pitch;
	float roll;
	float yaw;
	
}ACC_Reading;


ACC_Reading get_Reading(void);
void convertToAngles(ACC_Reading *data, float x, float y, float z);
void GPIO_init(void);
void ACC_init(void);
void EXTI0_and_NVIC_init(void);