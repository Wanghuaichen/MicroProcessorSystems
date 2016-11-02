#ifndef _SEVEN_SEGMENT
#define _SEVEN_SEGMENT

//remove: these are in interfaces/seven_segment.h
//#define DASHES 0
//#define ANGLE 1
//#define CELSIUS 2
//#define DIGIT 3

typedef enum { INIT, ACCEL, TEMP, KEYPAD } Display_State;

/*Brief: Refresh the display.
**Params: None
**Return: None
*/
void seven_segment_refresh(void);

#endif