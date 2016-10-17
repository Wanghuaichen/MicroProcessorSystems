////////////////////////////////////////////////////////////////////////////////
//	File Name					: gpio_example.h
//	Description				: header file for gpio_example.c
//	Author						: Harsh Aurora
//	Date							: Oct 1st, 2016
////////////////////////////////////////////////////////////////////////////////

#ifndef _GPIO_EXAMPLE
#define _GPIO_EXAMPLE

#define EXAMPLE_PORT GPIOD
#define EXAMPLE_PIN GPIO_PIN_13 // Pin D13 is connected to the onboard orange LED
#define EXAMPLE_PORT_CLK_EN __HAL_RCC_GPIOD_CLK_ENABLE();

void gpio_example_init(void);
void gpio_example(void);

#endif
