#include "utils.h"

bool monitor_for_change(int var, int *mem) {
	if(*mem==var) return false;
	*mem = var;
	return true;
}