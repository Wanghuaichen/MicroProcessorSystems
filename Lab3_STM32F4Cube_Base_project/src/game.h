#ifndef _GAME
#define _GAME

typedef enum { INIT, SEL, INPUT, PIEZO, TILT, RESETZ } Game_State;

#define KEY_A 67//65
#define KEY_B 68//66
#define KEY_C 69//67
#define KEY_D 70//68
#define KEY_POUND 66 //35
#define KEY_STAR 65 //42

void play(void);

#endif