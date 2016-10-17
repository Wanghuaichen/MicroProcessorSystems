#include <stm32f4xx_hal.h>
#include <supporting_functions.h>
#include <gpio_clk_display.h>

/*Brief: Sets up the desired(refer to header) GPIO pin to output mode
**			 and initializes its value to 0
**Params: None
**Return: None
*/
void gpio_clk_display_init(void) {
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

/*Brief: Toggles the value of the desired pin
**Params: None
**Return: None
*/
/*void digitize(float*ADC_val,int *digit) {
	
	digit[0] = (int)ADC_val/10;
	digit[1] = (int)ADC_val-digit[0]*10;
	digit[2] = '.';
	digit[3] = (int)ADC_val*10-digit[1]*10-digit[0]*100;
	
	//printf("7seg value: %d %d %d\n",digit[0],digit[1],digit[2]); //for debug
}*/

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
		case 10:
			set_letters("AFED",4);
			break;
		case 11:
			set_letters(":",1);
			break;
		case 12:
			set_letters(".",1);
			break;
		case 13:
			set_letters("U",1);
			break;
		default:
			break;
	}

	if(decimal == true) set_letters(".",1);
}

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

void seven_seg_reset() {
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



	/* 0
	HAL_GPIO_TogglePin(CLK_PORT, CLK_PIN16);//B	//A-14, B-16, C-13, D-3
	HAL_GPIO_TogglePin(CLK_PORT, CLK_PIN3);	//D	//E-5 , F-11. G-15. DP1 - 7
	HAL_GPIO_TogglePin(CLK_PORT, CLK_PIN5);	//E
	HAL_GPIO_TogglePin(CLK_PORT, CLK_PIN14);//A
	HAL_GPIO_TogglePin(CLK_PORT, CLK_PIN13);//C*/

 	//number 1
	/*
	HAL_GPIO_TogglePin(CLK_PORT, CLK_PIN13);
	HAL_GPIO_TogglePin(CLK_PORT, CLK_PIN16);*/

	/*number 2
	HAL_GPIO_TogglePin(CLK_PORT, CLK_PIN1);
	HAL_GPIO_TogglePin(CLK_PORT, CLK_PIN2);
	HAL_GPIO_TogglePin(CLK_PORT, CLK_PIN6);
	HAL_GPIO_TogglePin(CLK_PORT, CLK_PIN8);
	HAL_GPIO_TogglePin(CLK_PORT, CLK_PIN13);
	HAL_GPIO_TogglePin(CLK_PORT, CLK_PIN16);*/
	/* 2
	HAL_GPIO_TogglePin(CLK_PORT, CLK_PIN15);//G
	HAL_GPIO_TogglePin(CLK_PORT, CLK_PIN16);//B
	HAL_GPIO_TogglePin(CLK_PORT, CLK_PIN3);	//D
	HAL_GPIO_TogglePin(CLK_PORT, CLK_PIN5);	//E
	HAL_GPIO_TogglePin(CLK_PORT, CLK_PIN14);//A	*/

	/* 7
	HAL_GPIO_TogglePin(CLK_PORT, CLK_PIN16);//B
	HAL_GPIO_TogglePin(CLK_PORT, CLK_PIN14);//A
	HAL_GPIO_TogglePin(CLK_PORT, CLK_PIN13);//C	*/
	
	/* 8
	HAL_GPIO_TogglePin(CLK_PORT, CLK_PIN15);//G
	HAL_GPIO_TogglePin(CLK_PORT, CLK_PIN16);//B
	HAL_GPIO_TogglePin(CLK_PORT, CLK_PIN3);	//D
	HAL_GPIO_TogglePin(CLK_PORT, CLK_PIN11);//F
	HAL_GPIO_TogglePin(CLK_PORT, CLK_PIN5);	//E
	HAL_GPIO_TogglePin(CLK_PORT, CLK_PIN14);//A
	HAL_GPIO_TogglePin(CLK_PORT, CLK_PIN13);//C	*/
	
	/*HAL_GPIO_TogglePin(CLK_PORT, CLK_PIN1);
	HAL_GPIO_TogglePin(CLK_PORT, CLK_PIN2);
	HAL_GPIO_TogglePin(CLK_PORT, CLK_PIN3);
	HAL_GPIO_TogglePin(CLK_PORT, CLK_PIN4);
	HAL_GPIO_TogglePin(CLK_PORT, CLK_PIN5);
	HAL_GPIO_TogglePin(CLK_PORT, CLK_PIN6);
	HAL_GPIO_TogglePin(CLK_PORT, CLK_PIN7);
	HAL_GPIO_TogglePin(CLK_PORT, CLK_PIN8);
	HAL_GPIO_TogglePin(CLK_PORT, CLK_PIN9);
	HAL_GPIO_TogglePin(CLK_PORT, CLK_PIN10);
	HAL_GPIO_TogglePin(CLK_PORT, CLK_PIN11);
	HAL_GPIO_TogglePin(CLK_PORT, CLK_PIN12);
	HAL_GPIO_TogglePin(CLK_PORT, CLK_PIN13);
	HAL_GPIO_TogglePin(CLK_PORT, CLK_PIN14);
	HAL_GPIO_TogglePin(CLK_PORT, CLK_PIN15);
	HAL_GPIO_TogglePin(CLK_PORT, CLK_PIN16);*/
