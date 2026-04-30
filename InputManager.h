/*
 * Written by: AwokenOwen
 * Last Updated: March 25th 2026
 */

#pragma once
#include "Event.h"
#include "GLFW/glfw3.h"
#include "glm.hpp"
#include <string>

/**
 * @brief Singleton Macro
 */
#define Input InputManager::getInstance()


/**
 * @brief Used to send information about GLFW keyboard input through keyboard events
 */
struct KeyboardContext
{
private:
	/**
	 * @brief A code referring to which key was pressed on the keyboard
	 */
	int m_key;
	/**
	 * @brief A code referring to which key was pressed on the keyboard specific to OS
	 */
	int m_action;
	/**
	 * @brief A code referring to if the key was GLFW_PRESSED GLFW_RELEASED or GLFW_REPEAT
	 */
	int m_scancode;
	/**
	 * @brief A code with bit modifiers to represent the mods of the input (Held Ctrl, Shift, ect.)
	 */
	int m_mods;
public:
	/**
	 * @brief Keyboard Context Constructor
	 *
	 * @param key A code referring to which key was pressed on the keyboard
	 * @param action A code referring to which key was pressed on the keyboard specific to OS
	 * @param mods A code with bit modifiers to represent the mods of the input (Held Ctrl, Shift, ect.)
	 */
	KeyboardContext(const int key, const int action, const int mods) {
		this->m_key = key;
		this->m_action = action;
		this->m_mods = mods;

		m_scancode = glfwGetKeyScancode(key);
	}

	/**
	 * @brief Getter for key
	 *
	 * @return int
	 */
	[[nodiscard]] int getKey() const {
		return m_key;
	}

	/**
	 * @brief Getter for Action
	 *
	 * @return int
	 */
	[[nodiscard]] int getAction() const {
		return m_action;
	}

	/**
	 * @brief Getter for Scancode
	 *
	 * @return int
	 */
	[[nodiscard]] int getScancode() const {
		return m_scancode;
	}

	/**
	 * @brief Getter for mods
	 *
	 * @return int
	 */
	[[nodiscard]] int getMods() const {
		return m_mods;
	}
};

/**
 * @brief Used to send information about GLFW mouse button input through keyboard events
 */
struct MouseButtonContext
{
private:
	/**
	 * @brief A code referring to the mouse button pressed
	 */
	int m_button;
	/**
	 * @brief A code referring to which key was pressed on the keyboard specific to OS
	 */
	int m_action;
	/**
	 * @brief A code with bit modifiers to represent the mods of the input (Held Ctrl, Shift, ect.)
	 */
	int m_mods;
public:
	/**
	 * @brief Mouse Button Context Constructor
	 *
	 * @param button A code referring to the mouse button pressed
	 * @param action A code referring to which key was pressed on the keyboard specific to OS
	 * @param mods A code with bit modifiers to represent the mods of the input (Held Ctrl, Shift, ect.)
	 */
	MouseButtonContext(const int button, const int action, const int mods)
	{
		this->m_button = button;
		this->m_action = action;
		this->m_mods = mods;
	}

	/**
	 * @brief Getter for Button
	 *
	 * @return int
	 */
	[[nodiscard]] int getButton() const
	{
		return m_button;
	}

	/**
	 * @brief Getter for Action
	 *
	 * @return int
	 */
	[[nodiscard]] int getAction() const
	{
		return m_action;
	}

	/**
	 * @brief Getter for Mods
	 *
	 * @return int
	 */
	[[nodiscard]] int getMods() const
	{
		return m_mods;
	}
};

/**
 * @brief Singleton class that handles the GLFW inputs into more user-friendly format
 */
class InputManager
{
public:
	/**
	 * @brief Get Input Manager running for use in game
	 *
	 * Starts up and initializes everything the input manager needs to run. Returns 0 if successful and 1 if failed
	 *
	 * @return int
	 */
	int initialize(GLFWwindow* window);

	/**
	 * @brief shuts down the input manager at the end of the game loop
	 */
	void terminate();

	/**
	 * @brief InputManager Singleton get function
	 *
	 * @return static InputManager Instance
	 */
	static InputManager& getInstance();

	/**
	 * @brief Function called by GLFW for when the mouse moves
	 *
	 * Takes in position of the mouse as two floats and calls the mouseMoveEvent with a vec2 for mouse position and vec2 for the mouse delta (the change in mouse position between frames)
	 *
	 * @param posX X position of the mouse
	 * @param posY Y position of the mouse
	 */
	void mouseMoveInput(float posX, float posY);

