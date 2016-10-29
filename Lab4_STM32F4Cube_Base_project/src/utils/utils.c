#include "utils.h"

int mem[10];

bool monitor_for_change(int var, int *mem) {
	if(*mem==var) return false;
	*mem = var;
	return true;
}