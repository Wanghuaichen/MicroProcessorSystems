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
#include <mouse_thread.h>
#include <cmsis_os.h>
#include <rl_usb.h>                     // Keil.MDK-Pro::USB:CORE
#include <segmentsDisplay.h>
#include "accelero.h"
#include "keypad.h"
#include "cc2500.h"
#include "cc2500_spi.h"

//Interrupts
void MEMS_handler(void const *argument);
void init_TIM3();
void controller_thread(void const *argument);

//Global var for accelerometer readings
ACC_Reading acc_reading;

osThreadId MEMS_handler_thread;
osThreadId MOUSE_thread_ID;
osThreadId CONTROLLER_thread_ID;
osThreadId KEYPAD_thread_ID;
osThreadId DISPLAY_thread_ID;

osThreadDef(MEMS_handler, osPriorityNormal, 1, 0);
osThreadDef(mouse_thread, osPriorityNormal, 1,0);
osThreadDef(controller_thread, osPriorityNormal, 1, 0);
osThreadDef(keypad_thread, osPriorityNormal, 1, 0);
osThreadDef(display_thread, osPriorityNormal, 1, 0);

TIM_HandleTypeDef TIM3_handle;

int recieve = 0;

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

  //----Wireless Config------------
  //init
  CC2500_SPI_Init();
 	//reset
	osDelay(1000);
	printf("%d\n",CC2500_SPI_ReadReg(0x30));
	//reg. settings, and enable rx
	osDelay(1000);
	CC2500_Chipset_config();
	if(recieve) {CC2500_rx_config();} else {CC2500_tx_config();}
	//-------------------------------

	//Initialize acclerometer
	ACC_init();	
	
	//Initialize timer
	init_TIM3();
	
	MOUSE_thread_ID 			= osThreadCreate(osThread(mouse_thread)				, NULL);
	MEMS_handler_thread 	= osThreadCreate(osThread(MEMS_handler)				, NULL);
	KEYPAD_thread_ID 			= osThreadCreate(osThread(keypad_thread)			, NULL);
	CONTROLLER_thread_ID 	= osThreadCreate(osThread(controller_thread)	, NULL);
	DISPLAY_thread_ID 		= osThreadCreate(osThread(display_thread)			, NULL);
	
	osKernelStart();
	osDelay(osWaitForever);
	return 0;
}

void controller_thread(void const *argument){
	while(1){
		osSignalWait (0x00000001, osWaitForever);
		//printf("CONTROLLER: Accelerometer thread as seen from the controller %f\n", acc_reading.pitch);
		setCurrentPitch(acc_reading.pitch);
		setCurrentRoll(acc_reading.roll);
		
		if(isRightPressed()){
			//printf("CONTROLLER: Right BTN pressed\n");
			setRightButtonPress();
		}
		if(isScrollPressed()){
			//printf("CONTROLLER: Scroll BTN pressed\n");
			setScrollButtonPress();
		}
	}
}

void MEMS_handler(void const *argument){
	while(1){
		osSignalWait(0x01, osWaitForever);
		//execAcceleroInterrupt(&kstate_X, &kstate_Y, &kstate_Z);
		acc_reading = get_Reading();
	}
}

//EXTIO Interrupt Handler for the Accelerometer
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	GPIO_PinState state = HAL_GPIO_ReadPin (GPIOE , GPIO_Pin); 
	if (state == GPIO_PIN_SET){
		/* Do your stuff when PE0 is changed */
    osSignalSet(MEMS_handler_thread, 0x00000001);
		osSignalSet(CONTROLLER_thread_ID, 0x00000001);
		osSignalSet(MOUSE_thread_ID, 0x00000001);
		osSignalSet(KEYPAD_thread_ID, 0x00000001);		
	}	
}

void init_TIM3(){
	__HAL_RCC_TIM3_CLK_ENABLE();
	
	printf("init tim 3\n");
	TIM3_handle.Instance = TIM3;
	TIM3_handle.Init.Prescaler					= 20;  						// 20999     
	TIM3_handle.Init.CounterMode				= TIM_COUNTERMODE_DOWN;     
	TIM3_handle.Init.Period						= 8400;           			// 3999
	TIM3_handle.Init.ClockDivision			= 0;    
	TIM3_handle.Init.RepetitionCounter	= 0;
	
	HAL_TIM_Base_Init(&TIM3_handle);
	HAL_TIM_Base_Start_IT(&TIM3_handle);
	
	HAL_NVIC_EnableIRQ(TIM3_IRQn);
	HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
}

//Brief:		The TIM interrupt callback. Sends a signal to the LED_thread
//					if the interrupt was recived from TIM3
//Params:		Pointer to the TIM handle that caused the interrupt
//Return:		None
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {	
	if(htim->Instance == TIM3){
		osSignalSet(DISPLAY_thread_ID, 0x00000001);
	}
}
