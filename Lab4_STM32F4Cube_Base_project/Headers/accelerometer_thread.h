#ifndef _ACCELEROMETER_THREAD
#define _ACCELEROMETER_THREAD

#include <cmsis_os.h>

extern int length;
extern int state_dimension;
extern int measurement_dimension;
extern float accel_data_pitch, accel_data_roll;
extern osSemaphoreId sem_accel;
extern osThreadId accelerometer_thread_ID;
//extern kalman_state kstate;
extern int accel_sleep;

void accelerometer_thread_init(void);
void start_accelerometer_thread(void *args);
void accelerometer_thread(void const *args);
void get_pitch_value(void);
void get_roll_value(void);
	
#endif
