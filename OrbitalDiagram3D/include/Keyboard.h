#pragma once
#ifndef KEYBOARD_H
#define KEYBOARD_H

#include<glad/glad.h>
#include <GLFW/glfw3.h>
#include "OrbitalDiagram3DExport.h"
class OD_API Keyboard {
public:
	// key state callback
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	/*
		accesors
	*/
	static bool key(int key);
	static bool keyChanged(int key);
	static bool keyWentUp(int key);
	static bool keyWentDown(int key);
	static bool isEnabled();
	static void setEnabled(bool enabled);
private:
	static bool inputEnabled;
	static bool keys[];
	static bool keysChanged[];
};
#endif