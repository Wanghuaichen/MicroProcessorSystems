////////////////////////////////////////////////////////////////////////////////
//	File Name					: keypad_thread.c
//	Description				: program contains calling function for keypad thread, and
//                      keypad state transitions
//                      get pitch, and roll values
//	Author						: Alex Bhandari, Tianming Zhang 
//	Date							: Nov 6, 2016
////////////////////////////////////////////////////////////////////////////////

//include
#include <cmsis_os.h>
#include "keypad_thread.h"
#include "keypad.h"
#include "utils.h"

extern int special;
extern float display_val;
extern bool keypad_scan_flag;

//Global
int key_data = 999;
int first = 1;
int key_count = 0;
int value = 0;
int key[4]; //selection plus value plus enter

Keypad_State keypad_state = K_INIT;
Keypad_State state_mem = 0;

state display_state=INIT;

osThreadId keypad_thread_ID;
osThreadDef(keypad_thread, osPriorityNormal, 1,0);

//Brief: start keypad thread;
//Params: None
//Return: None
void start_keypad_thread(void *args) {
	keypad_thread_ID = osThreadCreate(osThread(keypad_thread), args);
}

//Brief:		The keypad thread function in the OS
//					Waits for a signal from the TIM2 interrupt handler and refresh the display
//Params:		A void pointer to initial arguments, NULL if unused
//Return:		None
void keypad_thread(void const *args) {
	keypad_init();
	while(1) {
		osSignalWait(0x00000001, osWaitForever);
		keypad_get_key();
	}
}

//Brief: Get key on timer flag high. If no key is pressed the value of key_data will not be updated.
//Params: None
//Return: None
void keypad_get_key(void) {

	switch(keypad_state) {
		case K_INIT:
			keypad_state = K_SEL;
			break;
		
		case K_SEL:
			if(first) {
				first = 0;
			}
			keypad_select();
			break;
			
		case K_INPUT:
			keypad_input();
			break;
		
		case K_POLL_ESCAPE:
			keypad_escape();
			break;
			
		case RESETZ:
			keypad_state = K_INIT;
			break;
	}
}

//Brief:  corresponding display state will be updated corresponding to user input
//Params: None
//Return: None
void keypad_select() {
	key[0] = get_key();
	if(monitor_for_change((int)key[0],&mem[MEM_KEY])) {
		switch(key[0]) {
			case 999:
				break;
			case KEY_A: //for temperature
				special = 0;
				display_val = 0;
				display_state = TEMP;
				break;
			case 1: //for tilt
				special = 0;
				display_val = 0;
				display_state = ACCEL_PITCH;
				break;
			case 2: //for roll
				special = 0;
			  display_val =0;
			  display_state = ACCEL_ROLL;
			default:
				//printf("Key pressed: %d is not valid. Please select A or B.\n", key[0]);
				break;
		}
	}
}

//Brief: receive 3 digit from user input
//Params: None
//Return: None
void keypad_input() {
	key[key_count] = get_key();
	if(monitor_for_change((int)key[key_count],&mem[MEM_KEY])) {
		if(key[key_count]!=999) {
			//printf("Key pressed: %d\n", key[key_count]);
			if(key[key_count] < 10) { 
				display_val = display_val*10 + key[key_count];
				//printf("Taget value: %f\n", display_val);
				key_count++;
				if(key_count == 4) { //overwrite values
					display_val = 0;
					memset(key,0,sizeof key);
					key_count = 0;
				}
			}
			else if (key[key_count] == KEY_POUND) {
				special = 0;
				//value = 100*key[2] + 10*key[1] + key[0];
				value = display_val;
				display_val = 0;
				key[key_count] = 0;
				keypad_state = state_mem;
				//printf("Taget value: %f\n", value);
			}
			else {
				//printf("Invalid key pressed: %d. Please use numbers only.\n", key[key_count]);
			}
		}
	}
}

//Brief: keypad state go back to select state
//Params: None
//Return: None
void keypad_escape() {
	key[0] = get_key();
	if(monitor_for_change((int)key[0],&mem[MEM_KEY])) {
		switch(key[0]) {
			case 999:
				break;
			default:
				keypad_state = K_SEL;
				break;
		}
	}
}