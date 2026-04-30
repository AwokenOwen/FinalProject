//
// Created by awokenowen on 4/19/26.
//

#pragma once
#include <vector>

#include "Component.h"
#include "vec3.hpp"

class Spline;
class SpiderMovement : public Component {
public:
    SpiderMovement(Object* parent);

    void start() override;
    void update() override;

    void setMovementSpline(Spline* spline);

    std::vector<Object*> balls{};
private:
    Spline* m_movementSpline{};

    float m_startTime{};
    float m_loopTime{};

    glm::vec3 lerp(glm::vec3 a, glm::vec3 b, float t);
};
