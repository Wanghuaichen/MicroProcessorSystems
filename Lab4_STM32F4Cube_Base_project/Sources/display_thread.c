#include <stm32f4xx_hal.h>
#include <supporting_functions.h>
#include <cmsis_os.h>
#include "timer.h"
#include "bool.h"
#include "accelerometer_thread.h"
#include "temperature_thread.h"
#include "display_thread.h"
#include "keypad.h"
#include "keypad_thread.h"

//typedef enum{INIT, ACCEL, TEMP, KEYPAD}state;
//state display_state=TEMP;//used for testing 
int scan_digit=1;
int digit[5];
int special;
float display_val;

//Global
osThreadId display_thread_ID;
osThreadDef(display_thread, osPriorityNormal, 1,0);
//TIM_HandleTypeDef TIM4_handle;

//Brief: Sets up the desired(refer to header) GPIO pin to output mode
//			 and initializes its value to 0
//Params: None
//Return: None
void display_thread_periph_init(void) {
	GPIO_InitTypeDef GPIO_InitDefB;
	GPIO_InitTypeDef GPIO_InitDefD;	
	
	CLK_DISPLAY_PORT_CLK_EN;
	
	GPIO_InitDefB.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
	GPIO_InitDefB.Mode = GPIO_MODE_OUTPUT_PP;   			
	GPIO_InitDefB.Pull = GPIO_NOPULL;
	GPIO_InitDefB.Speed = GPIO_SPEED_FREQ_MEDIUM;	

	GPIO_InitDefD.Pin = GPIO_PIN_6; 
	GPIO_InitDefD.Mode = GPIO_MODE_OUTPUT_PP;   			
	GPIO_InitDefD.Pull = GPIO_NOPULL;
	GPIO_InitDefD.Speed = GPIO_SPEED_FREQ_MEDIUM;	
	
	HAL_GPIO_Init(GPIOB, &GPIO_InitDefB);
	HAL_GPIO_Init(GPIOD, &GPIO_InitDefD);
	
	timer_init();
}

//Brief: Reset display pins;
//Params: None
//Return: None
void seven_seg_reset(void){
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, GPIO_PIN_RESET);
}

//Brief: start display thread;
//Params: None
//Return: None
void start_display_thread(void *args) {
	display_thread_ID = osThreadCreate(osThread(display_thread), args);
}

//Brief:		The display thread function in the OS
//					Waits for a signal from the TIM4 interrupt handler and refresh the display
//Params:		A void pointer to initial arguments, NULL if unused
//Return:		None
void display_thread(void const *args) {
	display_thread_periph_init();
	while(1) {
		osSignalWait(0x00000001, osWaitForever);
		seven_segment_refresh();
	}
}

//Brief:		The TIM interrupt callback. Sends a signal to the display_thread
//					if the interrupt was recived from TIM4
//Params:		Pointer to the TIM handle that caused the interrupt
//Return:		None
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {	
//	if(htim->Instance == TIM4)
//		osSignalSet(display_thread_ID, 0x00000001);
//}

void set_letters(char *letters,int length) {
	for(int i=0;i<length;i++) {
		switch (letters[i]) {
			case 'A':
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);//A	- 7seg pin 14
			  //printf("A");
				break;
			case 'B':
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);//B	- 7seg pin 16
			  //printf("B");
				break;
			case 'C':
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);//C	- 7seg pin 13
				//printf("C");
				break;
			case 'D':
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2 , GPIO_PIN_SET);//D	- 7seg pin 3
				//printf("D");
				break;
			case 'E':
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4 , GPIO_PIN_SET);//E	- 7seg pin 5
				//printf("E");
				break;
			case 'F':
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);//F	- 7seg pin 11
				//printf("F");
				break;
			case 'G':
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);//G	- 7seg pin 15
				//printf("G");
				break;
			case '.':
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6 , GPIO_PIN_SET);//.	- 7seg pin 7
				//printf(".");
				break;
			case ':':
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6 , GPIO_PIN_SET);//.	- 7seg pin 4
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET);//.	- 7seg pin 12
				//printf(":");
				break;
			case 'U':
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9 , GPIO_PIN_SET);//.	- 7seg pin 10
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8 , GPIO_PIN_SET);//.	- 7seg pin 9
				//printf("U");
				break;
			default:
				break;
		}
	}
	//printf("\n");
}

