#include "InputManager.h"

#include <iostream>
#include <ostream>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_cursor_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

InputManager::InputManager()
{
	m_lastX = 0.0f;
	m_lastY = 0.0f;
}

//start the input manager
int InputManager::initialize(GLFWwindow* window)
{
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_cursor_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, scroll_callback);

	m_mouseMoveEvent = Event<glm::vec2, glm::vec2>();

	return 0;
}

//free any data on the heap
void InputManager::terminate()
{

}

//singleton get function
InputManager& InputManager::getInstance()
{
	static InputManager single;
	return single;
}

// Input function for mouse movement
void InputManager::mouseMoveInput(float posX, float posY)
{
	if (m_firstMouse)
	{
		m_lastX = posX;
		m_lastY = posY;
		m_firstMouse = false;
	}

	const float offsetX = posX - m_lastX;
	const float offsetY = posY - m_lastY;

	m_lastX = posX;
	m_lastY = posY;

	m_mouseMoveEvent.callEvent(glm::vec2(posX, posY), glm::vec2(offsetX, offsetY));
}

// Input function for keyboard inputs
void InputManager::keyboardInputs(int key, int scancode, int action, int mods) 
{
	m_keyboardEvent.callEvent(KeyboardContext(key, action, mods));
}

// Input function for mouse button inputs
void InputManager::mouseButtonInputs(int button, int action, int mods)
{
	m_mouseButtonEvent.callEvent(MouseButtonContext(button, action, mods));
}

void InputManager::scrollInput(double xOffset, double yOffset)
{
	m_scrollEvent.callEvent(glm::vec2(float(xOffset), float(yOffset)));
}

#pragma region callbacks
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Input.keyboardInputs(key, scancode, action, mods);
}

void mouse_cursor_callback(GLFWwindow* window, double xpos, double ypos)
{
	Input.mouseMoveInput(float(xpos), float(ypos));
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	Input.mouseButtonInputs(button, action, mods);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Input.scrollInput(xoffset, yoffset);
}
#pragma endregion
