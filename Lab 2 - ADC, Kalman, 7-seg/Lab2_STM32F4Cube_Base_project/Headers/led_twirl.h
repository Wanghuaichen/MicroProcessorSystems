////////////////////////////////////////////////////////////////////////////////
//	File Name					: gpio_example.h
//	Description				: header file for gpio_example.c
//	Author						: Harsh Aurora
//	Date							: Oct 1st, 2016
////////////////////////////////////////////////////////////////////////////////

#ifndef _LED_TWIRL
#define _LED_TWIRL

#define PORT GPIOD
#define LED_N GPIO_PIN_13 //LD3
#define LED_S GPIO_PIN_15 //LD6
#define LED_E GPIO_PIN_14 //LD5
#define LED_W GPIO_PIN_12 //LD4
#define CLK_EN __HAL_RCC_GPIOD_CLK_ENABLE();

void led_twirl_init(void);
void led_twirl(int twirl);
void clear_leds(void);

#endif
