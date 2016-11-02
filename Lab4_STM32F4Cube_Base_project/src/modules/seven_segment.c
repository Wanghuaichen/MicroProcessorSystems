#include "modules/seven_segment.h"
#include "interfaces/seven_segment.h"

//import extern data variables
#include "modules/accelerometer.h"
#include "modules/keypad.h"
#include "modules/temperature.h"
#include "modules/led.h"

Display_State display_state = INIT;
extern int special;
extern int display_val;

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
			special = ANGEL;
			break;
		case TEMP:
			special = CELSIUS;
			break;
		case KEYPAD:
			display_val = DIGIT;
			break;
		default:
			break;
	}
	display_2(display_val,special);
}
