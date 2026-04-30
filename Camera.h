/*
 * Written by: AwokenOwen
 * Last Updated: March 25th 2026
 */

#pragma once
#include "Object.h"

/**
 * @brief Object used to determine View Matrix used in the Shaders of all Rendered Objects
 */
class Camera : public Object
{
public:
	/**
	 * @brief Default Constructor
	 */
	Camera();

	/**
	 * @brief Returns the View Matrix for the Objects Rendering in the Scene based off of the Position and Rotation of the Camera
	 *
	 * @return mat4
	 */
	mat4 getViewMatrix();
};

