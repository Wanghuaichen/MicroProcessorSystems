//include
#include "cc2500.h"
#include "cc2500_spi.h"

//brief  CC2500 wireless driver initialization
//param  None.
//retval None.
void CC2500_Init(void){
	//initial spi configuration
	CC2500_SPI_Init();
}

void CC2500_Chipset_config(void){
	CC2500_SPI_WriteReg(IOCFG2 	, VAL_CC2500_IOCFG2 	);
	CC2500_SPI_WriteReg(IOCFG0 	, VAL_CC2500_IOCFG0 	);
	CC2500_SPI_WriteReg(FIFOTHR , VAL_CC2500_FIFOTHR 	);
	CC2500_SPI_WriteReg(PKTLEN 	, VAL_CC2500_PKTLEN 	);
	CC2500_SPI_WriteReg(PKTCTRL1 , VAL_CC2500_PKTCTRL1);
	CC2500_SPI_WriteReg(PKTCTRL0 , VAL_CC2500_PKTCTRL0);
	CC2500_SPI_WriteReg(ADDR 	, VAL_CC2500_ADDR 		);
	CC2500_SPI_WriteReg(CHANNR 	, VAL_CC2500_CHANNR 	);
	CC2500_SPI_WriteReg(FSCTRL1 , VAL_CC2500_FSCTRL1 	);
	CC2500_SPI_WriteReg(FSCTRL0 , VAL_CC2500_FSCTRL0 	);
	CC2500_SPI_WriteReg(FREQ2 	, VAL_GROUP2_CC2500_FREQ2 	);
	CC2500_SPI_WriteReg(FREQ1 	, VAL_GROUP2_CC2500_FREQ1 	);
	CC2500_SPI_WriteReg(FREQ0 	, VAL_GROUP2_CC2500_FREQ0 	);
	CC2500_SPI_WriteReg(MDMCFG4 , VAL_CC2500_MDMCFG4 	);
	CC2500_SPI_WriteReg(MDMCFG3 , VAL_CC2500_MDMCFG3 	);
	CC2500_SPI_WriteReg(MDMCFG2 , VAL_CC2500_MDMCFG2 	);
	CC2500_SPI_WriteReg(MDMCFG1 , VAL_CC2500_MDMCFG1 	);
	CC2500_SPI_WriteReg(MDMCFG0 , VAL_CC2500_MDMCFG0 	);
	CC2500_SPI_WriteReg(DEVIATN , VAL_CC2500_DEVIATN 	);
	CC2500_SPI_WriteReg(MCSM1 	, VAL_CC2500_MCSM1 	);
	CC2500_SPI_WriteReg(MCSM0 	, VAL_CC2500_MCSM0 	);
	CC2500_SPI_WriteReg(FOCCFG 	, VAL_CC2500_FOCCFG 	);
	CC2500_SPI_WriteReg(BSCFG 	, VAL_CC2500_BSCFG 	);
	CC2500_SPI_WriteReg(AGCTRL2 , VAL_CC2500_AGCTRL2 	);
	CC2500_SPI_WriteReg(AGCTRL1 , VAL_CC2500_AGCTRL1 	);
	CC2500_SPI_WriteReg(AGCTRL0 , VAL_CC2500_AGCTRL0 	);
	CC2500_SPI_WriteReg(FREND1 	, VAL_CC2500_FREND1 	);
	CC2500_SPI_WriteReg(FREND0 	, VAL_CC2500_FREND0 	);
	CC2500_SPI_WriteReg(FSCAL3 	, VAL_CC2500_FSCAL3 	);
	CC2500_SPI_WriteReg(FSCAL2 	, VAL_CC2500_FSCAL2 	);
	CC2500_SPI_WriteReg(FSCAL1 	, VAL_CC2500_FSCAL1 	);
	CC2500_SPI_WriteReg(FSCAL0 	, VAL_CC2500_FSCAL0 	);
	CC2500_SPI_WriteReg(FSTEST 	, VAL_CC2500_FSTEST 	);
	CC2500_SPI_WriteReg(TEST2 	, VAL_CC2500_TEST2 	);
	CC2500_SPI_WriteReg(TEST1 	, VAL_CC2500_TEST1 	);
	CC2500_SPI_WriteReg(TEST0 	, VAL_CC2500_TEST0 	);
}

