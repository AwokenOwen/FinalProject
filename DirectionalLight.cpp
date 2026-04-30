#include "DirectionalLight.h"

#include "GameManager.h"
#include "Scene.h"

DirectionalLight::DirectionalLight(const vec3 color, const float power) : Object()
{
	this->m_power = power;
	this->m_color = color;

	setDirection(vec3(0.5, -1.0, 0.5));

	Game.getActiveScene()->setDirectionalLight(this);
}

vec3 DirectionalLight::getDirection()
{
	return normalize(getForward());
}

float DirectionalLight::getPower() const
{
	return m_power;
}

vec3 DirectionalLight::getColor() const {
	return m_color;
}

void DirectionalLight::setDirection(const vec3 direction)
{
	// maybe flip eye and center
	setLocalRotation(quat(lookAt(normalize(direction), vec3(0.0), vec3(0.0, 1.0, 0.0))));
}

void DirectionalLight::setPower(const float power)
{
	this->m_power = power;
}

void DirectionalLight::setColor(const vec3 color)
{
	this->m_color = color;
}

