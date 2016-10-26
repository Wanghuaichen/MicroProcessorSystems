#ifndef _GAME
#define _GAME

typedef enum { INIT, SEL, PIEZO_INPUT, TILT_INPUT, PIEZO, TILT, RESET } Game_State;
#define KEY_A 67
#define KEY_B 68
#define KEY_C 69
#define KEY_D 70

void play(void);

#endif