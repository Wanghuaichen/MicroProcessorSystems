#ifndef _KEYPAD_THREAD
#define _KEYPAD_THREAD

#define KEY_A 67//65
#define KEY_B 68//66
#define KEY_C 69//67
#define KEY_D 70//68
#define KEY_POUND 66 //35
#define KEY_STAR 65 //42

typedef enum {K_INIT, K_SEL, K_INPUT, K_POLL_ESCAPE, RESETZ} Keypad_State;
typedef enum {INIT, ACCEL_PITCH, ACCEL_ROLL, TEMP, KEYPAD} state;

extern int value;
extern int key_data;
extern state display_state;

/*Brief: Get key on timer flag high. If no key is pressed the value of key_data will not be updated.
**Params: None
**Return: None
*/

void start_keypad_thread(void *args);
void keypad_thread(void const *args);
void keypad_get_key(void);
void keypad_select(void);
void keypad_input(void);
void keypad_escape(void);

#endif
