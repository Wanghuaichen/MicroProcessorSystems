#ifndef _UTILS
#define _UTILS

#include "bool.h"

#define MEM_COLS 0
#define MEM_ROWS 1
#define MEM_DIGIT 2
#define MEM_KEY 3
#define MEM_PIEZO 4
#define MEM_ACCEL 5

bool monitor_for_change(int var, int *mem);

extern int mem[10];

#endif