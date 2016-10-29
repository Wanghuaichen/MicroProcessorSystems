#ifndef _SYSTEM_INIT
#define _SYSTEM_INIT

#include <stm32f4xx_hal.h>
#include <sysclk_config.h>
#include "piezo_adc.h"
#include "LIS3DSH.h"
#include "timer.h"
#include "keypad.h"
#include "accelerometer.h"
#include "gpio_clk_display.h"

void system_init(void);

#endif
