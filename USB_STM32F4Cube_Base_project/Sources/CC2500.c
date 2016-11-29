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


void CC2500_ReceivePacket(uint8_t *rxBuffer, uint8_t *size){
	uint8_t length;
	uint8_t status[2];
	
	//read length of the packet
	length=CC2500_SPI_ReadStatusReg(0x3B)&0x7F;
	//wait until the same value is returned twice for rx length
	uint8_t length_prev = 0;
	while(length != length_prev) {
		length_prev = length;
		length = CC2500_SPI_ReadStatusReg(0x3B)&0x7F;
	}
	//just read
	printf("%d\n", CC2500_SPI_ReadReg(0x06));
	printf("%d\n", CC2500_SPI_ReadReg(0x08));
	CC2500_SPI_ReadRegBurst(0x3F, rxBuffer, length);
  //printf(" %d\n %d\n %d\n %d\n %d\n %d\n %d\n %d\n %d\n %d\n",rxBuffer[0],rxBuffer[1],rxBuffer[2],rxBuffer[3],rxBuffer[4],rxBuffer[5],rxBuffer[6],rxBuffer[7],rxBuffer[8],rxBuffer[9]);
}