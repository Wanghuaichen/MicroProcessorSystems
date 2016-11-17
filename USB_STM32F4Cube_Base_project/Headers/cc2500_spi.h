#ifndef __CC2500_SPI_H
#define __CC2500_SPI_H

#include "stm32f4xx.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_spi.h"
#include "cc2500_settings.h"

//initialization struct
typedef struct{
	//SPI
	SPI_TypeDef*        CC2500_SPI;
	uint8_t						  CC2500_SPI_Periph;
	uint32_t					  CC2500_SPI_CLK;
	
	//SCK
	GPIO_TypeDef*				CC2500_SPI_SCK_GPIO_Port;
	uint32_t					  CC2500_SPI_SCK_GPIO_Pin;
	
	//MOSI
	GPIO_TypeDef*				CC2500_SPI_MOSI_GPIO_Port;
	uint32_t					  CC2500_SPI_MOSI_GPIO_Pin;
	
	//MISO
	GPIO_TypeDef*				CC2500_SPI_MISO_GPIO_Port;
	uint32_t					  CC2500_SPI_MISO_GPIO_Pin;
	
	//CSn
	GPIO_TypeDef*       CC2500_SPI_CSn_GPIO_Port;
	uint32_t			      CC2500_SPI_CSn_GPIO_Pin;
	
} CC2500_InitTypeDef;

//transmit mode struct
typedef struct{
	GPIO_TypeDef*			  CC2500_Trans_GPIO_Port;
	uint32_t					  CC2500_Trans_GPIO_Pin;
	uint32_t					  CC2500_Trans_GPIO_CLK;
} CC2500_TransTypeDef;

#endif
