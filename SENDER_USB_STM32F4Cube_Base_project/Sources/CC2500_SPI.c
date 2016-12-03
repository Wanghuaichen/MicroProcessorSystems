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

__IO uint32_t  CC2500Timeout = CC2500_FLAG_TIMEOUT;
SPI_HandleTypeDef    CC2500_SpiHandle;

//brief  SPI initialization
//param  None.
//retval None.
void CC2500_SPI_Init(void){

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
  //GPIO_InitStructure.Pin   = CC2500_SPI_INT1_PIN;
  //GPIO_InitStructure.Mode  = GPIO_MODE_IT_FALLING;
  //GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_MEDIUM;
	//GPIO_InitStructure.Pull  = GPIO_NOPULL;
  //HAL_GPIO_Init(CC2500_SPI_INT1_GPIO_PORT, &GPIO_InitStructure);
 
  //GPIO_InitStructure.Pin = CC2500_SPI_INT2_GPIO_Pin;
  //HAL_GPIO_Init(CC2500_SPI_INT2_GPIO_PORT, &GPIO_InitStructure);
}

//brief  Basic management of the timeout situation.
//param  None.
//retval None.
uint32_t CC2500_TIMEOUT_UserCallback(void)
{
  /* Block communication and all processes */
  // while (1)
  // {
  // }
	return 0;
}

//brief  Sends a Byte through the SPI interface and return the Byte received
//       from the SPI bus.
//param  Byte : Byte send.
//retval The received byte value
uint8_t CC2500_SPI_SendByte(uint8_t byte){
	//Loop while DR register in not empty
  CC2500Timeout = CC2500_FLAG_TIMEOUT;
	while(__HAL_SPI_GET_FLAG(&CC2500_SpiHandle, SPI_FLAG_TXE)==RESET){
		if((CC2500Timeout--) == 0) return CC2500_TIMEOUT_UserCallback();
	}
	
	//send a Byte through the SPI peripheral
	CC2500_SPI_SendData(&CC2500_SpiHandle, byte);
	
	//wait to receive a Byte
	CC2500Timeout = CC2500_FLAG_TIMEOUT;
  while (__HAL_SPI_GET_FLAG(&CC2500_SpiHandle, SPI_FLAG_RXNE) == RESET){
    if((CC2500Timeout--) == 0) return CC2500_TIMEOUT_UserCallback();
  }

  //Return the Byte read from the SPI bus 
  return CC2500_SPI_ReceiveData(&CC2500_SpiHandle);
}

//brief  reset Csn
//param  None.
//retval None.
void CC2500_SPI_CSn_Select(void){
	//Pull down CSn
	HAL_GPIO_WritePin(CC2500_SPI_CS_GPIO_PORT, CC2500_SPI_CS_PIN, GPIO_PIN_RESET);
}

//brief  set Csn
//param  None.
//retval None.
void CC2500_SPI_CSn_Deselect(void){
	// Pull up CSn
	HAL_GPIO_WritePin(CC2500_SPI_CS_GPIO_PORT, CC2500_SPI_CS_PIN, GPIO_PIN_SET);
}

//brief  read register in single read mod
//param  address.
//retval byte.
uint8_t CC2500_SPI_ReadReg(uint8_t addr) {
	CC2500_SPI_CSn_Select();
	
	//notify CC2500 with the destination reg. address
	CC2500_SPI_SendByte(addr | READ_SINGLE);
	
	//read the byte stored in the reg.
	uint8_t byte = CC2500_SPI_SendByte(Dummy_Byte);
	
	CC2500_SPI_CSn_Deselect();
	
	return byte;
}

//brief  read status registers
//param  address.
//retval byte.
uint8_t CC2500_SPI_ReadStatusReg(uint8_t addr) {
	CC2500_SPI_CSn_Select();
	
	//notify CC2500 with the destination reg. address
	CC2500_SPI_SendByte(addr | READ_BURST);
	
	//read the byte stored in the reg.
	uint8_t byte = CC2500_SPI_SendByte(Dummy_Byte);
	
	CC2500_SPI_CSn_Deselect();
	
	return byte;
}

//brief  read register in burst read mod
//param  address, *buffer, count.
//retval none.
void CC2500_SPI_ReadRegBurst(uint8_t addr, uint8_t *buffer, uint8_t count) {
	CC2500_SPI_CSn_Select();
	
	//notify CC2500 with the starting address of burst reading registers
	CC2500_SPI_SendByte(addr | READ_BURST);
	
	for(uint8_t i=0; i<count; i++){
		buffer[i] = CC2500_SPI_SendByte(Dummy_Byte);
	}
	
	CC2500_SPI_CSn_Deselect();
}

//brief  write register in single write mod
//param  address, byte.
//retval none.
void CC2500_SPI_WriteReg(uint8_t addr, uint8_t byte) {
	CC2500_SPI_CSn_Select();
	
	//notify CC2500 with the destination reg. address
	CC2500_SPI_SendByte(addr | WRITE_SINGLE);
	
	//write the byte to the reg.
	CC2500_SPI_SendByte(byte);
	
	CC2500_SPI_CSn_Deselect();
}

//brief  write register in burst write mod
//param  address, *buffer, count.
//retval none.
void CC2500_SPI_WriteRegBurst(uint8_t addr, uint8_t *buffer, uint8_t count) {
	CC2500_SPI_CSn_Select();
	
	//notify CC2500 with the starting address of burst writing registers
	CC2500_SPI_SendByte(addr | WRITE_BURST);
	
	for(uint8_t i=0; i<count; i++){
		CC2500_SPI_SendByte(buffer[i]);
	}
	
	CC2500_SPI_CSn_Deselect();
}

//brief  srobe command 
//param  address.
//retval status.
uint8_t CC2500_SPI_Strobe(uint8_t addr){
	CC2500_SPI_CSn_Select();
	CC2500_SPI_SendByte(addr);
	uint8_t status = CC2500_SPI_SendByte(Dummy_Byte);
	CC2500_SPI_CSn_Deselect();
	return status;
}

//brief  Transmits a Data through the SPIx/I2Sx peripheral.
//param  *hspi: Pointer to the SPI handle. Its member Instance can point to either SPI1, SPI2 or SPI3 
//param  Data: Data to be transmitted.
//retval None
void CC2500_SPI_SendData(SPI_HandleTypeDef *hspi, uint16_t Data){ 
  /* Write in the DR register the data to be sent */
  hspi->Instance->DR = Data;
}

//brief  Returns the most recent received data by the SPIx/I2Sx peripheral. 
//param  *hspi: Pointer to the SPI handle. Its member Instance can point to either SPI1, SPI2 or SPI3 
//retval The value of the received data.
uint8_t CC2500_SPI_ReceiveData(SPI_HandleTypeDef *hspi){
  /* Return the data in the DR register */
  return hspi->Instance->DR;
}