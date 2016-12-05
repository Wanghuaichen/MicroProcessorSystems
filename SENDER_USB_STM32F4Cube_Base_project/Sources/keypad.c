////////////////////////////////////////////////////////////////////////////////
//	File Name					: keypad.c
//	Description				: os thread that contains the function for monitoring 
//                      mouse right click, and scroll
//	Author						: Zeyad Saleh    -260556530
//                      Mahmood Hegazy -260580124
//                      Alex Bhandari  -260520610
//                      Tianming Zhang -260528705
//	Date							: Dec 5, 2016
////////////////////////////////////////////////////////////////////////////////

//include
#include "keypad.h"
#include <cmsis_os.h>

//Globles
int row;
int col;

int scroll_btn_Pressed;
int right_btn_Pressed;

//Brief:  initialize keypad row
//Params:	None
//Return:	None
void initKeypadrow () {			
	__HAL_RCC_GPIOD_CLK_ENABLE();
	
	//initialize the rows as inputs with 1s and columns as outputs with 0, 
	GPIO_InitTypeDef gpio_init_s;

		// Columns
		gpio_init_s.Pin = COLUMN_1 | COLUMN_2 | COLUMN_3;
		gpio_init_s.Mode = GPIO_MODE_INPUT; // Set as INPUT
		gpio_init_s.Speed = GPIO_SPEED_FREQ_VERY_HIGH; // Don't limit slew rate
		gpio_init_s.Pull = GPIO_PULLUP; 		//set to 1
		HAL_GPIO_Init(GPIOD,&gpio_init_s);
	
		// Rows
		gpio_init_s.Pin = RIGHT_BTN | SCROLL_BTN;
		gpio_init_s.Mode = GPIO_MODE_OUTPUT_PP; // Set as OUTPUT
		gpio_init_s.Pull = GPIO_NOPULL ; 				// Don't pull, Make it prone to external setting (don't pull it to 0 or 1)
		HAL_GPIO_Init(GPIOD,&gpio_init_s);
}

//Brief:  initialize keypad column
//Params:	None
//Return:	None
void initKeypadcol () {
	__HAL_RCC_GPIOD_CLK_ENABLE();
	
	GPIO_InitTypeDef gpio_init_s;

		// ROWS
		gpio_init_s.Pin = RIGHT_BTN | SCROLL_BTN;
		gpio_init_s.Mode = GPIO_MODE_INPUT; 	// Set as INPUT
		gpio_init_s.Speed = GPIO_SPEED_FREQ_VERY_HIGH; // Don't limit slew rate
		gpio_init_s.Pull = GPIO_PULLUP; 			//set to 1
		HAL_GPIO_Init(GPIOD,&gpio_init_s);
	
	
		//COLUMNS
		gpio_init_s.Pin = COLUMN_1 | COLUMN_2 | COLUMN_3;
		gpio_init_s.Mode = GPIO_MODE_OUTPUT_PP; // Set as OUTPUT
		gpio_init_s.Pull = GPIO_NOPULL ; 				// Don't pull, Make it prone to external setting (don't pull it to 0 or 1)
		HAL_GPIO_Init(GPIOD,&gpio_init_s);
}

//Brief:  keypad thread 
//Params:	None
//Return:	None
void keypad_thread(void const *argument){
	int key;
	while(1){
		osSignalWait (0x00000001, osWaitForever);
		key = getKeypadValue();
	}
}

//Brief:  monitor if scroll button is pressed
//Params:	None
//Return:	int last
int isScrollPressed(){
	//printf("last scroll = %d\n", scroll_btn_Pressed);
	int last =  scroll_btn_Pressed;
	scroll_btn_Pressed = 0;
	return last;
}

//Brief:  monitor if right button is pressed
//Params:	None
//Return:	int last
int isRightPressed(){
	//printf("last right = %d\n", scroll_btn_Pressed);
	int last =  right_btn_Pressed;
	right_btn_Pressed = 0;
	return last;
}

//Brief:  key debouce function
//Params:	None
//Return:	int last reading value
int getDebouncedKeypadValue(){
	int i;
	int reading = getKeypadValue ();		//Read the current keypress
	int lastReading = reading;					//Set the last reading as the current reading
	
	for (i = 0; i< DEBOUNCER_SIZE; i++){
		reading = getKeypadValue();				//Get a new reading from the keypad
		if (lastReading != reading){			//If it doesn't match the last reading then return -1
			return -1;
		}
		lastReading = reading;						//But if it does for Debouncer_size times, return the value
		HAL_Delay(DEBOUNCE_DELAY);				//Wait before taking another sample
	}
	printf("%d\n", lastReading);
	return lastReading;
}

//Brief:  search the rows for the row in which the button is being pressed
//Params:	None
//Return:	int 1
int getKeypadValue () {
	
	// search the rows for the row in which the button is being pressed	
		
	initKeypadcol ();
	//printf("init Cols\n");
	if(!HAL_GPIO_ReadPin(GPIOD, RIGHT_BTN)) {//if check row 1 is 0 then its the one being pressed
		//printf("MOUSE: Right BTN pressed\n");
		right_btn_Pressed = 1;
	}
	else {
		right_btn_Pressed = 0;
	}
	if(!HAL_GPIO_ReadPin(GPIOD, SCROLL_BTN)) {//if check row 1 is 0 then its the one being pressed
		//printf("MOUSE: Scroll BTN pressed\n");
		scroll_btn_Pressed = 1;
	}
	else {
		scroll_btn_Pressed = 0;
	}
	
	return 1;
		
}

//Brief:  set row
//Params:	None
//Return:	None
void setRows(){
	HAL_GPIO_WritePin(GPIOD, RIGHT_BTN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, SCROLL_BTN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, ROW_3, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, ROW_4, GPIO_PIN_SET);
}

//Brief:  reset row
//Params:	None
//Return:	None
void resetRows(){
	HAL_GPIO_WritePin(GPIOD, RIGHT_BTN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, SCROLL_BTN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, ROW_3, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, ROW_4, GPIO_PIN_RESET);
}

//Brief:  set columns
//Params:	None
//Return:	None
void setColumns(){
	HAL_GPIO_WritePin(GPIOD, COLUMN_1, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, COLUMN_2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, COLUMN_3, GPIO_PIN_SET);
}

//Brief:  reset columns
//Params:	None
//Return:	None
void resetColumns(){
	HAL_GPIO_WritePin(GPIOD, COLUMN_1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, COLUMN_2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, COLUMN_3, GPIO_PIN_RESET);
}


