#include "keypad.h"
#include "interfaces/keypad.h"

int key_data = 999;
state keypad_state = INIT;
state state_mem = 0;

/*Brief: Get key on timer flag high. If no key is pressed the value of key_data will not be updated.
**Params: None
**Return: None
*/
void keypad_get_key(void) {
}

	switch(keypad_state) {
		case INIT:
			system_init();
			keypad_state = SEL;
			printf("Keypad Initialized\n");
			break;
		case SEL:
			if(first) {
				printf("Please select piezo with A or tilt with B on the keypad\n");
				first = 0;
			}
			key_for_select();
			break;
		case INPUT:
			key_for_input();
			break;
		case PIEZO:
			poll_for_escape();
			break;
		case TILT:
	
			if(1) {
				keypad_scan_flag = 0;
				key[0] = get_key();
				if(monitor_for_change((int)key[0],&mem[MEM_KEY])) {
					switch(key[0]) {
						case 999:
							break;
						default:
							keypad_state = SEL;
							break;
					}
				}
			}
			break;
		case RESETZ:
			keypad_state = INIT;
			break;
	}

void key_for_select() {
	key[0] = get_key();
	if(monitor_for_change((int)key[0],&mem[MEM_KEY])) {
		switch(key[0]) {
			case 999:
				break;
			case KEY_A:
				special = 0;
				display_val = 0;
				printf("Key pressed: %d\n", key[0]);
				printf("Enter a value for target piezo strike\n");
				keypad_state = INPUT;
				state_mem = PIEZO;
				break;
			case KEY_B:
				special = 0;
				display_val = 0;
				printf("Key pressed: %d\n", key[0]);
				printf("Enter a value for target tilt\n");
				keypad_state = INPUT;
				state_mem = TILT;
				break;
			default:
				printf("Key pressed: %d is not valid. Please select A or B.\n", key[0]);
				break;
		}
	}
}

void key_for_input() {
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
				printf("Taget value: %d\n", value);
			}
			else {
				printf("Invalid key pressed: %d. Please use numbers only.\n", key[key_count]);
			}
		}
	}
}

void poll_for_escape() {
	key[0] = get_key();
	if(monitor_for_change((int)key[0],&mem[MEM_KEY])) {
		switch(key[0]) {
			case 999:
				break;
			default:
				keypad_state = SEL;
				break;
		}
	}
}