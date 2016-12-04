////////////////////////////////////////////////////////////////////////////////
//	File Name					: gpio_example.h
//	Description				: header file for gpio_example.c
//	Author						: Harsh Aurora
//	Date							: Oct 1st, 2016
////////////////////////////////////////////////////////////////////////////////

#ifndef _SEGMENTS_DISPLAY
#define _SEGMENTS_DISPLAY

#define EXAMPLE_PORT GPIOD
#define EXAMPLE_PIN GPIO_PIN_12 // Pin D12 is connected to the onboard green LED
#define orange_Led GPIO_PIN_13 // Pin D13 is connected to the onboard orange LED
#define red_Led GPIO_PIN_14 // Pin D14 is connected to the onboard red LED
#define blue_Led GPIO_PIN_15 // Pin D15 is connected to the onboard blue LED

#define EXAMPLE_PORT_CLK_EN __HAL_RCC_GPIOD_CLK_ENABLE();
#define A_Segment_Pin GPIO_PIN_7
#define B_Segment_Pin GPIO_PIN_13
#define C_Segment_Pin GPIO_PIN_14
#define D_Segment_Pin GPIO_PIN_15
#define E_Segment_Pin GPIO_PIN_0
#define F_Segment_Pin GPIO_PIN_1
#define G_Segment_Pin GPIO_PIN_11
#define DP_Segment_Pin GPIO_PIN_11
#define selectLine0 GPIO_PIN_12
#define selectLine1 GPIO_PIN_13
#define selectLine2 GPIO_PIN_14
#define selectLine3 GPIO_PIN_15

void setClearDisplays(int value);
void start_display_thread(void *args);
void display_thread (void const *argument);
void displays_gpio_init(void);
void gpio_example(void);
void toggleDisplays(int display);
void setSegmentA(void);
void setSegmentB(void);
void setSegmentC(void);
void setSegmentD(void);
void setSegmentE(void);
void setSegmentF(void);
void setSegmentG(void);
void setSegmentDP(void);
void resetDisplay(void);
void displayNumber(void);
void displayDigit(int digit);
void setNumberToDisplay(float number);
void setGreenLED(void);
void resetGreenLED(void);
void setOrangeLED(void);
void resetOrangeLED(void);
void setRedLED(void);
void resetRedLED(void);
void setBlueLED(void);
void resetBlueLED(void);
void resetLEDs(void);
void circularLEDs(void);

#endif
