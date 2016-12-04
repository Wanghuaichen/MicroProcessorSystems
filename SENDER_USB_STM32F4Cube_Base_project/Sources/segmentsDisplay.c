////////////////////////////////////////////////////////////////////////////////
//	File Name					: gpio_example.c
//	Description				: Sample code to help use the GPIO driver
//	Author						: Harsh Aurora
//	Date							: Oct 1st, 2016
////////////////////////////////////////////////////////////////////////////////
	
//		Includes		//
#include <stm32f4xx_hal.h>
#include <supporting_functions.h>
#include <segmentsDisplay.h>
#include <cmsis_os.h>

/*Brief: Sets up the desired(refer to header) GPIO pin to output mode
**			 and initializes its value to 0
**Params: None
**Return: None
*/
int display = 0;
int leds = 0;
float numberToDisplay = 0;

int decimalPoint = 0;
int LSD = 0;
int LSD2 = 0;
int MSD = 0;


osMutexId display_mutex;
osMutexDef(display_mutex);

int clearDisplays = 0;


void display_thread (void const *argument){
	displays_gpio_init();
	
	while(1){
		if (!clearDisplays){
			osSignalWait (0x00000001, osWaitForever);
			display = display % 4;
			displayNumber();
			display++;
		}
		else {
			resetDisplay();
		}
	}
}

void setClearDisplays(int value){
	clearDisplays = value;
}

void displays_gpio_init(void) {
	GPIO_InitTypeDef GPIO_InitDef;
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	
	GPIO_InitDef.Pin = EXAMPLE_PIN; 				//Green_LED
	GPIO_InitDef.Mode = GPIO_MODE_OUTPUT_PP;   			
	GPIO_InitDef.Pull = GPIO_NOPULL;
	GPIO_InitDef.Speed = GPIO_SPEED_FREQ_MEDIUM;		
	HAL_GPIO_Init(EXAMPLE_PORT, &GPIO_InitDef);
	HAL_GPIO_WritePin(EXAMPLE_PORT, EXAMPLE_PIN, GPIO_PIN_RESET);
	
	GPIO_InitDef.Pin = orange_Led; 				//Orange_LED
	HAL_GPIO_Init(GPIOD, &GPIO_InitDef);
	
	GPIO_InitDef.Pin = red_Led; 					//red_LED
	HAL_GPIO_Init(GPIOD, &GPIO_InitDef);
	
	GPIO_InitDef.Pin = blue_Led; 					//blue_LED
	HAL_GPIO_Init(GPIOD, &GPIO_InitDef);
	
  
	GPIO_InitDef.Pin = A_Segment_Pin;		 			//Pin PB12
	HAL_GPIO_Init(GPIOD, &GPIO_InitDef);
	HAL_GPIO_WritePin(GPIOD, A_Segment_Pin, GPIO_PIN_SET);
	
	GPIO_InitDef.Pin = B_Segment_Pin;		 			//Pin PB13
	HAL_GPIO_Init(GPIOB, &GPIO_InitDef);
	HAL_GPIO_WritePin(GPIOB, B_Segment_Pin, GPIO_PIN_SET);
	
	GPIO_InitDef.Pin = C_Segment_Pin;		 			//Pin PB14
	HAL_GPIO_Init(GPIOB, &GPIO_InitDef);
	HAL_GPIO_WritePin(GPIOB, C_Segment_Pin, GPIO_PIN_SET);
	
	GPIO_InitDef.Pin = D_Segment_Pin;		 			//Pin PB15
	HAL_GPIO_Init(GPIOB, &GPIO_InitDef);
	HAL_GPIO_WritePin(GPIOB, D_Segment_Pin, GPIO_PIN_SET);
	
	GPIO_InitDef.Pin = E_Segment_Pin;		 			//Pin PD0
	HAL_GPIO_Init(GPIOD, &GPIO_InitDef);
	HAL_GPIO_WritePin(GPIOD, E_Segment_Pin, GPIO_PIN_SET);
	
	GPIO_InitDef.Pin = F_Segment_Pin;		 			//Pin PD1
	HAL_GPIO_Init(GPIOD, &GPIO_InitDef);
	HAL_GPIO_WritePin(GPIOD, F_Segment_Pin, GPIO_PIN_SET);
	
	GPIO_InitDef.Pin = G_Segment_Pin;		 			//Pin PC11
	HAL_GPIO_Init(GPIOB, &GPIO_InitDef);
	HAL_GPIO_WritePin(GPIOB, G_Segment_Pin, GPIO_PIN_SET);
	
	GPIO_InitDef.Pin = DP_Segment_Pin;		 			//Pin PD11
	HAL_GPIO_Init(GPIOD, &GPIO_InitDef);
	HAL_GPIO_WritePin(GPIOD, DP_Segment_Pin, GPIO_PIN_SET);
	
	GPIO_InitDef.Pin = selectLine0;		 				//Pin PE12
	HAL_GPIO_Init(GPIOE, &GPIO_InitDef);
	
	GPIO_InitDef.Pin = selectLine1;		 				//Pin PE13
	HAL_GPIO_Init(GPIOE, &GPIO_InitDef);
	
	GPIO_InitDef.Pin = selectLine2;		 				//Pin PE14
	HAL_GPIO_Init(GPIOE, &GPIO_InitDef);
	
	GPIO_InitDef.Pin = selectLine3;		 				//Pin PE15
	HAL_GPIO_Init(GPIOE, &GPIO_InitDef);
	HAL_GPIO_WritePin(GPIOE, selectLine3, GPIO_PIN_SET);	//Last digit
}

