#pragma warning( disable: 6011 )

#include <stdlib.h>
#include <glfw3.h>
#include "input.h"

KeyboardState prevKeyState;
KeyboardState keyState;
MouseState prevMouseState;
MouseState mouseState;

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

void updateInputManager(GLFWwindow* window) {

	//update keyboard input
	if (prevKeyState == NULL) {
		prevKeyState = (KeyboardState)malloc(sizeof(struct KeyboardState));
		prevKeyState->key_escape = (KeyState)malloc(sizeof(struct KeyState));
		prevKeyState->key_W = (KeyState)malloc(sizeof(struct KeyState));
		prevKeyState->key_A = (KeyState)malloc(sizeof(struct KeyState));
		prevKeyState->key_S = (KeyState)malloc(sizeof(struct KeyState));
		prevKeyState->key_D = (KeyState)malloc(sizeof(struct KeyState));
		prevKeyState->key_1 = (KeyState)malloc(sizeof(struct KeyState));
		prevKeyState->key_2 = (KeyState)malloc(sizeof(struct KeyState));
		prevKeyState->key_3 = (KeyState)malloc(sizeof(struct KeyState));
		prevKeyState->key_4 = (KeyState)malloc(sizeof(struct KeyState));
		prevKeyState->key_5 = (KeyState)malloc(sizeof(struct KeyState));
		prevKeyState->key_6 = (KeyState)malloc(sizeof(struct KeyState));
		prevKeyState->key_7 = (KeyState)malloc(sizeof(struct KeyState));
		prevKeyState->key_8 = (KeyState)malloc(sizeof(struct KeyState));
		prevKeyState->key_9 = (KeyState)malloc(sizeof(struct KeyState));
		prevKeyState->key_0 = (KeyState)malloc(sizeof(struct KeyState));
	}
	if (keyState == NULL) {
		keyState = (KeyboardState)malloc(sizeof(struct KeyboardState));
		keyState->key_escape = (KeyState)malloc(sizeof(struct KeyState));
		keyState->key_W = (KeyState)malloc(sizeof(struct KeyState));
		keyState->key_A = (KeyState)malloc(sizeof(struct KeyState));
		keyState->key_S = (KeyState)malloc(sizeof(struct KeyState));
		keyState->key_D = (KeyState)malloc(sizeof(struct KeyState));
		keyState->key_1 = (KeyState)malloc(sizeof(struct KeyState));
		keyState->key_2 = (KeyState)malloc(sizeof(struct KeyState));
		keyState->key_3 = (KeyState)malloc(sizeof(struct KeyState));
		keyState->key_4 = (KeyState)malloc(sizeof(struct KeyState));
		keyState->key_5 = (KeyState)malloc(sizeof(struct KeyState));
		keyState->key_6 = (KeyState)malloc(sizeof(struct KeyState));
		keyState->key_7 = (KeyState)malloc(sizeof(struct KeyState));
		keyState->key_8 = (KeyState)malloc(sizeof(struct KeyState));
		keyState->key_9 = (KeyState)malloc(sizeof(struct KeyState));
		keyState->key_0 = (KeyState)malloc(sizeof(struct KeyState));
	}
	keyState->key_escape->down = glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
	keyState->key_W->down = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
	keyState->key_A->down = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
	keyState->key_S->down = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
	keyState->key_D->down = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;
	keyState->key_1->down = glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS;
	keyState->key_2->down = glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS;
	keyState->key_3->down = glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS;
	keyState->key_4->down = glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS;
	keyState->key_5->down = glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS;
	keyState->key_6->down = glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS;
	keyState->key_7->down = glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS;
	keyState->key_8->down = glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS;
	keyState->key_9->down = glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS;
	keyState->key_0->down = glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS;

	keyState->key_escape->pressed = (keyState->key_escape->down && !prevKeyState->key_escape->down);
	keyState->key_W->pressed = (keyState->key_W->down && !prevKeyState->key_W->down);
	keyState->key_A->pressed = (keyState->key_A->down && !prevKeyState->key_A->down);
	keyState->key_S->pressed = (keyState->key_S->down && !prevKeyState->key_S->down);
	keyState->key_D->pressed = (keyState->key_D->down && !prevKeyState->key_D->down);
	keyState->key_1->pressed = (keyState->key_1->down && !prevKeyState->key_1->down);
	keyState->key_2->pressed = (keyState->key_2->down && !prevKeyState->key_2->down);
	keyState->key_3->pressed = (keyState->key_3->down && !prevKeyState->key_3->down);
	keyState->key_4->pressed = (keyState->key_4->down && !prevKeyState->key_4->down);
	keyState->key_5->pressed = (keyState->key_5->down && !prevKeyState->key_5->down);
	keyState->key_6->pressed = (keyState->key_6->down && !prevKeyState->key_6->down);
	keyState->key_7->pressed = (keyState->key_7->down && !prevKeyState->key_7->down);
	keyState->key_8->pressed = (keyState->key_8->down && !prevKeyState->key_8->down);
	keyState->key_9->pressed = (keyState->key_9->down && !prevKeyState->key_9->down);
	keyState->key_0->pressed = (keyState->key_0->down && !prevKeyState->key_0->down);

	keyState->key_escape->released = (!keyState->key_escape->down && prevKeyState->key_escape->down);
	keyState->key_W->released = (!keyState->key_W->down && prevKeyState->key_W->down);
	keyState->key_A->released = (!keyState->key_A->down && prevKeyState->key_A->down);
	keyState->key_S->released = (!keyState->key_S->down && prevKeyState->key_S->down);
	keyState->key_D->released = (!keyState->key_D->down && prevKeyState->key_D->down);
	keyState->key_1->released = (!keyState->key_1->down && prevKeyState->key_1->down);
	keyState->key_2->released = (!keyState->key_2->down && prevKeyState->key_2->down);
	keyState->key_3->released = (!keyState->key_3->down && prevKeyState->key_3->down);
	keyState->key_4->released = (!keyState->key_4->down && prevKeyState->key_4->down);
	keyState->key_5->released = (!keyState->key_5->down && prevKeyState->key_5->down);
	keyState->key_6->released = (!keyState->key_6->down && prevKeyState->key_6->down);
	keyState->key_7->released = (!keyState->key_7->down && prevKeyState->key_7->down);
	keyState->key_8->released = (!keyState->key_8->down && prevKeyState->key_8->down);
	keyState->key_9->released = (!keyState->key_9->down && prevKeyState->key_9->down);
	keyState->key_0->released = (!keyState->key_0->down && prevKeyState->key_0->down);

	prevKeyState->key_escape->down = keyState->key_escape->down;
	prevKeyState->key_W->down = keyState->key_W->down;
	prevKeyState->key_A->down = keyState->key_A->down;
	prevKeyState->key_S->down = keyState->key_S->down;
	prevKeyState->key_D->down = keyState->key_D->down;
	prevKeyState->key_1->down = keyState->key_1->down;
	prevKeyState->key_2->down = keyState->key_2->down;
	prevKeyState->key_3->down = keyState->key_3->down;
	prevKeyState->key_4->down = keyState->key_4->down;
	prevKeyState->key_5->down = keyState->key_5->down;
	prevKeyState->key_6->down = keyState->key_6->down;
	prevKeyState->key_7->down = keyState->key_7->down;
	prevKeyState->key_8->down = keyState->key_8->down;
	prevKeyState->key_9->down = keyState->key_9->down;
	prevKeyState->key_0->down = keyState->key_0->down;

	//update mouse input
	if (prevMouseState == NULL) {
		prevMouseState = (MouseState)malloc(sizeof(struct MouseState));
		prevMouseState->left = (KeyState)malloc(sizeof(struct KeyState));
		prevMouseState->right = (KeyState)malloc(sizeof(struct KeyState));
		prevMouseState->wheel = (KeyState)malloc(sizeof(struct KeyState));
	}
	if (mouseState == NULL) {
		mouseState = (MouseState)malloc(sizeof(struct MouseState));
		mouseState->left = (KeyState)malloc(sizeof(struct KeyState));
		mouseState->right = (KeyState)malloc(sizeof(struct KeyState));
		mouseState->wheel = (KeyState)malloc(sizeof(struct KeyState));

		mouseState->scrollVal = 0.0f;
		glfwSetScrollCallback(window, scrollCallback);
	}
	mouseState->left->down = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
	mouseState->right->down = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
	mouseState->wheel->down = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS;

	mouseState->left->pressed = (mouseState->left->down && !prevMouseState->left->down);
	mouseState->right->pressed = (mouseState->right->down && !prevMouseState->right->down);
	mouseState->wheel->pressed = (mouseState->wheel->down && !prevMouseState->wheel->down);

	mouseState->left->released = (!mouseState->left->down && prevMouseState->left->down);
	mouseState->right->released = (!mouseState->right->down && prevMouseState->right->down);
	mouseState->wheel->released = (!mouseState->wheel->down && prevMouseState->wheel->down);

	prevMouseState->left->down = mouseState->left->down;
	prevMouseState->right->down = mouseState->right->down;
	prevMouseState->wheel->down = mouseState->wheel->down;

	//update mouse position
	int windowX, windowY;
	glfwGetFramebufferSize(window, &windowX, &windowY);
	glfwGetCursorPos(window, &mouseState->posX, &mouseState->posY);
	mouseState->posX /= 0.5 * (double)windowX;
	mouseState->posY /= -0.5 * (double)windowY;
	mouseState->posX -= 1.0;
	mouseState->posY += 1.0;
	if (mouseState->posX < -1.0) mouseState->posX = -1.0;
	if (mouseState->posX > 1.0) mouseState->posX = 1.0;
	if (mouseState->posY < -1.0) mouseState->posY = -1.0;
	if (mouseState->posY > 1.0) mouseState->posY = 1.0;
	if (windowX > windowY) mouseState->posY *= (double)windowY / (double)windowX;
	if (windowY > windowX) mouseState->posX *= (double)windowX / (double)windowY;
}

