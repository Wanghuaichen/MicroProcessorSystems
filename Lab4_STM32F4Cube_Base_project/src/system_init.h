#ifndef _SYSTEM_INIT
#define _SYSTEM_INIT

#include <stm32f4xx_hal.h>
#include <sysclk_config.h>
#include "interfaces/LIS3DSH.h"
#include "interfaces/timer.h"
#include "interfaces/keypad.h"
#include "interfaces/accelerometer.h"
#include "interfaces/seven_segment.h"

void system_init(void);

#endif
