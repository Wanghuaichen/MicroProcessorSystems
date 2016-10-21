#include <stm32f4xx_hal.h>
#include "keypad.h"
#include <supporting_functions.h>

int delay_flag = 0;

void keypad_init(void){
	GPIO_InitTypeDef GPIO_InitDefDOut;
	GPIO_InitTypeDef GPIO_InitDefDIn;
  CLK_KEYPAD_PORT_CLK_EN;
	
	GPIO_InitDefDOut.Pin = C1|C2|C3;
	GPIO_InitDefDOut.Mode = GPIO_MODE_OUTPUT_PP;   			
	GPIO_InitDefDOut.Pull = GPIO_NOPULL;
	GPIO_InitDefDOut.Speed = GPIO_SPEED_FREQ_LOW;	
	
	GPIO_InitDefDIn.Pin = R1|R2|R3|R4;
	GPIO_InitDefDIn.Mode = GPIO_MODE_INPUT;   			
	GPIO_InitDefDIn.Pull = GPIO_NOPULL;
	GPIO_InitDefDIn.Speed = GPIO_SPEED_FREQ_LOW;	
	
	HAL_GPIO_Init(GPIOD, &GPIO_InitDefDOut);
	HAL_GPIO_Init(GPIOD, &GPIO_InitDefDIn);
	
	HAL_GPIO_WritePin(GPIOD, C1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, C2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, C3, GPIO_PIN_RESET);
}

void keypad_scan(void){
	
	int val[8];
	int i=0;
	
	keypad_init();


		HAL_GPIO_WritePin(GPIOD, C1, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD, C2, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD, C3, GPIO_PIN_SET);
	
		int key_pressed = HAL_GPIO_ReadPin(GPIOD,R1) && HAL_GPIO_ReadPin(GPIOD,R2) && HAL_GPIO_ReadPin(GPIOD,R3) && HAL_GPIO_ReadPin(GPIOD,R4);
	  	//printf("%d%d%d%d\n", HAL_GPIO_ReadPin(GPIOD,R1) ,HAL_GPIO_ReadPin(GPIOD,R2) ,HAL_GPIO_ReadPin(GPIOD,R3) ,HAL_GPIO_ReadPin(GPIOD,R4) );
		//printf("%d",!key_pressed);
		if(!key_pressed) {
			printf("key pressed\n");
			get_digit();
		}

		HAL_GPIO_WritePin(GPIOD, C1, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD, C2, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD, C3, GPIO_PIN_RESET);
}

int get_digit(){

			HAL_GPIO_WritePin(GPIOD, C1, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD, C2, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD, C3, GPIO_PIN_RESET);
			int val[12] = {0,0,0,0,0,0,0,0,0,0,0,0};

			for(int i=0; i<50; i++) {
				// check 1,4,7,*
				HAL_GPIO_WritePin(GPIOD, C1, GPIO_PIN_SET);
				val[1] |= !HAL_GPIO_ReadPin(GPIOD,R1);
				val[4] |= !HAL_GPIO_ReadPin(GPIOD,R2);
				val[7] |= !HAL_GPIO_ReadPin(GPIOD,R3);
				val[10] |= !HAL_GPIO_ReadPin(GPIOD,R4);
				HAL_GPIO_WritePin(GPIOD, C1, GPIO_PIN_RESET);
			
				// check 2,5,8,0
				HAL_GPIO_WritePin(GPIOD, C2, GPIO_PIN_SET);
				val[2] |= !HAL_GPIO_ReadPin(GPIOD,R1);
				val[5] |= !HAL_GPIO_ReadPin(GPIOD,R2);
				val[8] |= !HAL_GPIO_ReadPin(GPIOD,R3);
				val[0] |= !HAL_GPIO_ReadPin(GPIOD,R4);
				HAL_GPIO_WritePin(GPIOD, C2, GPIO_PIN_RESET);
			
				// check 3,6,9,#
				HAL_GPIO_WritePin(GPIOD, C3, GPIO_PIN_SET);
				val[3] |= !HAL_GPIO_ReadPin(GPIOD,R1);
				val[6] |= !HAL_GPIO_ReadPin(GPIOD,R2);
				val[9] |= !HAL_GPIO_ReadPin(GPIOD,R3);
				val[11] |= !HAL_GPIO_ReadPin(GPIOD,R4);
				HAL_GPIO_WritePin(GPIOD, C3, GPIO_PIN_RESET);
			}
			for(int i=0; i<12 ;i++){
				printf("%d",val[i]);
			}
			printf("\n");
			delay_flag = 1;
			
			return 0;
}
