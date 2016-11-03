#ifndef _SEVEN_SEGMENT
#define _SEVEN_SEGMENT

//remove: these are in interfaces/seven_segment.h
//special inputs
#define DIGIT 0		//	xxx.x 	format
#define DASHES 1	//	----	dashes
#define LETTER 2	//	   X 	format
#define DEGREES 3	//	xxx.x° 	format
#define CELSIUS 4	//	xx.x°C 	format
#define KEYS 5		//	xxxx 	format

typedef enum { INIT, ACCEL, TEMP, KEYPAD } Display_State;

/*Brief: Refresh the display.
**Params: None
**Return: None
*/
void seven_segment_refresh(void);

#endif
