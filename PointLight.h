/*
 * Written by: AwokenOwen
 * Last Updated: March 25th 2026
 */

#pragma once
#include "glm.hpp"
#include "Object.h"

using namespace glm;

/**
 * @brief Class to hold point light data
 */
class PointLight : public Object
{
public:
    /**
     * @brief Constructor for Point light
     *
     * @param color Color of the light in RGB
     * @param power Power of the light
     */
    explicit PointLight(vec3 color = vec3(1.0f, 1.0f, 1.0f), float power = 1.0f);

    /**
     * @brief Getter for the color in RGB
     *
     * @return vec3
     */
    vec3 getColor() const;
    /**
     * @brief Getter for the power
     *
     * @return float
     */
    float getPower() const;

    /**
     * @brief Setter for the color in RGB
     *
     * @param color The new RGB color of the light
     */
    void setColor(vec3 color);
    /**
     * @brief Setter for the power
     *
     * @param power The new power of the light
     */
    void setPower(float power);

private:
    /**
     * @brief The color of the light in RGB
     */
    vec3 m_color{};
    /**
     * @brief The power of the light
     */
    float m_power;
};

