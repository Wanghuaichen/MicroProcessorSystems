#include "game.h"
#include "main.h"

volatile int systick_flag;
extern int delay_flag;
volatile int systick_flag; 
extern float piezo_max;
extern float piezo_counter;
extern TIM_HandleTypeDef TimerStructPiezo;
extern TIM_HandleTypeDef TimerStruct7seg;
extern bool keypad_scan_flag;
extern bool piezo_tim_flag;
extern bool seg_tim_flag;
extern bool acc_flag;
unsigned short key[4]; //selection plus value plus enter
extern int mem[3];
float display;
Game_State state = INIT;
Game_State state_mem = INIT;
int key_count = 0;

void play(void) {

	float out[3];
  	float pitch, roll;

	switch(state) {
		case INIT:
			system_init();
			state = SEL;
			printf("Game Initialized\n");
			break;
		case SEL:
			printf("Please select piezo with A or tilt with B on the keypad\n");
			if(keypad_scan_flag) {
				keypad_scan_flag = 0;
				key[0] = get_key();
				if(monitor_for_change((int)key[0],&mem[1])) {
					switch(key[0]) {
						case 999:
							break;
						case KEY_A:
							printf("Key pressed: %d\n", key[0]);
							printf("Enter a value for target piezo strike\n");
							state = INPUT;
							state_mem = PIEZO;
							break;
						case KEY_B:
							printf("Key pressed: %d\n", key[0]);
							printf("Enter a value for target tilt\n");
							state = INPUT;
							state_mem = TILT;
							break;
						default:
							printf("Key pressed: %d is not valid. Please select A or B.\n", key[0]);
							break;
					}
				}
			}
			break;
		case INPUT:
			if(keypad_scan_flag) {
				keypad_scan_flag = 0;
				key[key_count] = get_key();
				if(monitor_for_change((int)key[key_count],&mem[1])) {
					if(key[key_count]!=999) {
						printf("Key pressed: %d\n", key[key_count]);
						if(key[key_count] < 10) { 
							if(key[key_count] == KEY_POUND) {
								state = state_mem;
								value = 100*key[2] + 10*key[1] + key[0];
							}
							key_count++;
							if(key_count == 4) { //overwrite values
								key = {0,0,0,0};
								key_count = 0;
							}
						}
						else {
							printf("Invalid key pressed: %c. Please use numbers only.\n", key[key_count]);
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
				if(monitor_for_change(piezo_peak(),mem[2]) && piezo_peak() > 5) {
					display = piezo_peak();
					printf("Force: %d Target: %d Diff: %d",piezo_peak(),value,value-piezo_peak());
				}	
			}
			if(keypad_scan_flag) {
				keypad_scan_flag = 0;
				key[0] = get_key();
				if(monitor_for_change((int)key[0],&mem[1])) {
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
				if(monitor_for_change(piezo_peak(),mem[2])) {
					display = abs(pitch);
					printf("Tilt: %d Target: %d Diff: %d",pitch,value,value-tilt);
				}
			}	
			if(keypad_scan_flag) {
				keypad_scan_flag = 0;
				key[0] = get_key();
				if(monitor_for_change((int)key[0],&mem[1])) {
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
		case RESET:
			state = INIT;
			break;
	}

	//updates
	if(seg_tim_flag) {
		seg_tim_flag = 0;
		display(display);
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