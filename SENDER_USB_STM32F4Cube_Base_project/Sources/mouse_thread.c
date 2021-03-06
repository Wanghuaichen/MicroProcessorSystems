////////////////////////////////////////////////////////////////////////////////
//	File Name					: mouse.c
//	Description				: an OS thread controlling the mouse USB 
//											interface via the on-board pushbutton (left click),
//                      as well as GPIO buttons (right click and scroll function)
//	Author						: Zeyad Saleh    -260556530
//                      Mahmood Hegazy -260580124
//                      Alex Bhandari  -260520610
//                      Tianming Zhang -260528705
//	Date							: Dec 05, 2016
////////////////////////////////////////////////////////////////////////////////
	
//		Includes		//
#include <mouse_thread.h> 
#include <stm32f4xx_hal.h>
#include <cmsis_os.h>
#include <rl_usb.h>
#include <stdlib.h>
#include "cc2500.h"
#include "cc2500_spi.h"
#include <segmentsDisplay.h>


#define THRESHOLD 5

extern int recieve;

//		Globals 		//
uint8_t  mouse_in_report[4] = {0,0,0,0};

float previousPitch;
float previousRoll;
float currentPitch;
float currentRoll;
float currentRollRef = 90;
float currentPitchRef = 90;
int leftButtonPressed;
int rightButtonPressed;
int scrollButtonPressed;


//Brief:		Initializes the GPIO for the pushbutton
//Params:		None
//Return:		None
void mouse_thread_periph_init(void) {
	GPIO_InitTypeDef mouse_GPIO_struct;
	
	__HAL_RCC_GPIOA_CLK_ENABLE();

	mouse_GPIO_struct.Pin		= GPIO_PIN_0;
	mouse_GPIO_struct.Mode	= GPIO_MODE_INPUT;
	mouse_GPIO_struct.Pull	= GPIO_NOPULL;
	mouse_GPIO_struct.Speed	= GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(GPIOA, &mouse_GPIO_struct);
}

//Brief:		The mouse thread function in the OS
//					Performs a mouse click if the userbutton is pressed
//Params:		A void pointer to initial arguments, NULL if unused
//Return:		None
void mouse_thread(void const *args) {
	mouse_in_report[0] = 0;
	mouse_in_report[1] = 0;
	mouse_in_report[2] = 0;
	mouse_in_report[3] = 0;
	
	mouse_thread_periph_init();
	while(1) {
		osSignalWait(0x00000001, osWaitForever);
		
		if(recieve) {
			//RECIEVER
			uint8_t size = 4;
			uint8_t status=CC2500_ReceivePacket(mouse_in_report, size);
			if(status==Status_OK){
				printf("packet: %d %d %d %d\n", mouse_in_report[0], mouse_in_report[1],mouse_in_report[2],mouse_in_report[3]);
	  	}
			USBD_HID_GetReportTrigger(0, 0, mouse_in_report, 4);
			
			float numToDisplay = 0;
			
			//Set display
			if(mouse_in_report[1] < 159){
				printf("1\n");
				numToDisplay += 0.1;
			}
			else {
				printf("2\n");
				numToDisplay += 0.2;
			}
			
			if(mouse_in_report[2] < 159){
				printf("3\n");
				numToDisplay += 100;
			}
			else {
				printf("4\n");
				numToDisplay += 200;
			}
			setNumberToDisplay(numToDisplay);
			numToDisplay = 0;
			
		} else {
			//SENDER
			int horizontalMovement = getHorizontalMovementDirection();
			int verticalMovement = getVerticalMovementDirection();
			
			if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == SET){
				printf("Left Click\n");
				leftButtonPressed = 1;
				mouse_in_report[0] = 0x01;
				leftButtonPressed = 0;
			}
			else if(rightButtonPressed == 1){
				printf("Right Click\n");
				mouse_in_report[0] = 0x02;
				rightButtonPressed = 0;
			}
			else{
				//printf("No Click\n");
				mouse_in_report[0] = 0x00;
			}
			// If scrolling
			if(scrollButtonPressed == 1){
				printf("Scrolling Click\n");
				mouse_in_report[1] = 0;
				mouse_in_report[2] = 0;
				mouse_in_report[3] = verticalMovement;
				scrollButtonPressed = 0;
			}
			else{
				//printf("No Scrolling\n");
				mouse_in_report[1] = horizontalMovement;
				mouse_in_report[2] = verticalMovement;
				mouse_in_report[3] = 0;
			}
			//send mouse data through transmitter
			uint8_t size = 4;
			uint8_t status=CC2500_SendPacket(mouse_in_report, &size);
			printf("sending: %d, %d, %d, %d\n", mouse_in_report[0], mouse_in_report[1], mouse_in_report[2], mouse_in_report[3]);
			osDelay(50);
		}
	}
}

