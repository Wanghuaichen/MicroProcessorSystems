#ifndef _SEVEN_SEGMENT
#define _SEVEN_SEGMENT

#include "utils/utils.h"

#define CLK_DISPLAY_PORT_CLK_EN __HAL_RCC_GPIOB_CLK_ENABLE();

// digit locations in the 7-segment
#define DIG_1 1
#define DIG_2 2
#define DIG_3 3
#define DIG_4 4

//special inputs
#define DIGIT 0		//	xxx.x 	format
#define DASHES 1	//	----	dashes
#define LETTER 2	//	   X 	format
#define DEGREES 3	//	xxx.x˚ 	format
#define CELSIUS 4	//	xx.x˚C 	format
#define KEYS 5		//	xxxx 	format

//letter data inputs
#define A 15
#define B 16
#define C 10

void gpio_clk_display_init(void);
void digitize(float* ADC_val,int *digit);
void seven_seg_digit_display(int place, int digit, bool decimal);
void set_letters(char *letters,int length);
void seven_seg_reset(void);
void display(float data);
void display_2(float data, int is_num);

#endif
