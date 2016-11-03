#include <math.h>
#include "modules/keypad.h"
#include "seven_segment.h"
#include "interfaces/keypad.h"
#include "utils/utils.h"
#include "system_init.h"

int key_data = 999;
int first = 1;
int special = DASHES;
int key_count = 0;
int value = 0;
float display_val = 0;
extern bool keypad_scan_flag;
int key[4]; //selection plus value plus enter
Keypad_State keypad_state = K_INIT;
Keypad_State state_mem = 0;

/*Brief: Get key on timer flag high. If no key is pressed the value of key_data will not be updated.
**Params: None
**Return: None
*/
void keypad_get_key(void) {

	switch(keypad_state) {
		case K_INIT:
			keypad_state = K_SEL;
			printf("Keypad Initialized\n");
			break;
		
		case K_SEL:
			if(first) {
				printf("Please select piezo with A or tilt with B on the keypad\n");
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


void keypad_select() {
	key[0] = get_key();
	if(monitor_for_change((int)key[0],&mem[MEM_KEY])) {
		switch(key[0]) {
			case 999:
				break;
			case KEY_A:
				special = 0;
				display_val = 0;
				printf("Key pressed: %d\n", key[0]);
				printf("Enter a value for temperature\n");
				keypad_state = K_INPUT;
				//state_mem = K_PIEZO;
				break;
			case KEY_B:
				special = 0;
				display_val = 0;
				printf("Key pressed: %d\n", key[0]);
				printf("Enter a value for target tilt\n");
				keypad_state = K_INPUT;
				//state_mem = K_TILT;
				break;
			default:
				printf("Key pressed: %d is not valid. Please select A or B.\n", key[0]);
				break;
		}
	}
}

void keypad_input() {
	key[key_count] = get_key();
	if(monitor_for_change((int)key[key_count],&mem[MEM_KEY])) {
		if(key[key_count]!=999) {
			printf("Key pressed: %d\n", key[key_count]);
			if(key[key_count] < 10) { 
				display_val = display_val*10 + key[key_count];
				printf("Taget value: %f\n", display_val);
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
				printf("Taget value: %f\n", value);
			}
			else {
				printf("Invalid key pressed: %d. Please use numbers only.\n", key[key_count]);
			}
		}
	}
}

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
