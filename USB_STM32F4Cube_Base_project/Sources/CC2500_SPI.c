//include
#include "cc2500_spi.h"

//initialization
//static volatile SPI_TypeDef*   _CC2500_SPI;
//static volatile GPIO_TypeDef*  _CC2500_SPI_CSn_GPIO_Port;
//static volatile uint32_t       _CC2500_CSn_GPIO_Pin;
//static volatile GPIO_TypeDef*  _CC2500_MISO_GPIO_Port;
//static volatile uint32_t       _CC2500_MISO_GPIO_Pin;
//static volatile GPIO_TypeDef*  _CC2500_Trans_GPIO_Port;
//static volatile uint32_t       _CC2500_Trans_GPIO_Pin;

SPI_HandleTypeDef    CC2500_SpiHandle;

void CC2500_SPI_Init(CC2500_InitTypeDef *CC2500_SPI_InitStruct){
	
	uint8_t ctrl = 0x00;

  /* Configure the low level interface ---------------------------------------*/
	  /* SPI configuration -------------------------------------------------------*/
	__HAL_RCC_SPI2_CLK_ENABLE();
	
  HAL_SPI_DeInit(&CC2500_SpiHandle);
  CC2500_SpiHandle.Instance 							  = SPI2;
  CC2500_SpiHandle.Init.BaudRatePrescaler 	= SPI_BAUDRATEPRESCALER_16; //clock running at 5.625 Mhz
  CC2500_SpiHandle.Init.Direction 					= SPI_DIRECTION_2LINES;
  CC2500_SpiHandle.Init.CLKPhase 					= SPI_PHASE_1EDGE;
  CC2500_SpiHandle.Init.CLKPolarity 				= SPI_POLARITY_LOW;
  CC2500_SpiHandle.Init.CRCCalculation			= SPI_CRCCALCULATION_DISABLED;
  CC2500_SpiHandle.Init.CRCPolynomial 			= 7;
  CC2500_SpiHandle.Init.DataSize 					= SPI_DATASIZE_8BIT;
  CC2500_SpiHandle.Init.FirstBit 					= SPI_FIRSTBIT_MSB;
  CC2500_SpiHandle.Init.NSS 								= SPI_NSS_SOFT;
  CC2500_SpiHandle.Init.TIMode 						= SPI_TIMODE_DISABLED;
  CC2500_SpiHandle.Init.Mode 							= SPI_MODE_MASTER;
	if (HAL_SPI_Init(&CC2500_SpiHandle) != HAL_OK) {printf ("ERROR: Error in initialising SPI2 \n");};
  
	__HAL_SPI_ENABLE(&CC2500_SpiHandle);
	
	/*//enable SPI periph clock
	if (CC2500_SPI_InitStruct->CC2500_SPI_Periph==CC2500_SPI_Periph_APB1){
		__HAL_RCC_SPI2_CLK_ENABLE(); //low speed
	} else {
		__HAL_RCC_SPI1_CLK_ENABLE(); //high speed
	}
	
	//enable GPIO periph clock
	__HAL_RCC_GPIOA_CLK_ENABLE();
	
	//initialization of GPIO pins
	GPIO_InitTypeDef GPIO_INIT;
	
	//GPIO
	GPIO_INIT.Mode=GPIO_MODE_AF_PP;//alternative push-pull mode
	GPIO_INIT.Pull=GPIO_NOPULL;
	GPIO_INIT.Speed=GPIO_SPEED_FREQ_MEDIUM;
	
	//SCK
	GPIO_INIT.Pin = CC2500_SPI_InitStruct->CC2500_SPI_SCK_GPIO_Pin;
	HAL_GPIO_Init(CC2500_SPI_InitStruct->CC2500_SPI_SCK_GPIO_Port, &GPIO_INIT);
	
	// MOSI
	GPIO_INIT.Pin = CC2500_SPI_InitStruct->CC2500_SPI_MOSI_GPIO_Pin;
	HAL_GPIO_Init(CC2500_SPI_InitStruct->CC2500_SPI_MOSI_GPIO_Port, &GPIO_INIT);
	
	// MISO
	GPIO_INIT.Pin = CC2500_SPI_InitStruct->CC2500_SPI_MISO_GPIO_Pin;
	HAL_GPIO_Init(CC2500_SPI_InitStruct->CC2500_SPI_MISO_GPIO_Port, &GPIO_INIT);
	
	//HAL_GPIO_PinAFConfig();*/
}
