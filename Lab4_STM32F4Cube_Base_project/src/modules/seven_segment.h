#ifndef _SEVEN_SEGMENT
#define _SEVEN_SEGMENT

#define DASHES 0
#define ANGEL 1
#define CELSIUS 2
#define DIGIT 3

typedef enum { INIT, SEL, INPUT, PIEZO, TILT, RESETZ, ACCEL, TEMP, KEYPAD } Display_State;

/*Brief: Refresh the display.
**Params: None
**Return: None
*/
void seven_segment_refresh(void);

#endif