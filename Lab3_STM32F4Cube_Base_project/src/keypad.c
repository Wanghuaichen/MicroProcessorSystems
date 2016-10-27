#include <stm32f4xx_hal.h>
#include "keypad.h"
#include <supporting_functions.h>
#include "bool.h"
#include "utils.h"

int delay_flag = 0;
bool keypad_scan_flag = 0;
bool acc_flag = 0;
/*Brief: Initialize keypad
**Params: None
**Return: None
*/
void keypad_init(void){
    __HAL_RCC_GPIOD_CLK_ENABLE();
    init_read_cols();
	  keypad_scan_flag=1;

    // enable interupts on timers
	/*HAL_NVIC_SetPriority(EXTI0_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
	HAL_NVIC_SetPriority(EXTI1_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ(EXTI1_IRQn);
	HAL_NVIC_SetPriority(EXTI2_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ(EXTI2_IRQn);
	HAL_NVIC_SetPriority(EXTI3_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ(EXTI3_IRQn);*/

}
/*Brief: Returns the key pressed
**Params: None
**Return: unsigned short key
*/
unsigned short get_key(void){
		unsigned short cols;
		unsigned short rows;
		for(int i=0;i<DEBOUNCE_RATE;i++) {
			init_read_cols();
			//--read cols--
			cols = 	HAL_GPIO_ReadPin(GPIOD,C0)<<0 | //C0,C1,C2,C3
					HAL_GPIO_ReadPin(GPIOD,C1)<<1 |
					HAL_GPIO_ReadPin(GPIOD,C2)<<2 |
					HAL_GPIO_ReadPin(GPIOD,C3)<<3;
	
			//if(monitor_for_change((int)cols,&mem[0])) printf("cols: %d%d%d%d\n",HAL_GPIO_ReadPin(GPIOD,C0),HAL_GPIO_ReadPin(GPIOD,C1),HAL_GPIO_ReadPin(GPIOD,C2),HAL_GPIO_ReadPin(GPIOD,C3));
			if(cols == 15) return 999;
	
			//--prep rows--
			init_read_rows();
			//--read rows--
			rows = 	HAL_GPIO_ReadPin(GPIOD,R0)<<0 | //R0,R1,R2,R3
					HAL_GPIO_ReadPin(GPIOD,R1)<<1 |
					HAL_GPIO_ReadPin(GPIOD,R2)<<2 |
					HAL_GPIO_ReadPin(GPIOD,R3)<<3; 
			//if(monitor_for_change((int)rows,&mem[2])) printf("rows: %d%d%d%d\n",HAL_GPIO_ReadPin(GPIOD,R0),HAL_GPIO_ReadPin(GPIOD,R1),HAL_GPIO_ReadPin(GPIOD,R2),HAL_GPIO_ReadPin(GPIOD,R3));
			if(monitor_for_change((int)cols,&mem[0]) || monitor_for_change((int)rows,&mem[2])) return 999;
		}
		//--get digit pressed--
		unsigned short digit;
		switch(cols<<4 | rows) {
			case 238:
				digit = 1;
				break;
			case 237:
				digit = 4;
				break;
			case 235:
				digit = 7;
				break;
			case 231:
				digit = 'A';
				break;
			case 222:
				digit = 2;
				break;
			case 221:
				digit = 5;
				break;
			case 219:
				digit = 8;
				break;
			case 215:
				digit = 0;
				break;
			case 190:
				digit = 3;
				break;
			case 189:
				digit = 6;
				break;
			case 187:
				digit = 9;
				break;
			case 183:
				digit = 'B';
				break; 
			case 126:
				digit = 'C';
				break;
			case 125:
				digit = 'D';
				break;
			case 123:
				digit = 'E';
				break;
			case 119:
				digit = 'F';
				break;
			default:
				break;
		}
		//if(monitor_for_change((int)digit,&mem[1])) printf("Digit: %d\n", digit);
		//--reset to read cols--
		init_read_cols();	
		return digit;
}
/*Brief: Inits the pins for col read
**Params: None
**Return: None
*/
void init_read_cols(void) {
	GPIO_InitTypeDef GPIO_InitDefDOut;
	GPIO_InitTypeDef GPIO_InitDefDIn;

	//set columns pull high
	GPIO_InitDefDIn.Pin = C0|C1|C2|C3;
	GPIO_InitDefDIn.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitDefDIn.Pull = GPIO_PULLUP;
	GPIO_InitDefDIn.Speed = GPIO_SPEED_FREQ_LOW;	
	
	GPIO_InitDefDOut.Pin = R0|R1|R2|R3;
	GPIO_InitDefDOut.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitDefDOut.Pull = GPIO_NOPULL;
	GPIO_InitDefDOut.Speed = GPIO_SPEED_FREQ_LOW;	
	
	HAL_GPIO_Init(GPIOD, &GPIO_InitDefDIn);
	HAL_GPIO_Init(GPIOD, &GPIO_InitDefDOut);

	//set rows to low
	HAL_GPIO_WritePin(GPIOD, R0|R1|R2|R3, GPIO_PIN_RESET);
	//when button is pressed columns will be pulled down by rows
}
/*Brief: Inits the pins for row read
**Params: None
**Return: None
*/
void init_read_rows(void) {
	GPIO_InitTypeDef GPIO_InitDefDOut;
	GPIO_InitTypeDef GPIO_InitDefDIn;

	//set rows pull high
	GPIO_InitDefDIn.Pin = R0|R1|R2|R3;
	GPIO_InitDefDIn.Mode = GPIO_MODE_INPUT;
	GPIO_InitDefDIn.Pull = GPIO_PULLUP;
	GPIO_InitDefDIn.Speed = GPIO_SPEED_FREQ_LOW;	
	
	GPIO_InitDefDOut.Pin = C0|C1|C2|C3;
	GPIO_InitDefDOut.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitDefDOut.Pull = GPIO_NOPULL;
	GPIO_InitDefDOut.Speed = GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Init(GPIOD, &GPIO_InitDefDIn);
	HAL_GPIO_Init(GPIOD, &GPIO_InitDefDOut);

	//set columns to low
	HAL_GPIO_WritePin(GPIOD, C0|C1|C2|C3, GPIO_PIN_RESET);
	//when button is pressed rows will be pulled down by columns
}