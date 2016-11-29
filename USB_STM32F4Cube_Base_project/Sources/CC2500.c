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

//brief  receive packet
//param  uint8_t *rxBuffer, uint8_t *size.
//retval status.
uint8_t CC2500_ReceivePacket(uint8_t *rxBuffer, uint8_t *size){
	uint8_t packet_length;
	uint8_t status[2];
	
	//read length of the packet
	packet_length=CC2500_SPI_ReadStatusReg(RXBYTES)&RXBYTES_MASK;
	printf("first read - packet length is %d\n", packet_length);
	
	//wait until the rx length changes
	while(packet_length == Reset_Flag) {
		packet_length=1;
		printf("packet length is: %d\n", packet_length);
	}
	
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