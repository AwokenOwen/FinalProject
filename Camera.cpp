/*
 * Written by: AwokenOwen
 * Last Updated: March 12th 2026
 */

#include "Camera.h"

Camera::Camera() : Object()
{
	
}

mat4 Camera::getViewMatrix() {
	// Calculate Matrix using lookAt function where the camera is looking with it's forward vector
	return lookAt(getWorldPosition(), getWorldPosition() + m_forward, m_up);
}
