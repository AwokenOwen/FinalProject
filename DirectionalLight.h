/*
 * Written by: AwokenOwen
 * Last Updated: March 12th 2026
 */

#pragma once
#include "glm.hpp"
#include "Object.h"

/**
 * @brief Object to configure directional light data in Scene
 */
class DirectionalLight : public Object
{
public:
	/**
	 * @brief Directional Light Constructor
	 *
	 * @param color The color of the directional light in RGB
	 * @param power The power of the directional light
	 */
	explicit DirectionalLight(vec3 color = vec3(1.0f, 1.0f, 1.0f), float power = 1.0f);

	/**
	 * @brief Gets the direction the directional light is pointing
	 *
	 * @return vec3
	 */
	vec3 getDirection();
	/**
	 * @brief Gets the power of the directional light
	 *
	 * @return float
	 */
	[[nodiscard]] float getPower() const;
	/**
	 * @brief Gets the color of the directional light
	 *
	 * @return vec3
	 */
	[[nodiscard]] vec3 getColor() const;

	/**
	 * @brief Sets the direction of the directional light
	 *
	 * @param direction The new direction of the directional light
	 */
	void setDirection(vec3 direction);
	/**
	 * @brief Sets the power of the directional light
	 *
	 * @param power The new power of the directional light
	 */
	void setPower(float power);
	/**
	 * @brief Sets the color of the directional light
	 *
	 * @param color The new color of tge directional light
	 */
	void setColor(vec3 color);

private:
	/**
	 * @brief Power of the directional light
	 */
	float m_power;
	/**
	 * @brief Color of the directional light
	 */
	vec3 m_color{};
};

