#ifndef __CC2500_H
#define __CC2500_H

#include "stm32f4xx.h"

void CC2500_Init(void);
uint8_t CC2500_ReceivePacket(uint8_t *rxBuffer, uint8_t *size);

#endif
