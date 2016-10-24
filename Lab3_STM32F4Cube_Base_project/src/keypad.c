#include <stm32f4xx_hal.h>
#include "keypad.h"
#include <supporting_functions.h>

int delay_flag = 0;

void keypad_init(void){
	GPIO_InitTypeDef GPIO_InitDefDOut;
	GPIO_InitTypeDef GPIO_InitDefDIn;
    CLK_KEYPAD_PORT_CLK_EN;
	
	GPIO_InitDefDOut.Pin = GPIO_PIN_0|GPIO_PIN_8|GPIO_PIN_9; //1,6,7
	GPIO_InitDefDOut.Mode = GPIO_MODE_OUTPUT_PP;   			
	GPIO_InitDefDOut.Pull = GPIO_NOPULL;
	GPIO_InitDefDOut.Speed = GPIO_SPEED_FREQ_LOW;	
	
	GPIO_InitDefDIn.Pin = GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_7; //2,3,4,5
	GPIO_InitDefDIn.Mode = GPIO_MODE_INPUT;   			
	GPIO_InitDefDIn.Pull = GPIO_PULLDOWN;
	GPIO_InitDefDIn.Speed = GPIO_SPEED_FREQ_LOW;	
	
	HAL_GPIO_Init(GPIOD, &GPIO_InitDefDOut);
	HAL_GPIO_Init(GPIOD, &GPIO_InitDefDIn);
	
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_RESET);
}

void keypad_scan(void){

		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_SET);
	
		int key_pressed = HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_1) && HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_2) && HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_3) && HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_7);
	  	//printf("%d%d%d%d\n", HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_3) ,HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_7) ,HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_8) ,HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_9) );
		//printf("%d",!key_pressed);
		if(key_pressed) {
			//printf("key pressed\n");
			//get_digit();
		}
		get_digit();

		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_RESET); //1
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_RESET); //6
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_RESET); //7
}

unsigned long long pow(int b,int e) {
	unsigned long long d = 1;
	if(e == 0) d = 0;
	for(int i=0; i<e; i++) {
		d=d*b;
	}
	return d;
}

int get_digit(){

			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_RESET);
			int val[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
			int key[12] = {0,0,0,0,0,0,0,0,0,0,0,0};

			//printf("start scan\n");
			for(int i=0; i<1; i++) {
				//write pin 1
				// check 1,4,7,*
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_SET); //1
				val[3] += HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_1); //2
				val[6] += HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_2); //4
				val[9] += HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_3); //6
				val[11]+= HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_7); //7
				//printf("%d%d%d%d", HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_3) ,HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_7) ,HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_8) ,HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_9) );
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_RESET);
			
				// check 2,5,8,0
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_SET); //6
				val[1] += HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_1);  //2
				val[4] += HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_2); //4
				val[9] += HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_3);  //6
				val[10]+= HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_7);  //7
				//printf("%d%d%d%d\n", HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_3) ,HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_7) ,HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_8) ,HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_9) );
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_RESET);
			
				// check 3,6,9,#
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_SET); //7
				val[2] += HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_1);  //2
				val[5] += HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_2); //4
				val[8] += HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_3);  //6
				val[0] += HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_7);  //7
				//printf("%d%d%d%d\n", HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_3) ,HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_7) ,HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_8) ,HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_9) );
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_RESET);
			}
			//printf("end scan\n");
			unsigned long long int val_int = 0;
			for(int i=0;i<12;i++) {
				val_int += val[i]*pow(10,i);
			}
			//printf("%llu\n",val_int);

			//printf("%d\n",((unsigned long long)(10010000000ll) == val_int));

			key[0]  = val[11];
			key[1]  = val[7] & val[10];
			key[2]  = val[6] & val[9];
			key[3]  = val[5] & val[8];
			key[4]  = val[2] & val[7] & val[10];
			key[5]  = val[6] & val[9];
			key[6]  = val[5] & val[8];
			key[7]  = val[2] & val[7];
			key[8]  = val[9] & val[6] & val[3];
			key[9]  = val[8] & val[5];
			key[10] = val[1];
			key[11] = val[0] & val[8];
			
			int n = 0;
			n += 0*(val[11]						);
			n += 1*(val[7] & val[10]				);
			n += 2*(val[6] & val[9]				);
			n += 3*(val[5] & val[8]				);
			n += 4*(val[2] & val[7] & val[10]		);
			n += 5*(val[6] & val[9]				);
			n += 6*(val[5] & val[8]				);
			n += 7*(val[2] & val[7]				);
			n += 8*(val[9] & val[6] & val[3]		);
			n += 9*(val[8] & val[5]				);
			n += 10*(val[1]							);
			n += 11*(val[0] & val[8]				);
// 100000000000ll 0
// 010010000000ll 1 
// 001001000000ll 2
// 000100100000ll 3
// 010010000100ll 4
// 001001000000ll 5
// 000100100000ll 6
// 000010000100ll 7
// 001001001000ll 8
// 000100100000ll 9
// 000000000010ll 10 *
// 000100000001ll 11 #


			delay_flag = 1;
			printf("%d\n",n);
			for(int i=0; i<12 ;i++){
				printf("%d",val[i]);
			}
			printf("\n");

			return 0;
}