	/**
	 * @brief Function called by GLFW for all keyboard inputs
	 *
	 * Takes in the key, scancode, action, and mods and turns it into a KeyboardContext and calls the keyboardEvent sending the KeyboardContext
	 *
	 * @param key A code referring to which key was pressed on the keyboard
	 * @param scancode A code referring to which key was pressed on the keyboard specific to OS
	 * @param action A code referring to if the key was GLFW_PRESSED GLFW_RELEASED or GLFW_REPEAT
	 * @param mods A code with bit modifiers to represent the mods of the input (Held Ctrl, Shift, ect.)
	 */
	void keyboardInputs(int key, int scancode, int action, int mods);

	/**
	 * @brief Function called by GLFW for all mouse button inputs
	 *
	 * Takes in the button, action, and mods and turns it into a MouseContext and calls the mouseButtonEvent sending the MouseContext
	 *
	 * @param button A code referring to the mouse button pressed
	 * @param action A code referring to if the key was GLFW_PRESSED GLFW_RELEASED or GLFW_REPEAT
	 * @param mods A code with bit modifiers to represent the mods of the input (Held Ctrl, Shift, ect.)
	 */
	void mouseButtonInputs(int button, int action, int mods);

	/**
	 * @brief Function called by GLFW for all scroll inputs
	 *
	 * Takes in the offset of the scroll through two floats, converts it to a vec2 and sends it in the scrollEvent
	 *
	 * @param xOffset The offset of the scroll on the X Axis
	 * @param yOffset The offset of the scroll on the Y Axis
	 */
	void scrollInput(double xOffset, double yOffset);

	/**
	 * @brief add function to the mouseMoveEvent
	 *
	 * @tparam T Class of member function
	 * @param object Pointer to the Object
	 * @param func The function getting added as a listener
	 */
	template<typename T>
	void addToMouseMoveEvent(T* object, void(T::* func)(glm::vec2, glm::vec2));

	/**
	 * @brief add function to the keyboardEvent
	 *
	 * @tparam T Class of member function
	 * @param object Pointer to the Object
	 * @param func The function getting added as a listener
	 */
	template<typename T>
	void addToKeyboardEvent(T* object, void(T::* func)(KeyboardContext));

	/**
	 * @brief add function to the mouseButtonEvent
	 *
	 * @tparam T Class of member function
	 * @param object Pointer to the Object
	 * @param func The function getting added as a listener
	 */
	template<typename T>
	void addToMouseButtonEvent(T* object, void(T::* func)(MouseButtonContext));

	/**
	 * @brief add function to the scrollEvent
	 *
	 * @tparam T Class of member function
	 * @param object Pointer to the Object
	 * @param func The function getting added as a listener
	 */
	template<typename T>
	void addScrollEvent(T* object, void(T::* func)(glm::vec2));

private:
	/**
	 * @brief Private Constructor for singleton
	 */
	InputManager();

	/**
	 * @brief Event called when based on mouse position and delta
	 */
	Event<glm::vec2, glm::vec2> m_mouseMoveEvent;
	/**
	 * @brief Event gives KeyboardContext based on key
	 */
	Event<KeyboardContext> m_keyboardEvent;
	/**
	 * @brief Event gives MouseButtonContext on the mouse button
	 */
	Event<MouseButtonContext> m_mouseButtonEvent;
	/**
	 * @brief Event give a vec2 of scroll delta
	 */
	Event<glm::vec2> m_scrollEvent;

	//mouse movement variables
	/**
	 * @brief mouse variable for if it's the first time the mouse moved
	 */
	bool m_firstMouse = true;

	/**
	 * @brief last position of mouse
	 */
	float m_lastX, m_lastY;
};

template<typename T>
void InputManager::addToMouseMoveEvent(T *object, void(T::*func)(glm::vec2, glm::vec2)) {
	m_mouseMoveEvent.add(object, func);
}

template<typename T>
void InputManager::addToKeyboardEvent(T *object, void(T::*func)(KeyboardContext)) {
	m_keyboardEvent.add(object, func);
}

template<typename T>
void InputManager::addToMouseButtonEvent(T *object, void(T::*func)(MouseButtonContext)) {
	m_mouseButtonEvent.add(object, func);
}

template<typename T>
void InputManager::addScrollEvent(T *object, void(T::*func)(glm::vec2)) {
	m_scrollEvent.add(object, func);
}
