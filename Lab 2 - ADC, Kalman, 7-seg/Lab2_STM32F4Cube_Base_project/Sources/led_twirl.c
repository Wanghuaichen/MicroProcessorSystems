////////////////////////////////////////////////////////////////////////////////
//	File Name					: gpio_example.c
//	Description				: Sample code to help use the GPIO driver
//	Author						: Harsh Aurora
//	Date							: Oct 1st, 2016
////////////////////////////////////////////////////////////////////////////////
	
//		Includes		//
#include <stm32f4xx_hal.h>
#include <supporting_functions.h>
#include <led_twirl.h>

/*Brief: Controls the circle of LEDs on the STM board. LED's are referenced
**       using the cardinal directions.
**Params: None
**Return: None
*/
void led_twirl_init(void) { //initializes the 4 LEDs to GPIO ports configured in the push/pull setup
	GPIO_InitTypeDef LED_N_InitDef;
	GPIO_InitTypeDef LED_S_InitDef;
	GPIO_InitTypeDef LED_E_InitDef;
	GPIO_InitTypeDef LED_W_InitDef;	
	CLK_EN; 									
	
	LED_N_InitDef.Pin = LED_N; 
	LED_N_InitDef.Mode = GPIO_MODE_OUTPUT_PP;   			
	LED_N_InitDef.Pull = GPIO_NOPULL;
	LED_N_InitDef.Speed = GPIO_SPEED_FREQ_MEDIUM;

	LED_S_InitDef.Pin = LED_S; 
	LED_S_InitDef.Mode = GPIO_MODE_OUTPUT_PP;   			
	LED_S_InitDef.Pull = GPIO_NOPULL;
	LED_S_InitDef.Speed = GPIO_SPEED_FREQ_MEDIUM;

	LED_E_InitDef.Pin = LED_E; 
	LED_E_InitDef.Mode = GPIO_MODE_OUTPUT_PP;   			
	LED_E_InitDef.Pull = GPIO_NOPULL;
	LED_E_InitDef.Speed = GPIO_SPEED_FREQ_MEDIUM;

	LED_W_InitDef.Pin = LED_W; 
	LED_W_InitDef.Mode = GPIO_MODE_OUTPUT_PP;   			
	LED_W_InitDef.Pull = GPIO_NOPULL;
	LED_W_InitDef.Speed = GPIO_SPEED_FREQ_MEDIUM;	
	
	HAL_GPIO_Init(PORT, &LED_N_InitDef);
	HAL_GPIO_Init(PORT, &LED_S_InitDef);
	HAL_GPIO_Init(PORT, &LED_E_InitDef);
	HAL_GPIO_Init(PORT, &LED_W_InitDef);

	HAL_GPIO_WritePin(PORT, LED_N, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PORT, LED_S, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PORT, LED_E, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PORT, LED_W, GPIO_PIN_RESET);
}

/*Brief: Turns on the LED corresponding to the input value for twirl
**Params: None
**Return: None
*/
void led_twirl(int twirl) {
	clear_leds();
	switch (twirl) {
		case 0:
			HAL_GPIO_WritePin(PORT, LED_N, GPIO_PIN_SET);
			//printf("led %d\n",twirl);
			break;
		case 1:
			HAL_GPIO_WritePin(PORT, LED_W, GPIO_PIN_SET);
			//printf("led %d\n",twirl);
			break;
		case 3:
			HAL_GPIO_WritePin(PORT, LED_S, GPIO_PIN_SET);
			//printf("led %d\n",twirl);
			break;
		case 4:
			HAL_GPIO_WritePin(PORT, LED_E, GPIO_PIN_SET);
			//printf("led %d\n",twirl);
			break;
		default:
			break;
	}
}
/*Brief: Clear LEDs
**Params: None
**Return: None
*/
void clear_leds(void) {
	HAL_GPIO_WritePin(PORT, LED_N, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PORT, LED_S, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PORT, LED_E, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PORT, LED_W, GPIO_PIN_RESET);
}