void updateInputManagerFixedTime(GLFWwindow* window) {
	mouseState->scrollVal = mouseState->scrollVal * .3f;
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	mouseState->scrollVal += (float)yoffset;
	if (mouseState->scrollVal < -2.0f) mouseState->scrollVal = -2.0f;
	if (mouseState->scrollVal > 2.0f) mouseState->scrollVal = 2.0f;
}

void InputManager_free() {
	free(prevKeyState->key_escape);
	free(prevKeyState->key_W);
	free(prevKeyState->key_A);
	free(prevKeyState->key_S);
	free(prevKeyState->key_D);
	free(prevKeyState->key_1);
	free(prevKeyState->key_2);
	free(prevKeyState->key_3);
	free(prevKeyState->key_4);
	free(prevKeyState->key_5);
	free(prevKeyState->key_6);
	free(prevKeyState->key_7);
	free(prevKeyState->key_8);
	free(prevKeyState->key_9);
	free(prevKeyState->key_0);
	free(prevKeyState);

	free(keyState->key_escape);
	free(keyState->key_W);
	free(keyState->key_A);
	free(keyState->key_S);
	free(keyState->key_D);
	free(keyState->key_1);
	free(keyState->key_2);
	free(keyState->key_3);
	free(keyState->key_4);
	free(keyState->key_5);
	free(keyState->key_6);
	free(keyState->key_7);
	free(keyState->key_8);
	free(keyState->key_9);
	free(keyState->key_0);
	free(keyState);

	free(prevMouseState->left);
	free(prevMouseState->right);
	free(prevMouseState->wheel);
	free(prevMouseState);

	free(mouseState->left);
	free(mouseState->right);
	free(mouseState->wheel);
	free(mouseState);
}