#ifndef __THREADS_H
#define __THREADS_H

 void accelerometer_thread(void const* args);
 void keypad_thread(void const* args);
 void temperature_thread(void const* args);
 void led_thread(void const* args);
 void display_thread(void const* args);

#endif