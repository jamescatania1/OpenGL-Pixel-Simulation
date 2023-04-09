#ifndef _input_h
#define _input_h

typedef struct KeyState {
	int down;
	int pressed;
	int released;
}
*KeyState;

typedef struct KeyboardState {
	KeyState key_escape, key_W, key_A, key_S, key_D, key_0, key_1, key_2, key_3, key_4, key_5, key_6, key_7, key_8, key_9;
}
*KeyboardState;

typedef struct MouseState {
	KeyState left, right, wheel;
	double posX, posY;
	
	/*
	 * Scroll up : positive (maximum 2.0)
	 * Scroll down : negative (minimum -2.0)
	 * Else : 0.0;
	 */
	double scrollVal;
}
*MouseState;

/*
 * The keyboard state in its current form.
 */
KeyboardState keyState;

/*
 * The mouse state in its current form.
 */
MouseState mouseState;

/*
 * Updates the input manager's keyboard information. Call this in the update cycle.
 */
extern void updateInputManager(GLFWwindow* window);

extern void updateInputManagerFixedTime(GLFWwindow* window);

extern void InputManager_free();

#endif