/*Brief: Toggles the value of the desired pin
**Params: None
**Return: None
*/
void gpio_example(void) {
	HAL_GPIO_TogglePin(EXAMPLE_PORT, EXAMPLE_PIN);
}

void toggleDisplays(int display){
	display = display%4;
	
	if (display == 0){ //Leftmost display
		HAL_GPIO_WritePin(GPIOE, selectLine0, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOE, selectLine1, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOE, selectLine2, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOE, selectLine3, GPIO_PIN_RESET);
	}
	
	else if (display == 1){ 
		HAL_GPIO_WritePin(GPIOE, selectLine0, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOE, selectLine1, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOE, selectLine2, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOE, selectLine3, GPIO_PIN_RESET);
	}
	
	else if (display == 2){
		HAL_GPIO_WritePin(GPIOE, selectLine0, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOE, selectLine1, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOE, selectLine2, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOE, selectLine3, GPIO_PIN_RESET);
	}
	
	else if (display == 3){ //Rightmost display
		HAL_GPIO_WritePin(GPIOE, selectLine0, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOE, selectLine1, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOE, selectLine2, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOE, selectLine3, GPIO_PIN_SET);
	}
}

void setNumberToDisplay(float number){
	numberToDisplay = number;
	decimalPoint = (int)(numberToDisplay * 10)%10 ;
	LSD = (int)(numberToDisplay)%10 ;
	LSD2 = (int)(numberToDisplay / 10)%10 ;
	MSD = (int)(numberToDisplay/100)%10 ;
	
	
}

void displayNumber(){ //At each 1ms iteration, update one of the 4 7-segment displays
	toggleDisplays(display%4);
	
	if (display == 0){
		displayDigit(MSD);
	}
	else if (display == 1){
		displayDigit(LSD2);
	}
	else if (display == 2){
		displayDigit(LSD);
		HAL_GPIO_WritePin(GPIOD, DP_Segment_Pin, GPIO_PIN_SET);
	}
	else if (display == 3){
		displayDigit(decimalPoint);
	}
}

