#include <stdint.h>
#include "stm32f4xx.h"                 
//#include "stm32f4xx_conf.h"


#define STAR 11
#define POUND 12

#define ROWS 4
#define COLUMNS 3

#define RIGHT_BTN  		GPIO_PIN_2		//Pin 2
#define SCROLL_BTN  		GPIO_PIN_3		//Pin 3
#define PIN4  		GPIO_PIN_4
#define PIN5 			GPIO_PIN_5
#define ROW_3  		GPIO_PIN_6		//Pin 4
#define ROW_4  		GPIO_PIN_7		//Pin 5
#define COLUMN_1  GPIO_PIN_8	//Pin 6
#define COLUMN_2  GPIO_PIN_9	//Pin 7
#define COLUMN_3 	GPIO_PIN_10	//Pin 8

#define DEBOUNCER_SIZE 4
#define DEBOUNCE_DELAY 30

int isRightPressed(void);
int isScrollPressed(void);

void keypad_thread(void const *argument);
void initKeypad(void);
void rowAsInput(void) ;
void columnAsInput (void);
void searchColumn(void);
void searchRow(void);
int getKeypadValue(void);
void initKeypadrow(void);
void initKeypadrow(void);
void initKeypadcol(void);
int decodeKeypad(int row, int column);
void probeInput (void);
void setRows(void);
void resetRows(void);
void setColumns(void);
void resetColumns(void);
int probeOutput(int column);
void setRow(int row);
int getDebouncedKeypadValue(void);