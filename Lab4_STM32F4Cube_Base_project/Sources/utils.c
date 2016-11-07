////////////////////////////////////////////////////////////////////////////////
//	File Name					: utils.c
//	Description				: simple function for monitoring change
//	Author						: Alex Bhandari, Tianming Zhang 
//	Date							: Nov 6, 2016
////////////////////////////////////////////////////////////////////////////////

//include
#include "utils.h"

//Global
int mem[10];

/*Brief: if input variable is different than previous one, the return true
**Params: int var, int *mem
**Return: None
*/
bool monitor_for_change(int var, int *mem) {
	if(*mem==var) return false;
	*mem = var;
	return true;
}