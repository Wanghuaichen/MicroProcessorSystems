#include "game.h"
#include "main.h"
#include <math.h>
#include "utils.h"

extern int delay_flag;
extern float piezo_max;
extern float piezo_counter;
extern TIM_HandleTypeDef TimerStructPiezo;
extern TIM_HandleTypeDef TimerStruct7seg;
extern bool keypad_scan_flag;
extern bool piezo_tim_flag;
extern bool seg_tim_flag;
extern bool acc_flag;
extern unsigned short key[4]; //selection plus value plus enter
float display_val = 0;
int special = 1;
Game_State state = INIT;
Game_State state_mem = INIT;
int key_count = 0;
float value = 0;
int first = 1;

void play(void) {
	if (first) {
		printf("Game Initialized\n");
		first = 0;
	}

	float out[3];
	float pitch, roll;

	switch(state) {
		case INIT:
			system_init();
			state = SEL;
			for(int i=0;i<5000;i++) printf("Game Initialized\n");
			break;
		case SEL:
			if(first) {
				for(int i=0;i<5000;i++) printf("Please select piezo with A or tilt with B on the keypad\n");
				first = 0;
			}
			if(1) {
				keypad_scan_flag = 0;
				key[0] = get_key();
				if(monitor_for_change((int)key[0],&mem[MEM_KEY])) {
					switch(key[0]) {
						case 999:
							break;
						case KEY_A:
							special = 0;
							display_val = 0;
							for(int i=0;i<5000;i++)printf("Key pressed: %d\n", key[0]);
							for(int i=0;i<5000;i++)printf("Enter a value for target piezo strike\n");
							state = INPUT;
							state_mem = PIEZO;
							break;
						case KEY_B:
							special = 0;
							display_val = 0;
							for(int i=0;i<5000;i++)printf("Key pressed: %d\n", key[0]);
							for(int i=0;i<5000;i++)printf("Enter a value for target tilt\n");
							state = INPUT;
							state_mem = TILT;
							break;
						default:
							for(int i=0;i<5000;i++)printf("Key pressed: %d is not valid. Please select A or B.\n", key[0]);
							break;
					}
				}
			}
			break;
		case INPUT:
			if(1) {
				keypad_scan_flag = 0;
				key[key_count] = get_key();
				if(monitor_for_change((int)key[key_count],&mem[MEM_KEY])) {
					if(key[key_count]!=999) {
						for(int i=0;i<5000;i++)printf("Key pressed: %d\n", key[key_count]);
						if(key[key_count] < 10) { 
							display_val += key[key_count]*10^key_count;
							key_count++;
							if(key_count == 4) { //overwrite values
								display_val = 0;
								memset(key,0,sizeof key);
								key_count = 0;
							}
						}
						else if (key[key_count] == KEY_POUND) {
								special = 0;
								display_val = 0;
								state = state_mem;
								value = 100*key[2] + 10*key[1] + key[0];
						}
						else {
							for(int i=0;i<5000;i++)printf("Invalid key pressed: %d. Please use numbers only.\n", key[key_count]);
						}
					}
				}
			}
			break;
		case PIEZO:
			if(piezo_tim_flag) {
				piezo_tim_flag = 0;
				piezo_adc_poll();
				piezo_peak_update();
				if(monitor_for_change(piezo_peak(),&mem[MEM_PIEZO]) && piezo_peak() > 5) {
					display_val = piezo_peak();
					printf("Force: %f Target: %f Diff: %f\n",piezo_peak(),value,(float)value-piezo_peak());
				}	
			}
			if(1) {
				keypad_scan_flag = 0;
				key[0] = get_key();
				if(monitor_for_change((int)key[0],&mem[MEM_KEY])) {
					switch(key[0]) {
						case 999:
							break;
						default:
							state = SEL;
							break;
					}
				}
			}
			break;
		case TILT:
			//accelerameter
			if(acc_flag) {
				LIS3DSH_ReadACC(out);
				pitch=atan((out[0])/sqrt(pow((out[1]),2)+pow((out[2]),2)))*(180/3.1415926);
				roll=atan((out[1])/sqrt(pow((out[0]),2)+pow((out[2]),2)))*(180/3.1415926);
				//printf("pitch:%f roll:%f \n",pitch,roll);
				acc_flag=0;
				if(monitor_for_change(pitch,&mem[MEM_ACCEL])) {
					display_val = fabsf(pitch);
					printf("Tilt: %f Target: %f Diff: %f\n",pitch,value,value-pitch); //5142904396
				}
			}	
			if(1) {
				keypad_scan_flag = 0;
				key[0] = get_key();
				if(monitor_for_change((int)key[0],&mem[MEM_KEY])) {
					switch(key[0]) {
						case 999:
							break;
						default:
							state = SEL;
							break;
					}
				}
			}
			break;
		case RESETZ:
			state = INIT;
			break;
	}

	//updates
	if(seg_tim_flag) {
		seg_tim_flag = 0;
		display_2(display_val,special);
	}

}

//callback from stm32f4xx_hal_gpio.c
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if(GPIO_Pin == GPIO_PIN_0) {
		acc_flag = 1;
	} else {
	keypad_scan_flag = 1;
	}
}