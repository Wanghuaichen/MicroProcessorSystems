////////////////////////////////////////////////////////////////////////////////
//	File Name					: main.c
//	Description				: program entry
//	Author						: Harsh Aurora
//	Date							: Oct 1st, 2016
////////////////////////////////////////////////////////////////////////////////
	
//		Includes		//
#include <stm32f4xx_hal.h>
#include <supporting_functions.h>
#include <sysclk_config.h>
#include <gpio.h>
#include <adc.h>
#include <gpio_clk_display.h>
#include <kalmanfilter.h>
#include <stdlib.h>
#include <led_twirl.h>

//		Global variables		//
volatile int systick_flag; // flag to tell if a systick interrup has occured

//initialize the state variables for Kalmanfilter
kalman_state kstate = { .F = {1},
                        .H = {1},
                        .Q = {.1},
												.R = {0.7707},
												.X = {0},
												.P = {0.1},
												.K = {1},
											}; 

void insertArray(float* InputArray, float ADC_val){
	InputArray = realloc(InputArray,2*sizeof(int));
}
											
int main(void)
{
	int time_counter = 0;
	int alarm_counter = 0;
	int led_position = 0;
	int scan_digit = 1;
	int digit[5];
	float data = 0;
	int count = 0;
	int print_count = 0;
	int raw[100];
	int filt[100];
	
	//initialize input length, state matrix, and measurement dimensions for kalmanfilter
	const int Length=1;
  	const int State_dimension=1;
  	const int Measurement_dimension=1;
	
	//initialize input and ouput arrays for kalmanfilter
	float InputArray[Length*Measurement_dimension];
  	float OutputArray[Length*Measurement_dimension]; 

	
 	//		MCU Configuration		//
 	//	Reset of all peripherals, Initializes the Flash interface and the Systick	//
 	HAL_Init();
	
 	//	Configure the system clock	//
 	SystemClock_Config();
	
	//   LED config   // 
	led_twirl_init();

 	//	Example code initialization	//
	gpio_init();
	adc_init(); //<- works
	//adc_init(); // <- probably doesn't work
	gpio_clk_display_init();
	
	//	run sample code forever	//
	while(1) {
		
		// run every 1ms.
		if(systick_flag == 1) {
			systick_flag = 0;
			
			if(time_counter%10 == 0) { // runs every 10 ms	- refresh rate
				InputArray[0] = adc_poll(); //sample the ADC every 1ms
				kalmanfilter_c(InputArray, OutputArray, &kstate, Length, State_dimension, Measurement_dimension); //call kalmanfilter function in C
				printf("%f\t%f\n", InputArray[0],kstate.X[0]);
				/*if(count < 100 && InputArray[0] != 0) {
					raw[count] = InputArray[0];
					filt[count] = kstate.X[0];
					count++;
				}*/
			}
	/*		
			if(time_counter%100 == 0 && InputArray[0] != 0) { // T = 100 ms
				printf("%f\t%f\n", filt[print_count],raw[print_count]);
				print_count++;
				if(print_count > count) {
					count = 0;
					print_count = 0;
				}
			}
	*/		
			if(time_counter++ == 1000) { // runs every 1000 ms
				time_counter = 0;
				data = kstate.X[0];
			}
			
			//Updates the value of digit only at the start of a display refresh
			if(scan_digit == 1) {
				//sets digit[0..5] to xx.xUC (degrees C)
				digit[0] = (int)data/10;
				digit[1] = (int)data-digit[0]*10;
				digit[2] = (int)(data*10-digit[1]*10-digit[0]*100);
			  digit[3] = 10; //display C
				digit[4] = 13; //upper point LED - degrees sign
			}	
			
			//---Update 7 segment---
			seven_seg_reset();
			seven_seg_digit_display(scan_digit, digit[scan_digit-1], scan_digit == 2); //decimal used for 2nd digit
			if(scan_digit++ == 5) scan_digit = 1;
			//----------------------

			//gpio_toggle();
			//gpio_clk_display(&ADC_val);

			//-----Alarm--------
			if(kstate.X[0] > 50) {
				alarm_counter++;
				if(alarm_counter == 100) {
					alarm_counter = 0;
					led_twirl(led_position);
					led_position++;
					if(led_position > 4) led_position = 0;
				}		
			}
			//------------------

			//------Piezo-------

		}
	}
}
