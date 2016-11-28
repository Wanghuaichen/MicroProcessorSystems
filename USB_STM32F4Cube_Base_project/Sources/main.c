////////////////////////////////////////////////////////////////////////////////
//	File Name					: main.c
//	Description				: program entry
//	Author						: Harsh Aurora
//	Date							: Oct 28, 2016
////////////////////////////////////////////////////////////////////////////////
	
//		Includes		//
#include <stm32f4xx_hal.h>
#include <supporting_functions.h>
#include <sysclk_config.h>
#include <lis3dsh.h>
#include <arm_math.h>
#include <LED_thread.h>
#include <mouse_thread.h>
#include <cmsis_os.h>
#include <rl_usb.h>                     // Keil.MDK-Pro::USB:CORE

#include "cc2500_spi.h"


//Brief:	main program
//				
//Params:	None
//Return:	None
int main(void) {
	
	uint8_t buffer_write[3]={1,2,3};
	uint8_t buffer_read[3];
	
  //		MCU Configuration		//
  //	Reset of all peripherals, Initializes the Flash interface and the Systick	//
	osKernelInitialize();  
  HAL_Init();
	
  //	Configure the system clock	//
  SystemClock_Config();
	
	USBD_Initialize(0);               /* USB Device 0 Initialization        */
  USBD_Connect(0); 
	
	start_LED_thread(NULL);
	start_mouse_thread(NULL);
	
	osKernelStart();
	CC2500_SPI_Init();
	
	//test read status reg.
	//printf("%d\n",CC2500_SPI_ReadStatusReg(0x31));
	
	//test single W/R
	//CC2500_SPI_WriteReg(0x03,4);
	//printf("%d\n",CC2500_SPI_ReadReg(0x03));
	
	//test burst W/R
	//CC2500_SPI_WriteRegBurst(0x03,buffer_write,3);
	//CC2500_SPI_ReadReg(0x03);
	//printf("%d\n",CC2500_SPI_ReadReg(0x03));
	//printf("%d\n",CC2500_SPI_ReadReg(0x04));
	//printf("%d\n",CC2500_SPI_ReadReg(0x05));
	//printf("%d\n, %d\n, %d\n",buffer_read[0],buffer_read[1],buffer_read[2]);
	
	//reset
	//printf("%d\n",CC2500_SPI_ReadReg(0x30));
	//printf("%d\n",CC2500_SPI_ReadReg(0x03));
	//printf("%d\n",CC2500_SPI_ReadReg(0x04));
	//printf("%d\n",CC2500_SPI_ReadReg(0x05));
	
	//test status
	//printf("%d\n",CC2500_SPI_WriteReg(0x03,4));
	
	//enable rx
	printf("%d\n",CC2500_SPI_ReadReg(0x34));
	CC2500_SPI_ReadRegBurst(0x3F, buffer_read, 3);
	printf("%d\n, %d\n, %d\n",buffer_read[0],buffer_read[1],buffer_read[2]);
	//printf("%d\n",CC2500_SPI_ReadReg(0x3F));
	//printf("%d\n",CC2500_SPI_ReadReg(0x3F));
	//printf("%d\n",CC2500_SPI_ReadReg(0x3F));
	//printf("%d\n",CC2500_SPI_ReadReg(0x3F));
	
	osDelay(osWaitForever);
	return 0;
}
