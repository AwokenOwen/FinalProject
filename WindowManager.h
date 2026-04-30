/*
 * Written by: AwokenOwen
 * Last Updated: March 26th 2026
 */

#pragma once
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include "glm.hpp"
#include "iostream"

using namespace glm;
using namespace std;

/**
 * @brief Window Manager singleton macro
 */
#define Window WindowManager::getInstance()

class Object;
/**
 * @brief Manager in charge of creating, destroying, and maintaining the window
 */
class WindowManager
{
public:
	/**
	 * Initialize the Window manager and call the create window function. Return 0 if successful and -1 if unsuccessful
	 *
	 * @return int
	 */
	int initialize();

	/**
	 * @brief free any data on the heap
	 */
	static void terminate();

	/**
	 * @brief Getter for singleton
	 *
	 * @return WindowManager
	 */
	static WindowManager& getInstance();

	/**
	 * G@brief Getter for the GLFW window
	 *
	 * @return GLFW window*
	 */
	GLFWwindow* getWindow();

	/**
	 * @brief Getter for the width in pixels of the current window
	 *
	 * @return int
	 */
	static int getWindowWidth();
	/**
	 * @brief Getter for the height in pixels of the current window
	 *
	 * @return int
	 */
	static int getWindowHeight();

	/**
 * @brief Getter for the width in pixels of the current window
 *
 * @return int
 */
	static int getViewportWidth();
	/**
	 * @brief Getter for the height in pixels of the current window
	 *
	 * @return int
	 */
	static int getViewportHeight();

	/**
	 * @brief Clear the color and depth buffers
	 */
	static void clear();

	/**
	 * @brief Swap buffers and poll events
	 */
	void swap() const;

	/**
	 * @brief Function to set the window as maximized or not.
	 *
	 * @param maximized The new bool for whether to maximize the window
	 */
	void setMaximized(bool maximized = true) const;

	/**
	 * @brief Calculate and returns the perspective projection matrix
	 *
	 * @return mat4
	 */
	static mat4 getPerspectiveMatrix();

	/**
	 * @brief Setter for whether the viewport has a fixed aspect ratio putting black bars where it's too big
	 *
	 * @param fixed The new bool for whether the viewport has a fixed aspect ratio
	 */
	static void setFixedAspect(bool fixed = true);
	/**
	 * @brief Setter for the fixed aspect ratio
	 *
	 * @param aspect The new fixed aspect ratio
	 */
	static void setAspectRatio(float aspect);

	void activateFrameBuffer();
	void deactivateFrameBuffer();
	unsigned int getTextureColorbuffer() const;
	void deleteFrameBuffer();
	void createFrameBuffer();
	void setFrameBufferObject(Object* object);
	void sendFrameBufferTexture();

private:
	/**
	 * @brief Private constructor for singleton functionality
	 */
	WindowManager();

	/**
	 * creates the window by grabbing the primary monitor then initializing glad, then finally creating the window. Return 0 if successful -1 if unsuccessful
	 *
	 * @return int
	 */
	int createWindow();

	unsigned int framebuffer;
	unsigned int textureColorbuffer;
	unsigned int rbo;

	/**
	 * @brief Variable that holds the GLFW window
	 */
	GLFWwindow* p_window{};

	Object* frameBufferObject{};
};