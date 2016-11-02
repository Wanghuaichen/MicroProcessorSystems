#include "modules/seven_segment.h"
#include "interfaces/seven_segment.h"

//import extern data variables
#include "modules/accelerometer.h"
#include "modules/keypad.h"
#include "modules/temperature.h"
#include "modules/led.h"

state display_state = INIT;
int special = 0;
float display_val = 0;

/*Brief: Refresh the display.
**Params: None
**Return: None
*/
void seven_segment_refresh(void) {
	//printf("seven seg\n");
	switch(display_state) {
		case INIT:
			special = DASHES;
			break;
		case ACCEL:
			special = DIGITS;
			break;
		case TEMP:
			break;
		case KEYPAD:
			display_val = 
			break;
		default:
			break;
	}
	display_2(display_val,special);
}
