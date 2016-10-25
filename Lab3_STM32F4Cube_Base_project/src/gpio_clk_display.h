#ifndef _GPIO_CLK_DISPLAY
#define _GPIO_CLK_DISPLAY

#include "bool.h"

#define CLK_DISPLAY_PORT_CLK_EN __HAL_RCC_GPIOB_CLK_ENABLE();

#define DIG_1 1
#define DIG_2 2
#define DIG_3 3
#define DIG_4 4

void gpio_clk_display_init(void);
void digitize(float* ADC_val,int *digit);
void seven_seg_digit_display(int place, int digit, bool decimal);
void set_letters(char *letters,int length);
void seven_seg_reset(void);
void display(float data);

#endif