void seven_seg_digit_display(int place, int digit, bool decimal) {
	//set which digit is being written
	switch (place) {
		case DIG_1:
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
			break;
		case DIG_2:
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
			break;
		case DIG_3:
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
			break;
		case DIG_4:
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
			break;
		default:
			break;
	}
	//set the value of the digit
	switch (digit) {
		case 0: //BDEAC
			set_letters("BCDEAF",6);
			break;
		case 1: //BC
			set_letters("BC",2);
			break;		
		case 2: //GBDEA
			set_letters("GBDEA",5);
			break;
		case 3: //GBDAC
			set_letters("GBDAC",5);
			break;
		case 4: //GBFC
			set_letters("GBFC",4);
			break;
		case 5: //GDFAC
			set_letters("GDFAC",5);
			break;
		case 6: //GDFEAC
			set_letters("GDFEAC",6);
			break;
		case 7: //BAC
			set_letters("BAC",3);
			break;
		case 8: //GBDFEAC
			set_letters("GBDFEAC",7);
			break;
		case 9: //GBDFAC
			set_letters("GBDFAC",6);
			break;
		case 10: //C
			set_letters("AFED",4);
			break;
		case 11:
			set_letters(":",1);
			break;
		case 12:
			set_letters(".",1);
			break;
		case 13: //upper decimal
			set_letters("U",1);
			break;
		case 14: //dashes
			set_letters("G",1);
		case 15: //A (EFABCG)
			set_letters("EFABCG",6);
			break;
		case 16: //B
			set_letters("GDFEC",6);
			break;
		default:
			break;
	}

	if(decimal == true) set_letters(".",1);
}

void seven_segment_display(float data, int special){
	if(special == DIGIT) {
		//Updates the value of digit only at the start of a display refresh
		if(scan_digit == 1) {	//xxx.x 	format
			//sets digit[0..3] to xxx.x
			digit[0] = (int)data/100;
			digit[1] = (int)data/10-digit[0]*10;
			digit[2] = (int)(data-digit[0]*100-digit[1]*10);
	    digit[3] = (int)(data*10-digit[0]*1000-digit[1]*100-digit[2]*10);
		}	
		//---Update 7 segment---
		seven_seg_reset();
		seven_seg_digit_display(scan_digit, digit[scan_digit-1], scan_digit == 3); //decimal used for 3rd digit
		if(scan_digit++ == 4) scan_digit = 1;
		//----------------------
	} 
	else if(special == DASHES) { //----	dashes
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
		set_letters("G",1);
	}
	else if(special == LETTER) { //   X 	format
		seven_seg_digit_display((int)data, 0, false);
	}
	else if(special == DEGREES) { //xxx.x° 	format
		if(scan_digit == 1) {
			digit[0] = (int)data/100;
			digit[1] = (int)data/10-digit[0]*10;
			digit[2] = (int)(data-digit[0]*100-digit[1]*10);
	  	digit[3] = (int)(data*10-digit[0]*1000-digit[1]*100-digit[2]*10);
		}
		//---Update 7 segment---
		seven_seg_reset();
		seven_seg_digit_display(scan_digit, digit[scan_digit-1], scan_digit == 3); //decimal used for 3rd digit
		if(scan_digit++ == 4) scan_digit = 1;
		//----------------------
	}
	else if(special == CELSIUS) { //xx.x°C 	format
		if(scan_digit == 1) {
			digit[0] = (int)data/100;
			digit[1] = (int)data/10-digit[0]*10;
			digit[2] = (int)(data-digit[0]*100-digit[1]*10);
	  	digit[3] = C;
		}	
		//---Update 7 segment---
		seven_seg_reset();
		seven_seg_digit_display(scan_digit, digit[scan_digit-1], scan_digit == 2); //decimal used for 2nd digit
		if(scan_digit++ == 4) scan_digit = 1;
		//----------------------
	}
	//TODO update to display letters
	else if(special == KEYS) {
		//Updates the value of digit only at the start of a display refresh
		if(scan_digit == 1) {	//xxx.x 	format
			//sets digit[0..3] to xxx.x
			digit[0] = (int)data/100;
			digit[1] = (int)data/10-digit[0]*10;
			digit[2] = (int)(data-digit[0]*100-digit[1]*10);
	  	digit[3] = (int)(data*10-digit[0]*1000-digit[1]*100-digit[2]*10);
		}	
		//---Update 7 segment---
		seven_seg_reset();
		seven_seg_digit_display(scan_digit, digit[scan_digit-1], false);
		if(scan_digit++ == 4) scan_digit = 1;
		//----------------------
	}
	
}

//Brief: refresh display with user specified input;
//Params: None
//Return: None
void seven_segment_refresh(void) {
	
	switch(display_state){
		case INIT:
			special = DASHES;
			display_val = 1;
			break;
		case ACCEL_PITCH:
			special = DEGREES;
		  osSemaphoreWait(sem_accel, osWaitForever);
			display_val = accel_data_pitch;
		  osSemaphoreRelease(sem_accel);
			break;
		case ACCEL_ROLL:
			special = DEGREES;
			display_val = accel_data_roll;
			break;
		case TEMP:
			special = CELSIUS;
			display_val = temp_data;
			break;
		case KEYPAD: 
			special = KEYS;
			display_val = key_data;
			break;
		default:
			break;
	}
	
	if(temp_data>=30){
		if(blink_flag) {
			seven_segment_display(display_val,special);
		}
		else {
			seven_seg_reset();
		}
	}
	else {
		seven_segment_display(display_val,special);
	}
}