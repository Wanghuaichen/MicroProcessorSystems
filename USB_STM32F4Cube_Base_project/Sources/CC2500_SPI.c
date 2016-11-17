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

void CC2500_SPI_Init(){

  /* Configure the low level interface ---------------------------------------*/
	/* SPI configuration -------------------------------------------------------*/
	__HAL_RCC_SPI2_CLK_ENABLE();
	
  	HAL_SPI_DeInit(&CC2500_SpiHandle);
  	CC2500_SpiHandle.Instance 							  = SPI2;
  	CC2500_SpiHandle.Init.BaudRatePrescaler 	= SPI_BAUDRATEPRESCALER_16; //clock running at 5.625 Mhz
  	CC2500_SpiHandle.Init.Direction 					= SPI_DIRECTION_2LINES;
  	CC2500_SpiHandle.Init.CLKPhase 						= SPI_PHASE_1EDGE;
  	CC2500_SpiHandle.Init.CLKPolarity 				= SPI_POLARITY_LOW;
  	CC2500_SpiHandle.Init.CRCCalculation			= SPI_CRCCALCULATION_DISABLED;
  	CC2500_SpiHandle.Init.CRCPolynomial 			= 7;
  	CC2500_SpiHandle.Init.DataSize 						= SPI_DATASIZE_8BIT;
  	CC2500_SpiHandle.Init.FirstBit 						= SPI_FIRSTBIT_MSB;
  	CC2500_SpiHandle.Init.NSS 								= SPI_NSS_SOFT;
  	CC2500_SpiHandle.Init.TIMode 							= SPI_TIMODE_DISABLED;
  	CC2500_SpiHandle.Init.Mode 								= SPI_MODE_MASTER;

	if (HAL_SPI_Init(&CC2500_SpiHandle) != HAL_OK) {printf ("ERROR: Error in initialising SPI2 \n");};
  
	__HAL_SPI_ENABLE(&CC2500_SpiHandle);
	
	/* Enable SCK and CS GPIO clocks */
  __GPIOB_CLK_ENABLE();

	/* Enable MOSI and MISO GPIO clocks */
  __GPIOC_CLK_ENABLE();
	
	__GPIOE_CLK_ENABLE();

  //GPIO_PinAFConfig(LIS3DSH_SPI_SCK_GPIO_PORT, LIS3DSH_SPI_SCK_SOURCE, LIS3DSH_SPI_SCK_AF);
  //GPIO_PinAFConfig(LIS3DSH_SPI_MISO_GPIO_PORT, LIS3DSH_SPI_MISO_SOURCE, LIS3DSH_SPI_MISO_AF);
  //GPIO_PinAFConfig(LIS3DSH_SPI_MOSI_GPIO_PORT, LIS3DSH_SPI_MOSI_SOURCE, LIS3DSH_SPI_MOSI_AF);
	
	GPIO_InitTypeDef GPIO_InitStructure;

  GPIO_InitStructure.Mode  = GPIO_MODE_AF_PP;
  GPIO_InitStructure.Pull  = GPIO_NOPULL;
  GPIO_InitStructure.Speed = GPIO_SPEED_MEDIUM;
  GPIO_InitStructure.Alternate = GPIO_AF5_SPI2;

  /* SPI SCK pin configuration */
  GPIO_InitStructure.Pin = CC2500_SPI_SCK_PIN;
  HAL_GPIO_Init(CC2500_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);

  /* SPI  MOSI pin configuration */
  GPIO_InitStructure.Pin =  CC2500_SPI_MOSI_PIN;
  HAL_GPIO_Init(CC2500_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);

  /* SPI MISO pin configuration */
  GPIO_InitStructure.Pin = CC2500_SPI_MISO_PIN;
  HAL_GPIO_Init(CC2500_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.Pin   = CC2500_SPI_CS_PIN;
  GPIO_InitStructure.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(CC2500_SPI_CS_GPIO_PORT, &GPIO_InitStructure);

  /* Deselect : Chip Select high */
  HAL_GPIO_WritePin(CC2500_SPI_CS_GPIO_PORT, CC2500_SPI_CS_PIN, GPIO_PIN_SET);

  /* Configure GPIO PINs to detect Interrupts */
  GPIO_InitStructure.Pin   = CC2500_SPI_INT1_PIN;
  GPIO_InitStructure.Mode  = GPIO_MODE_IT_FALLING;
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(CC2500_SPI_INT1_GPIO_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.Pin = CC2500_SPI_INT2_PIN;
  HAL_GPIO_Init(CC2500_SPI_INT2_GPIO_PORT, &GPIO_InitStructure);
}

uint8_t readPN() {
	//HAL_SPI_Receive(hspi, data, uint16_t Size, uint32_t Timeout);
	//HAL_SPI_Transmit(hspi, pData, uint16_t Size, uint32_t Timeout);
	uint8_t rxData = 0;
	uint8_t txData = READ_PARTNUM_CMD;
	uint16_t size = 2;
	uint32_t timeout = 4;
	
	if(HAL_SPI_TransmitReceive(&CC2500_SpiHandle, &txData, &rxData, size, timeout) == HAL_OK); //SPI_HandleTypeDef *hspi, uint8_t *pTxData, uint8_t *pRxData, uint16_t Size, uint32_t Timeout
	return rxData;
}