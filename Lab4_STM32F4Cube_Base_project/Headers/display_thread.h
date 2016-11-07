#ifndef _DISPLAY_THREAD
#define _DISPLAY_THREAD

// digit locations in the 7-segment
#define DIG_1 1
#define DIG_2 2
#define DIG_3 3
#define DIG_4 4
#define C 10

#define DIGIT   0	//	xxx.x  format
#define DASHES  1	//	----	 dashes
#define LETTER  2	//	   X 	 format
#define DEGREES 3	//	xxx.x° format
#define CELSIUS 4	//	xx.x°C format
#define KEYS    5	//	xxxx 	 format

#define CLK_DISPLAY_PORT_CLK_EN __HAL_RCC_GPIOB_CLK_ENABLE();

//		Exported Functios		//
void display_thread_periph_init(void);
void seven_segment_reset(void);
void start_display_thread(void *args);
void display_thread(void const *args);
void seven_segment_display(float data, int special);
void seven_segment_refresh(void);

#endif