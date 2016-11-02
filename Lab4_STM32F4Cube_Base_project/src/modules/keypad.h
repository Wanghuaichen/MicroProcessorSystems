#ifndef _KEYPAD
#define _KEYPAD
#define KEY_A 67//65
#define KEY_B 68//66
#define KEY_C 69//67
#define KEY_D 70//68
#define KEY_POUND 66 //35
#define KEY_STAR 65 //42

extern int value;
extern int key_data;
typedef enum {K_INIT, K_SEL, K_INPUT, K_PIEZO, K_TILT, K_RESETZ} Keypad_State;

/*Brief: Get key on timer flag high. If no key is pressed the value of key_data will not be updated.
**Params: None
**Return: None
*/
void keypad_get_key(void);
void key_for_select(void);
void key_for_input(void);
void poll_for_escape(void);

#endif