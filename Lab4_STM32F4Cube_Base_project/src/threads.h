#ifndef __THREADS_H
#define __THREADS_H

#include "utils/utils.h"
#include <cmsis_os.h>

extern bool accel_int_flag;
extern bool keypad_tim_flag;
extern bool temp_tim_flag;
extern bool led_tim_flag;
extern bool seven_segment_tim_flag;

extern osThreadId thread1;
extern osThreadId thread2;
extern osThreadId thread3;
extern osThreadId thread4;
extern osThreadId thread5;

 void accelerometer_thread(void const* args);
 void keypad_thread(void const* args);
 void temperature_thread(void const* args);
 void led_thread(void const* args);
 void seven_segment_thread(void const* args);
 
 void run_threads(void);

#endif
 