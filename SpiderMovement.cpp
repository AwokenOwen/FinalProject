//
// Created by awokenowen on 4/19/26.
//

#include "SpiderMovement.h"

#include <iostream>
#include <ostream>

#include "GameManager.h"
#include "Object.h"
#include "Spline.h"

SpiderMovement::SpiderMovement(Object *parent) : Component(parent){
    m_startTime = Game.getTime();
    m_loopTime = 20.0f;
}

void SpiderMovement::start() {

    Component::start();
}

void SpiderMovement::update() {

    if (m_movementSpline != nullptr) {
        int pointCount = m_movementSpline->getPointCount();
        float timePerSegment = m_loopTime / static_cast<float>(pointCount);

        float currentLoopTime = std::fmod((Game.getTime() - m_startTime) / 0.1f, m_loopTime);

        int segment = static_cast<int>(std::floor(currentLoopTime / timePerSegment));
        glm::vec3 curvePoints1 = m_movementSpline->getPoint(segment);
        glm::vec3 curvePoints2 = m_movementSpline->getPoint((segment + 1) % pointCount);

        float t = (fmod(currentLoopTime, timePerSegment) / timePerSegment);

        glm::vec3 position = lerp(curvePoints1, curvePoints2, t);

        getParent()->setLocalPosition(position);
    }

    Component::update();
}

void SpiderMovement::setMovementSpline(Spline *spline) {
    m_movementSpline = spline;
}

glm::vec3 SpiderMovement::lerp(glm::vec3 a, glm::vec3 b, float t) {
    return (1-t)*a + t*b;
}
