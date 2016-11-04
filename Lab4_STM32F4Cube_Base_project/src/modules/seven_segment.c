#include "modules/seven_segment.h"
#include "interfaces/seven_segment.h"

//import extern data variables
#include "modules/accelerometer.h"
#include "modules/keypad.h"
#include "modules/temperature.h"
#include "modules/led.h"

Display_State display_state = ACCEL;
extern float accel_data;
int special;
float display_val;

/*Brief: Refresh the display.
**Params: None
**Return: None
*/
void seven_segment_refresh(void) {
	//display_state = ACCEL;
	//printf("seven seg\n");
	switch(display_state) {
		case INIT:
			special = DASHES;
			display_val = 1;
			break;
		case ACCEL:
			special = DEGREES;
			display_val = accel_data;
			break;
		case TEMP:
			special = CELSIUS;
			display_val = temp_data;
			break;
		case KEYPAD:
			special = KEYS;
			display_val = key_data;
			break;
		default:
			break;
	}
	display_2(display_val,special);
}
