////////////////////////////////////////////////////////////////////////////////
//	File Name					: LED_thread.h
//	Description				: Header file for LED thread
//	Author						: Harsh Aurora
//	Date							: Oct 28, 2016
////////////////////////////////////////////////////////////////////////////////

#ifndef _LED_THREAD
#define _LED_THREAD

#include <cmsis_os.h>

extern osThreadId LED_thread_ID;

//		Exported Functios		//
void start_LED_thread(void *args);

#endif
