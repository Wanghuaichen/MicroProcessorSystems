////////////////////////////////////////////////////////////////////////////////
//	File Name					: gpio_example.h
//	Description				: header file for gpio_example.c
//	Author						: Harsh Aurora
//	Date							: Oct 1st, 2016
////////////////////////////////////////////////////////////////////////////////

#ifndef _GPIO
#define _GPIO

#define PORT GPIOD
#define PIN GPIO_PIN_13 // Pin D13 is connected to the onboard orange LED
#define PORT_CLK_EN __HAL_RCC_GPIOD_CLK_ENABLE();

void gpio_init(void);
void gpio_toggle(void);

#endif
