#ifndef _KEYPAD
#define _KEYPAD

#define CLK_KEYPAD_PORT_CLK_EN __HAL_RCC_GPIOD_CLK_ENABLE()

#define DEBOUNCE_RATE 10

//GPIOD:   10,0,9,8,7;3,2,1
//Pin:		0,1,2,3,4,5,6,7

#define C0 (GPIO_PIN_10)
#define C1 (GPIO_PIN_12)
#define C2 (GPIO_PIN_9)
#define C3 (GPIO_PIN_8)
         
#define R0 (GPIO_PIN_7)
#define R1 (GPIO_PIN_3)
#define R2 (GPIO_PIN_2)
#define R3 (GPIO_PIN_1)
                      
void keypad_init(void);
unsigned short get_key(void);
int get_digit(void);
void init_read_cols(void);
void init_read_rows(void);

#endif

 /* Key Map
	1	0,4
	2	1,4
	3	2,4
	A	3,4
	4	0,5
	5	1,5
	6	2,5
	B	3,5
	7	0,6
	8	1,6
	9	2,6
	C	3,6
	*	0,7
	0	1,7
	#	2,7
	D	3,7

row 1: 4
row 2: 5
row 3: 6
row 4: 7

col 1: 0
col 2: 1
col 3: 2
col 4: 7
*/