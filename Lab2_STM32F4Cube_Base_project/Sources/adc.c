////////////////////////////////////////////////////////////////////////////////
//	File Name					: adc_example.c
//	Description				: Sample code to help use the ADC driver
//	Author						: Harsh Aurora
//	Date							: Oct 1st, 2016
////////////////////////////////////////////////////////////////////////////////
	
//		Includes		//
#include <stm32f4xx_hal.h>
#include <supporting_functions.h>
#include <adc.h>

//		Global variables		//
ADC_HandleTypeDef ADC_Handle;

/*Brief: Sets up the desired(refer to header) ADC and corresponding GPIO for input
**Params: None
**Return: None
*/
void adc_init(void) {
	ADC_InitTypeDef ADC_Init; 																	// definition of ADC1 initialiation struct
	ADC_ChannelConfTypeDef ADC_Channel;												// definition of ADC1 channel struct
	HAL_LockTypeDef ADC_Lock; 																	// define ADC1 locking object
	ADC_MultiModeTypeDef ADC_Mode; 														// define ADC1 mode struct
	
	/*  initialize ADC init struct */
	ADC_Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;					// ADC Clock frequency 42MHz (168/4)
	ADC_Init.Resolution = ADC_RESOLUTION_12B;										// 12 bit resolution, better but slower
	ADC_Init.DataAlign = ADC_DATAALIGN_RIGHT;										// align the 12 bits data at the right of the 32 bits words
	ADC_Init.ScanConvMode = DISABLE;														// single channel mode
	ADC_Init.EOCSelection = ADC_EOC_SEQ_CONV;										// perform ADC conversions without having to read all conversion data
	ADC_Init.ContinuousConvMode = DISABLE;											// single mode convertion
	ADC_Init.DMAContinuousRequests = DISABLE;										// single mode DMA request
	ADC_Init.NbrOfConversion = 1;																// one conversion
	ADC_Init.DiscontinuousConvMode = ENABLE;										// enable discountinuous mode
	ADC_Init.NbrOfDiscConversion = 1;														// one conversion
	ADC_Init.ExternalTrigConv = ADC_SOFTWARE_START;							// no external trigger
	ADC_Init.ExternalTrigConvEdge = ADC_SOFTWARE_START;					// no external trigger
	
	/* initialize ADC handle struct */
	ADC_Handle.Instance = ADC;
	ADC_Handle.Init = ADC_Init;
	ADC_Handle.NbrOfCurrentConversionRank = 1;
	ADC_Handle.Lock = ADC_Lock;
	ADC_Handle.State = 0;
	ADC_Handle.ErrorCode = HAL_ADC_ERROR_NONE;
	
	/* initialize ADC channel struct */
	ADC_Channel.Channel = ADC_CHANNEL;
	ADC_Channel.Rank = 1;																			// use to determine the rank in which this channel is sampled
	ADC_Channel.SamplingTime = ADC_SAMPLETIME_480CYCLES;				// time for the internal capacitor to charge. longuer means more accurate
	ADC_Channel.Offset = 0;
	
	/* initialize ADC mode struct */
	ADC_Mode.Mode = ADC_MODE_INDEPENDENT;														
	ADC_Mode.DMAAccessMode = ADC_DMAACCESSMODE_DISABLED;
	ADC_Mode.TwoSamplingDelay = ADC_TWOSAMPLINGDELAY_5CYCLES;
	
	ADC_CLK_EN;
	HAL_ADC_ConfigChannel(&ADC_Handle, &ADC_Channel);
	HAL_ADCEx_MultiModeConfigChannel(&ADC_Handle, &ADC_Mode);	/* configure mode */

	GPIO_InitTypeDef GPIO_InitDef; 								
	ADC_GPIO_PORT_CLK_EN; 									
	
	GPIO_InitDef.Pin = ADC_GPIO_PIN; 	
	GPIO_InitDef.Mode = GPIO_MODE_ANALOG;   			
	GPIO_InitDef.Pull = GPIO_PULLDOWN;
	GPIO_InitDef.Speed = GPIO_SPEED_FREQ_MEDIUM;		
	
	HAL_GPIO_Init(ADC_GPIO_PORT, &GPIO_InitDef);
}

/*Brief: Performs the steps to poll the ADC, and converts the obtained value to a voltage
**Params: None
**Return: None
*/
float adc_poll(void) {
	float val = 0;
	HAL_ADC_Start(&ADC_Handle);
	if(HAL_ADC_PollForConversion(&ADC_Handle, POLL_TIMEOUT) == HAL_OK)
		val = HAL_ADC_GetValue(&ADC_Handle);
	HAL_ADC_Stop(&ADC_Handle);
	return ((val*3.0f)/4096.0f*1000) / 2.5f + 25;  //voltage*1/slope-offset    // 2.5mV/degC   |    0.76V = 25degC
}

/*float poll_adc(float *val, int flag) {
	if(flag == 1) {
		HAL_ADC_Start(&example_ADC_Handle);
		if(HAL_ADC_PollForConversion(&example_ADC_Handle, POLL_TIMEOUT) == HAL_OK)
			*val = HAL_ADC_GetValue(&example_ADC_Handle);
		HAL_ADC_Stop(&example_ADC_Handle);
		*val = ((*val*3.0f)/4096.0f*1000) / 2.5f - 279;  //voltage*1/slope-offset    // 2.5mV/degC   |    0.76V = 25degC
	}
}*/