//Brief:		Set left button press
//Params:		None
//Return:		None
void setLeftButtonPress(){
	leftButtonPressed = 1;
}

//Brief:		Set right button press
//Params:		None
//Return:		None
void setRightButtonPress(){
	printf("MOUSE: Right pressed\n");
	rightButtonPressed = 1;
}

//Brief:		Set Scroll Button Press
//Params:		None
//Return:		None
void setScrollButtonPress(){
	printf("MOUSE: Scroll pressed\n");
	scrollButtonPressed = 1;
}

//Brief:		Sets the previousPitch to the currentPitch 
//					and the currentPitch to the new value pitch
//Params:		new pitch value
//Return:		None
void setCurrentPitch(float pitch){
	previousPitch = currentPitch;
	currentPitch = pitch;
}

//Brief:		Sets the previousRoll to the currentRoll
//					and the currentRoll to the new value roll
//Params:		new roll value
//Return:		None
void setCurrentRoll (float roll){
	previousRoll = currentRoll;
	currentRoll = roll;
}

//Brief:		Gets the differenece between the current and the previous pitch values
//Params:		None
//Return:		The change of the pitch between the current and the previous values (from 0 to 359)
float getDeltaPitch(){
	return currentPitchRef - currentPitch;
}


//Brief:		Gets the differenece between the current and the previous roll values
//Params:		None
//Return:		The change of the roll between the current and the previous values (from 0 to 359)
float getDeltaRoll(){
	return currentRollRef - currentRoll;
}

//Brief:		Horizontal mouse movement
//          The 8 bit number to mouse cursor movement is as follows
//					0 to 9F (159): Increasing speed mouse movement to the right
//					A0 (160) to FF (255): Decreasing speed mouse movement to the 
//Params:		None
//Return:		None
uint8_t getHorizontalMovementDirection(){
	float deltaRoll = getDeltaRoll();
	//printf("	current reference = %f\n", currentRollRef);
	//printf("	current pitch = %f\n", currentPitch);
	//printf("	deta pitch = %f\n", deltaRoll);
	if (deltaRoll > 0){		//Moving in right direction
		if (deltaRoll > 90){		//Exceeded max threshold for right direction
			return 0x9F;
		}
		else {
			float normalizedFloat = (deltaRoll * 90)/159;
			//printf("Right direction normalized float = %f\n", normalizedFloat);
			uint8_t normalizedInt = (uint8_t) normalizedFloat;
			//printf("Right direction normalized int = %d\n", normalizedInt);
			return normalizedInt;
		}
	}
	
	else{
		if (deltaRoll < -90){		//Exceeded max threshold for right direction
			return 0xA0;		//Maximum left movement velocity
		}
		else {
			float normalizedFloat = (abs(deltaRoll) * 90)/159;
			//printf("Left direction normalized float = %f\n", normalizedFloat);
			uint8_t normalizedInt = (uint8_t) normalizedFloat;
			//printf("Left direction normalized int = %d\n", normalizedInt);
			return 0xFF - normalizedInt;
		}
	}
}

//Brief:    Vertical mouse movement
//					The 8 bit number to mouse cursor movement is as follows
//					0 to 9F (159): Increasing speed mouse movement to the right
//					A0 (160) to FF (255): Decreasing speed mouse movement to the left
//Params:		None
//Return:		None
uint8_t getVerticalMovementDirection(){
	float deltapitch = getDeltaPitch();
	//printf("	current reference = %f\n", currentRollRef);
	//printf("	current pitch = %f\n", currentPitch);
	//printf("	deta pitch = %f\n", deltapitch);
	if (deltapitch > 0){		//Moving in upward direction
		if (deltapitch > 90){		//Exceeded max threshold for right direction
			return 0xA0;
		}
		else {
			float normalizedFloat = (deltapitch * 90)/159;
			//printf("Right direction normalized float = %f\n", normalizedFloat);
			uint8_t normalizedInt = (uint8_t) normalizedFloat;
			//printf("Right direction normalized int = %d\n", normalizedInt);
			return 0xFF - normalizedInt;
		}
	}
	
	else{
		if (deltapitch < -90){		//Exceeded max threshold for right direction
			return 0x9F;		//Maximum left moveme                                                                                                                                                                                                                                                                                                      nt velocity
		}
		else {
			float normalizedFloat = (abs(deltapitch) * 90)/159;
			//printf("Left direction normalized float = %f\n", normalizedFloat);
			uint8_t normalizedInt = (uint8_t) normalizedFloat;
			//printf("Left direction normalized int = %d\n", normalizedInt);
			return normalizedInt;
		}
	}
}