void displayDigit(int digit){	//Digit to segments mapping
	resetDisplay();
	
	switch (digit){
		case 0:
			setSegmentA();
			setSegmentB();
			setSegmentC();
			setSegmentD();
			setSegmentE();
			setSegmentF();
			break;
		case 1:
			setSegmentB();
			setSegmentC();
			break;
		case 2:
			setSegmentA();
			setSegmentB();
			setSegmentD();
			setSegmentE();
			setSegmentG();
			break;
		case 3:
			setSegmentA();
			setSegmentB();
			setSegmentC();
			setSegmentD();
			setSegmentG();
			break;
		case 4:
			setSegmentB();
			setSegmentC();
			setSegmentF();
			setSegmentG();
			break;
		case 5:
			setSegmentA();
			setSegmentC();
			setSegmentD();
			setSegmentF();
			setSegmentG();
			break;
		case 6:
			setSegmentA();
			setSegmentC();
			setSegmentD();
			setSegmentE();
			setSegmentF();
			setSegmentG();
			break;
		case 7:
			setSegmentA();
			setSegmentB();
			setSegmentC();
			break;
		case 8:
			setSegmentA();
			setSegmentB();
			setSegmentC();
			setSegmentD();
			setSegmentE();
			setSegmentF();
			setSegmentG();
			break;
		case 9:
			setSegmentA();
			setSegmentB();
			setSegmentC();
			setSegmentD();
			setSegmentF();
			setSegmentG();
			break;
		default:
			break;
	}
}

void resetDisplay(){
	HAL_GPIO_WritePin(GPIOD, A_Segment_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, B_Segment_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, C_Segment_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, D_Segment_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, E_Segment_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, F_Segment_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, G_Segment_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, DP_Segment_Pin, GPIO_PIN_RESET);
}

void circularLEDs(){
	resetLEDs();
	if(leds == 0){
		setGreenLED();
	}
	else if (leds == 1){
		setOrangeLED();
	}
	else if (leds == 2){
		setRedLED();
	}
	else if (leds == 3){
		setBlueLED();
	}
	
	leds++;
	leds = leds%4;
}

void resetLEDs(){
	resetGreenLED();
	resetOrangeLED();
	resetRedLED();
	resetBlueLED();
}

void setSegmentA(){
	HAL_GPIO_WritePin(GPIOD, A_Segment_Pin, GPIO_PIN_SET);
}
void setSegmentB(){
	HAL_GPIO_WritePin(GPIOB, B_Segment_Pin, GPIO_PIN_SET);
}
void setSegmentC(){
	HAL_GPIO_WritePin(GPIOB, C_Segment_Pin, GPIO_PIN_SET);
}
void setSegmentD(){
	HAL_GPIO_WritePin(GPIOB, D_Segment_Pin, GPIO_PIN_SET);
}
void setSegmentE(){
	HAL_GPIO_WritePin(GPIOD, E_Segment_Pin, GPIO_PIN_SET);
}
void setSegmentF(){
	HAL_GPIO_WritePin(GPIOD, F_Segment_Pin, GPIO_PIN_SET);
}
void setSegmentG(){
	HAL_GPIO_WritePin(GPIOB, G_Segment_Pin, GPIO_PIN_SET);
}
void setSegmentDP(){
	HAL_GPIO_WritePin(GPIOD, DP_Segment_Pin, GPIO_PIN_SET);
}

void setGreenLED(){
	HAL_GPIO_WritePin(GPIOD, EXAMPLE_PIN, GPIO_PIN_SET);
}
void resetGreenLED(){
	HAL_GPIO_WritePin(GPIOD, EXAMPLE_PIN, GPIO_PIN_RESET);
}
void setOrangeLED(){
	HAL_GPIO_WritePin(GPIOD, orange_Led, GPIO_PIN_SET);
}
void resetOrangeLED(){
	HAL_GPIO_WritePin(GPIOD, orange_Led, GPIO_PIN_RESET);
}
void setRedLED(){
	HAL_GPIO_WritePin(GPIOD, red_Led, GPIO_PIN_SET);
}
void resetRedLED(){
	HAL_GPIO_WritePin(GPIOD, red_Led, GPIO_PIN_RESET);
}
void setBlueLED(){
	HAL_GPIO_WritePin(GPIOD, blue_Led, GPIO_PIN_SET);
}
void resetBlueLED(){
	HAL_GPIO_WritePin(GPIOD, blue_Led, GPIO_PIN_RESET);
}

