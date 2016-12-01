#ifndef __CC2500_H
#define __CC2500_H

#include "stm32f4xx.h"

extern int exti_flag;

void CC2500_Init(void);
void CC2500_Rx_Interrupt_Config(void);
uint8_t CC2500_ReceivePacket(uint8_t *rxBuffer, uint8_t *size);
void CC2500_Chipset_config(void);
void CC2500_rx_config(void);
uint8_t CC2500_SendPacket(uint8_t *txBuffer, uint8_t *size);
void CC2500_tx_config(void);

#endif
