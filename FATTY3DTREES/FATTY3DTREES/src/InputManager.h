#pragma once
#include <GLFW/glfw3.h>
#include <unordered_map>


namespace InputManager
{
	void Init(GLFWwindow* window);
	void Poll(GLFWwindow* window);
	int GetKeyState(int GLFWKeyCode);
	int GetKeyToggle(int GLFWKeyCode);
	float GetMouseX();
	float GetMouseY();
	float GetMouseScrollYOffset();
	float GetDeltaMouseX();
	float GetDeltaMouseY();
};
