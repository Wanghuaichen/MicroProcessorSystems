#ifndef _GAME
#define _GAME

typedef enum { INIT, SEL, INPUT, PIEZO, TILT, RESETZ } Game_State;
#define KEY_A 65
#define KEY_B 66
#define KEY_C 67
#define KEY_D 68
#define KEY_POUND 35
#define KEY_STAR 42

void play(void);

#endif