void CC2500_Rx_Interrupt_Config(void){
	
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
	HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
}

//brief  receive packet
//param  uint8_t *rxBuffer, uint8_t *size.
//retval status.
uint8_t CC2500_ReceivePacket(uint8_t *rxBuffer, uint8_t *size){
	uint8_t packet_length;
	uint8_t status[2];
	
	//read length of the packet
	packet_length=CC2500_SPI_ReadStatusReg(RXBYTES)&RXBYTES_MASK;
	//printf("first read - packet length is %d\n", packet_length);
	
	//wait until the rx length changes
	//while(packet_length == Reset_Flag) {
		//packet_length=1;
		//printf("packet length is: %d\n", packet_length);
	//}
	
	//uint8_t bit_ready=CC2500_SPI_ReadReg(0x3B)&RXBYTES_MASK;
	//printf("bit ready is: %d\n", bit_ready);
	
	if(packet_length != Reset_Flag) {
		if (packet_length <= *size){
			
			// If legnth byte is attached at the head of the payload
			if((CC2500_SPI_ReadReg(PKTCTRL0)&PKTCTRL0_LENGTH_CONFIG_MASK)==PKTCTRL0_LENGTH_CONFIG_VariableLength){
				packet_length=CC2500_SPI_ReadReg(RXFIFO);
			}
		
		  //read
		  CC2500_SPI_ReadRegBurst(RXFIFO, rxBuffer, packet_length);
	    
		  //update packet size
		  *size = packet_length;
		  
		  //read the appended status byte
		  if((CC2500_SPI_ReadReg(PKTCTRL1)&PKTCTRL1_APPEND_STATUS_MASK)!=Reset_Flag){
		  	CC2500_SPI_ReadRegBurst(RXFIFO, status, 2);
		  	return status[RX_LQI_Index]&RX_LQI_CRC_OK_Mask;
		  }
		  
		  return Status_OK;
	  } else {
		  //request size larger than available packet length
		  *size = packet_length;
		  //flush the rx buffer
		  CC2500_SPI_Strobe(SFRX);
		  
		  return Status_Error;
	    }
		}
	
		//if nothing happened
		return Status_Error;
}

//brief  send packet
//param  uint8_t *txBuffer, uint8_t *size.
//retval status.
uint8_t CC2500_SendPacket(uint8_t *txBuffer, uint8_t *size){
	uint8_t status = CC2500_SPI_Strobe(SIDLE); //get status
	uint8_t tx_bytes_available = status&0x0F; //read num bytes in tx buffer
	if(tx_bytes_available < *size) {
		return Status_Error;
	}
	CC2500_SPI_WriteRegBurst(TXFIFO, txBuffer, *size);
	status = CC2500_SPI_Strobe(STX); //get status, set to TX
	uint8_t tx_bytes_available_new = status&0x0F; //read num bytes in tx buffer
	while(tx_bytes_available_new < tx_bytes_available) {
		tx_bytes_available_new = CC2500_SPI_Strobe(STX)&0x0F;
	}
	return Status_OK;
}

void CC2500_tx_config(void) {
	CC2500_SPI_WriteReg(MCSM1,49); //write to radio control reg - txmode = 01: move to FSTXON after TX
}

void CC2500_rx_config(void) {
	CC2500_SPI_WriteReg(MCSM1,60); //write to radio control reg - stay in RX mode after 1st packet
	CC2500_SPI_Strobe(SRX); //set to rx mode
}

void message_format(uint8_t *msg, uint8_t x, uint8_t y, uint8_t scroll, uint8_t br, uint8_t bl) {
	msg[0] = scroll;
	msg[1] = y;
	msg[2] = x;
	msg[3] = bl<<2 | br;
}

//void message_decode(uint8_t *msg, uint8_t *x, uint8_t *y, uint8_t *scroll, uint8_t *br, uint8_t *bl) {
//	scroll = msg[0]
//	y = msg[1];
//	x = msg[2];
//	bl = bl>>2;
//	br = msg[3]&0x01;
//}
