#include "bool.h"
#ifndef _UTILS
#define _UTILS

#define MEM_ACCEL 5

bool monitor_for_change(int var, int *mem);
extern int mem[10];
extern int thread_flag;

#endif
