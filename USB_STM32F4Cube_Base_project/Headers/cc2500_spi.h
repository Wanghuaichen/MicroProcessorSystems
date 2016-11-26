#ifndef __CC2500_SPI_H
#define __CC2500_SPI_H

#include "stm32f4xx.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_spi.h"
#include "cc2500_settings.h"

/* Maximum Timeout values for flags waiting loops. These timeouts are not based
   on accurate values, they just guarantee that the application will not remain
   stuck if the SPI communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */

#define CC2500_FLAG_TIMEOUT         ((uint32_t)0x1000)

//COMMANDS
//R/W
//#define READ 0x00//0000 0000
//#define WRITE 0x80//1000 0000 bit 7
#define WRITE_SINGLE	  0x00
#define WRITE_BURST     0x40
#define READ_SINGLE			0x80
#define READ_BURST	    0xC0
//access mode
#define BURST_MODE 0x40 //0100 0000 
#define SINGLE_ACCESS_MODE 0x00 //0000 0000 bit 6
//Utility
#define Dummy_Byte			0x00

//--------------------pin config------------------------------
#define CC2500_SPI                       SPI2
#define CC2500_SPI_CLK                   RCC_APB1Periph_SPI2
        
#define CC2500_SPI_SCK_PIN               GPIO_PIN_10                 /* PB.10 */
#define CC2500_SPI_SCK_GPIO_PORT         GPIOB                       /* GPIOB */
#define CC2500_SPI_SCK_GPIO_CLK          RCC_AHB1Periph_GPIOB
#define CC2500_SPI_SCK_SOURCE            GPIO_PinSource5
#define CC2500_SPI_SCK_AF                GPIO_AF5_SPI2
        
#define CC2500_SPI_MISO_PIN              GPIO_PIN_2                  /* PC.02 */
#define CC2500_SPI_MISO_GPIO_PORT        GPIOC                       /* GPIOC */
#define CC2500_SPI_MISO_GPIO_CLK         RCC_AHB1Periph_GPIOC
#define CC2500_SPI_MISO_SOURCE           GPIO_PinSource6
#define CC2500_SPI_MISO_AF               GPIO_AF5_SPI2
        
#define CC2500_SPI_MOSI_PIN              GPIO_PIN_3                  /* PC.03 */
#define CC2500_SPI_MOSI_GPIO_PORT        GPIOC                       /* GPIOC */
#define CC2500_SPI_MOSI_GPIO_CLK         RCC_AHB1Periph_GPIOC
#define CC2500_SPI_MOSI_SOURCE           GPIO_PinSource7
#define CC2500_SPI_MOSI_AF               GPIO_AF5_SPI2
        
#define CC2500_SPI_CS_PIN                GPIO_PIN_12                 /* PB.12 */
#define CC2500_SPI_CS_GPIO_PORT          GPIOB                       /* GPIOB */
#define CC2500_SPI_CS_GPIO_CLK           RCC_AHB1Periph_GPIOB
        
#define CC2500_SPI_INT1_PIN              GPIO_PIN_0                  /* PE.00 */
#define CC2500_SPI_INT1_GPIO_PORT        GPIOE                       /* GPIOE */
#define CC2500_SPI_INT1_GPIO_CLK         RCC_AHB1Periph_GPIOE
#define CC2500_SPI_INT1_EXTI_LINE        EXTI_Line0
#define CC2500_SPI_INT1_EXTI_PORT_SOURCE EXTI_PortSourceGPIOE
#define CC2500_SPI_INT1_EXTI_PIN_SOURCE  EXTI_PinSource0
#define CC2500_SPI_INT1_EXTI_IRQn        EXTI0_IRQn
        
