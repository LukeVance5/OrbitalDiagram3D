#include "Keyboard.h"

bool Keyboard::keys[GLFW_KEY_LAST] = { false };
bool Keyboard::keysChanged[GLFW_KEY_LAST] = { false };
bool Keyboard::inputEnabled = true;

void Keyboard::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (!inputEnabled)
		return;
	if (action != GLFW_RELEASE) {
		if (!keys[key]) {
			keys[key] = true;
		}
	}
	else {
		keys[key] = false;
	}

	keysChanged[key] = action != GLFW_REPEAT;
}

bool Keyboard::key(int key) {
	if (key < 0 || key >= GLFW_KEY_LAST) {
		return false;
	}
	return keys[key];
}
bool Keyboard::keyChanged(int key) {
	bool ret = keysChanged[key];
	keysChanged[key] = false;
	return ret;
}

bool Keyboard::keyWentUp(int key) {
	if (key < 0 || key >= GLFW_KEY_LAST) {
		return false;
	}
	return keyChanged(key) && !keys[key];
}
bool Keyboard::keyWentDown(int key) {
	if (key < 0 || key >= GLFW_KEY_LAST) {
		return false;
	}
	return keyChanged(key) && keys[key];
}

void Keyboard::setEnabled(bool enabled)
{
	inputEnabled = enabled;
}

bool Keyboard::isEnabled()
{
	return inputEnabled;
}