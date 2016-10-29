#include <stm32f4xx_hal.h>
#include <supporting_functions.h>
#include "seven_segment.h"

int scan_digit = 1;
int digit[5];

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
/*Brief: Display on the 7seg, needs to be polled continuously
**Params: float data, int special
**Return: None
*/
void display_2(float data, int special) {

	if(special == DIGIT) {
		//Updates the value of digit only at the start of a display refresh
		if(scan_digit == 1) {
			//sets digit[0..5] to xx.xUC (degrees C)
			digit[0] = (int)data/100;
			digit[1] = (int)data/10-digit[0]*10;
			digit[2] = (int)(data-digit[0]*100-digit[1]*10);
	  		digit[3] = (int)(data*10-digit[0]*1000-digit[1]*100-digit[2]*10);
			//digit[4] = 13; //upper point LED - degrees sign
		}	
		
		//---Update 7 segment---
		seven_seg_reset();
		seven_seg_digit_display(scan_digit, digit[scan_digit-1], scan_digit == 3); //decimal used for 2nd digit
		if(scan_digit++ == 4) scan_digit = 1;
		//----------------------
	} 
	else if(special == DASHES) { //dashes
		//seven_seg_digit_display(14, digit[scan_digit-1], false); //disp: ----
		//if(scan_digit++ == 4) scan_digit = 1;
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
		set_letters("G",1);
	}
	else if(special == LETTER) {
		seven_seg_digit_display((int)data, 0, false);
	}
	else {
	}
}
/*Brief: Display on the 7seg, needs to be polled continuously
**Params: float data
**Return: None
*/
void display(float data) {
	//Updates the value of digit only at the start of a display refresh
	if(scan_digit == 1) {
		digit[0] = (int)data/100;
		digit[1] = (int)data/10-digit[0]*10;
		digit[2] = (int)(data-digit[0]*100-digit[1]*10);
	  	digit[3] = (int)(data*10-digit[0]*1000-digit[1]*100-digit[2]*10);
		//digit[4] = 13; //upper point LED - degrees sign
		//printf("%d",digit[0]);
		//printf("%d",digit[1]);
		//printf("%d",digit[2]);
		//printf("%d\n",digit[3]);
	}	
	
	//---Update 7 segment---
	seven_seg_reset();
	seven_seg_digit_display(scan_digit, digit[scan_digit-1], scan_digit == 3); //decimal used for 2nd digit
	if(scan_digit++ == 4) scan_digit = 1;
	//----------------------
}
/*Brief: Displays a single digit
**Params: int place, int digit, bool decimal
**Return: None
*/
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
/*Brief: Sets the segments for a given number or letter or special symbol
**Params: char *letters,int length
**Return: None
*/
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
/*Brief: Resets the display
**Params: None
**Return: None
*/
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