#define CC2500_SPI_INT2_PIN              GPIO_PIN_1                  /* PE.01 */
#define CC2500_SPI_INT2_GPIO_PORT        GPIOE                       /* GPIOE */
#define CC2500_SPI_INT2_GPIO_CLK         RCC_AHB1Periph_GPIOE
#define CC2500_SPI_INT2_EXTI_LINE        EXTI_Line1
#define CC2500_SPI_INT2_EXTI_PORT_SOURCE EXTI_PortSourceGPIOE
#define CC2500_SPI_INT2_EXTI_PIN_SOURCE  EXTI_PinSource1
#define CC2500_SPI_INT2_EXTI_IRQn        EXTI1_IRQn

//addr - bit 5 to 0
//-------------------register map-----------------------------

//status registers
#define PARTNUM 				0x30	//CC2500 part number 81
#define VERSION 				0x31	//Current version number 81
#define FREQEST 				0x32	//Frequency offset estimate 81
#define LQI 						0x33	//Demodulator estimate for Link Quality 82
#define RSSI 						0x34	//Received signal strength indication 82
#define MARCSTATE 			0x35	//Control state machine state 82
#define WORTIME1 				0x36	//High byte of WOR timer 83
#define WORTIME0 				0x37	//Low byte of WOR timer 83
#define PKTSTATUS 			0x38	//Current GDOx status and packet status 83
#define VCO_VC_DAC 			0x39	//Current setting from PLL calibration module 83
#define TXBYTES 				0x3A	//Underflow and number of bytes in the TX FIFO 83
#define RXBYTES 				0x3B	//Overflow and number of bytes in the RX FIFO 84
#define RCCTRL1_STATUS 	0x3C	//Last RC oscillator calibration result 84
#define RCCTRL0_STATUS 	0x3D	//Last RC oscillator calibration result 84

//commands for status registers - must be read in burst mode
#define READ_PARTNUM_CMD					READ | BURST_MODE | PARTNUM 
#define READ_VERSION_CMD					READ | BURST_MODE | VERSION 
#define READ_FREQEST_CMD					READ | BURST_MODE | FREQEST 
#define READ_LQI_CMD							READ | BURST_MODE | LQI 	
#define READ_RSSI_CMD							READ | BURST_MODE | RSSI 	
#define READ_MARCSTATE_CMD				READ | BURST_MODE | MARCSTATE 
#define READ_WORTIME1_CMD					READ | BURST_MODE | WORTIME1 
#define READ_WORTIME0_CMD					READ | BURST_MODE | WORTIME0 
#define READ_PKTSTATUS_CMD				READ | BURST_MODE | PKTSTATUS 
#define READ_VCO_VC_DAC_CMD				READ | BURST_MODE | VCO_VC_DAC 
#define READ_TXBYTES_CMD					READ | BURST_MODE | TXBYTES 
#define READ_RXBYTES_CMD					READ | BURST_MODE | RXBYTES 
#define READ_RCCTRL1_STATUS_CMD		READ | BURST_MODE | RCCTRL1_STATUS
#define READ_RCCTRL0_STATUS_CMD		READ | BURST_MODE | RCCTRL0_STATUS

