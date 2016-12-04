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

#include "cc2500.h"
#include "cc2500_spi.h"

void rx_run(void);
void tx_run(void);

int exti_flag;

//Brief:	main program
//				
//Params:	None
//Return:	None
int main(void) {
	
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
	CC2500_Rx_Interrupt_Config();
	
	if(0) rx_run();
	else tx_run();
	
	osDelay(osWaitForever);
	return 0;
}

void rx_run(void) {

	uint8_t size=4;
	uint8_t buffer_write[3]={1,2,3};
	uint8_t buffer_read[10];
	uint8_t rxBuffer[4];
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
	osDelay(1000);
	printf("%d\n",CC2500_SPI_ReadReg(0x30));
	//printf("%d\n",CC2500_SPI_ReadReg(0x03));
	//printf("%d\n",CC2500_SPI_ReadReg(0x04));
	//printf("%d\n",CC2500_SPI_ReadReg(0x05));
	
	//test status
	//printf("%d\n",CC2500_SPI_WriteReg(0x03,4));
	
	//reg. settings, and enable rx
	osDelay(1000);
	//CC2500_SPI_WriteReg(0x0A,1);
	//CC2500_SPI_WriteReg(0x0D,93);
	//CC2500_SPI_WriteReg(0x0E,148);
	//CC2500_SPI_WriteReg(0x0F,2);
	CC2500_Chipset_config();
  CC2500_rx_config();
	printf("radio mode: %d\n",CC2500_SPI_ReadReg(0x17)); //read radio mode
	while(1){
	  uint8_t status=CC2500_ReceivePacket(rxBuffer, size);
		if(status==Status_OK){
			printf("packet: %d %d %d %d\n", rxBuffer[0], rxBuffer[1],rxBuffer[2],rxBuffer[3]);
		//osDelay(500);
	  	}
	  //printf("%d\n", status)
	  //printf(" %d\n %d\n %d\n %d\n %d\n %d\n %d\n %d\n %d\n %d\n",rxBuffer[0],rxBuffer[1],rxBuffer[2],rxBuffer[3],rxBuffer[4],rxBuffer[5],rxBuffer[6],rxBuffer[7],rxBuffer[8],rxBuffer[9]);
	}
	
	//CC2500_ReceivePacket(rxBuffer,size);
	//uint8_t packet_length = 0;
	//while(packet_length == 0) {
	//	packet_length =CC2500_SPI_ReadReg(0x3F);
	//	printf("packet length is: %d\n", packet_length);
	//}
	//uint8_t buffer_read2[packet_length];
	//
	//CC2500_SPI_ReadRegBurst(0x3F, buffer_read2, packet_length);
	//for(int i=0; i<packet_length; i++){
	//	printf("%d\n", buffer_read2[i]);
	//}
	
	//printf("%d\n %d\n %d\n %d\n %d\n %d\n %d\n %d\n %d\n %d\n",buffer_read[0],buffer_read[1],buffer_read[2],buffer_read[3],buffer_read[4],buffer_read[5],buffer_read[6],buffer_read[7],buffer_read[8],buffer_read[9]);
	//printf("%d\n",CC2500_SPI_WriteReg(0x03,8));
	//printf("%d\n",CC2500_SPI_ReadReg(0x03));
	//printf("%d\n",CC2500_SPI_ReadReg(0x03));
	//printf("%d\n",CC2500_SPI_ReadReg(0x03));
	//printf("%d\n",CC2500_SPI_ReadReg(0x03));
	//printf("%d\n",CC2500_SPI_ReadReg(0x03));
	//printf("%d\n",CC2500_SPI_ReadReg(0x3F)); //read rx fifo
	//printf("%d\n",CC2500_SPI_ReadReg(0x3F));
	//printf("%d\n",CC2500_SPI_ReadReg(0x3F));
}

void tx_run(void) {

	//uint8_t size = 1;
	//uint8_t txBuffer[1] = {0};
	uint8_t txBuffer[4] = {234,123,255,0x01};
	//message_format(txBuffer,234,123,444,0x01);
	uint8_t size = 4;
	
	//reset
	osDelay(1000);
	CC2500_SPI_ReadReg(SRES);
	
	//reg. settings, and enable rx
	osDelay(1000);

	CC2500_Chipset_config();
  CC2500_tx_config();
	printf("radio mode: %d\n",CC2500_SPI_ReadReg(0x17)); //read radio mode
	while(1){
	  uint8_t status=CC2500_SendPacket(txBuffer, &size);
		printf("sending: %d, %d, %d, %d\n", txBuffer[0], txBuffer[1], txBuffer[2], txBuffer[3]);
		txBuffer[0] = txBuffer[0] + 1;
		osDelay(500);
	}
}
	
//EXTIO Interrupt Handler for the Accelerometer
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	GPIO_PinState state = HAL_GPIO_ReadPin (GPIOE , GPIO_Pin); 
	if (state == GPIO_PIN_SET){
		/* Do your stuff when PE0 is changed */
    exti_flag=1;	
	}	
}
