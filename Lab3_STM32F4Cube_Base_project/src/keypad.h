#ifndef _KEYPAD
#define _KEYPAD

#define CLK_KEYPAD_PORT_CLK_EN __HAL_RCC_GPIOD_CLK_ENABLE();

#define C1 (GPIO_PIN_0);
#define C2 (GPIO_PIN_1);
#define C3 (GPIO_PIN_2);
          
#define R1 (GPIO_PIN_3);
#define R2 (GPIO_PIN_7);
#define R3 (GPIO_PIN_8);
#define R4 (GPIO_PIN_9);
                      
void keypad_init(void);
void keypad_scan(void);
int get_digit(void);

#endif