//configuration registers
#define IOCFG2 		0x00 	//GDO2 output pin configuration
#define IOCFG1 		0x01 	//GDO1 output pin configuration
#define IOCFG0 		0x02 	//GDO0 output pin configuration
#define FIFOTHR 	0x03 	//RX FIFO and TX FIFO thresholds
#define SYNC1 		0x04 	//Sync word, high byte
#define SYNC0 		0x05 	//Sync word, low byte
#define PKTLEN 		0x06 	//Packet length
#define PKTCTRL1 	0x07 	//Packet automation control
#define PKTCTRL0 	0x08 	//Packet automation control
#define ADDR 			0x09 	//Device address
#define CHANNR 		0x0A 	//Channel number
#define FSCTRL1 	0x0B 	//Frequency synthesizer control
#define FSCTRL0 	0x0C 	//Frequency synthesizer control
#define FREQ2 		0x0D 	//Frequency control word, high byte
#define FREQ1 		0x0E 	//Frequency control word, middle byte
#define FREQ0 		0x0F 	//Frequency control word, low byte
#define MDMCFG4 	0x10 	//Modem configuration
#define MDMCFG3 	0x11 	//Modem configuration
#define MDMCFG2 	0x12 	//Modem configuration
#define MDMCFG1 	0x13 	//Modem configuration
#define MDMCFG0 	0x14 	//Modem configuration
#define DEVIATN 	0x15 	//Modem deviation setting
#define MCSM2 		0x16 	//Main Radio Control State Machine configuration
#define MCSM1 		0x17 	//Main Radio Control State Machine configuration
#define MCSM0 		0x18 	//Main Radio Control State Machine configuration
#define FOCCFG 		0x19 	//Frequency Offset Compensation configuration
#define BSCFG 		0x1A 	//Bit Synchronization configuration
#define AGCTRL2 	0x1B 	//AGC control
#define AGCTRL1 	0x1C 	//AGC control
#define AGCTRL0 	0x1D 	//AGC control
#define WOREVT1 	0x1E 	//High byte Event 0 timeout
#define WOREVT0 	0x1F 	//Low byte Event 0 timeout
#define WORCTRL 	0x20 	//Wake On Radio control
#define FREND1 		0x21 	//Front end RX configuration
#define FREND0 		0x22 	//Front end TX configuration
#define FSCAL3 		0x23 	//Frequency synthesizer calibration
#define FSCAL2 		0x24 	//Frequency synthesizer calibration
#define FSCAL1 		0x25 	//Frequency synthesizer calibration
#define FSCAL0 		0x26 	//Frequency synthesizer calibration
#define RCCTRL1 	0x27 	//RC oscillator configuration
#define RCCTRL0 	0x28 	//RC oscillator configuration
#define FSTEST 		0x29 	//Frequency synthesizer calibration control
#define PTEST 		0x2A 	//Production test
#define AGCTEST 	0x2B 	//AGC test
#define TEST2 		0x2C 	//Various test settings
#define TEST1 		0x2D 	//Various test settings
#define TEST0 		0x2E 	//Various test settings

//---------------------------------------------------------------------------

//initialization struct
//typedef struct{
//	//SPI
//	SPI_TypeDef*        CC2500_SPI;
//	uint8_t						  CC2500_SPI_Periph;
//	uint32_t					  CC2500_SPI_CLK;
//	
//	//SCK
//	GPIO_TypeDef*				CC2500_SPI_SCK_GPIO_Port;
//	uint32_t					  CC2500_SPI_SCK_GPIO_Pin;
//	
//	//MOSI
//	GPIO_TypeDef*				CC2500_SPI_MOSI_GPIO_Port;
//	uint32_t					  CC2500_SPI_MOSI_GPIO_Pin;
//	
//	//MISO
//	GPIO_TypeDef*				CC2500_SPI_MISO_GPIO_Port;
//	uint32_t					  CC2500_SPI_MISO_GPIO_Pin;
//	
//	//CSn
//	GPIO_TypeDef*       CC2500_SPI_CSn_GPIO_Port;
//	uint32_t			      CC2500_SPI_CSn_GPIO_Pin;
//	
//} CC2500_InitTypeDef;

//transmit mode struct
//typedef struct{
//	GPIO_TypeDef*			  CC2500_Trans_GPIO_Port;
//	uint32_t					  CC2500_Trans_GPIO_Pin;
//	uint32_t					  CC2500_Trans_GPIO_CLK;
//} CC2500_TransTypeDef;

void CC2500_SPI_Init();
uint8_t CC2500_SPI_ReadReg(uint8_t addr);
void SPI_SendData(SPI_HandleTypeDef *hspi, uint16_t Data);
uint8_t SPI_ReceiveData(SPI_HandleTypeDef *hspi);

#endif

