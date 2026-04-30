//
// Created by awokenowen on 4/19/26.
//

#include "SlingShotComponent.h"

#include <iostream>
#include <ostream>
#include "Camera.h"
#include "GameManager.h"
#include "Object.h"
#include "Scene.h"
#include <gtc/quaternion.hpp>

#include "BallLauncher.h"
#include "MeshRenderer.h"
#include "MovementComponent.h"
#include "TextRenderer.h"

SlingShotComponent::SlingShotComponent(Object *parent) : Component(parent){
    m_player = dynamic_cast<Object *>(Game.getActiveScene()->getCamera());
    m_useDistance = 10.0f;
    Input.addToKeyboardEvent(this, &SlingShotComponent::handleKeyboard);
    Input.addToMouseButtonEvent(this, &SlingShotComponent::handleMouseButton);
    Input.addToMouseMoveEvent(this, &SlingShotComponent::handleMouse);
    powerMeter = new Object();
    powerMeter->addComponent<TextRenderer>()->setText("0");
    ball = nullptr;
    phi = glm::pi<float>() / 2.0f;
    theta = glm::pi<float>() / 2.0f;
}

void SlingShotComponent::update() {
    if (pressing) {
        m_power += Game.getDeltaTime();
    }
    powerMeter->getComponent<TextRenderer>()->setText(to_string(m_power));

    auto forward = vec3
    (
        (float)sin(phi) * (float)cos(theta),
        (float)cos(phi),
        (float)sin(phi) * (float)sin(theta)
    );

    forward = normalize(forward);

    getParent()->setLocalRotation(qua(lookAt(getParent()->getWorldPosition(), getParent()->getWorldPosition() + forward, vec3(0,1,0))));

    Component::update();
}

void SlingShotComponent::handleKeyboard(KeyboardContext context) {
    if (context.getKey() == GLFW_KEY_SPACE && context.getAction() == GLFW_PRESS && m_distance < m_useDistance) {
        m_interacting = !m_interacting;
        m_player->getComponent<MovementComponent>()->setMove(!m_interacting);
        m_player->setLocalPosition(getParent()->getWorldPosition() + vec3(0,2,-3));
        if (m_interacting && ball == nullptr) {
            reload();
        }
    }

    if (context.getKey() == GLFW_KEY_R && context.getAction() == GLFW_PRESS && m_interacting && ball == nullptr) {
        reload();
    }
}

void SlingShotComponent::handleMouseButton(MouseButtonContext context) {
    if (context.getButton() == GLFW_MOUSE_BUTTON_LEFT && context.getAction() != GLFW_RELEASE && ball != nullptr) {
        pressing = true;
    }
    if (context.getButton() == GLFW_MOUSE_BUTTON_LEFT && context.getAction() == GLFW_RELEASE && ball != nullptr) {
        //launch
        ball->getComponent<BallLauncher>()->launch(getParent()->getForward(), m_power);
        m_power = 0;
        pressing = false;
        ball = nullptr;
    }
}

void SlingShotComponent::handleMouse(glm::vec2 pos, glm::vec2 delta) {
    if (m_interacting) {
        phi -= delta.y * 0.005f;
        phi = glm::clamp(phi, 0.1f, glm::pi<float>() - 0.1f);

        theta += delta.x * 0.005f;
        theta = mod(theta, 2.0f * glm::pi<float>());
    }
}

void SlingShotComponent::reload() {
    ball = new Object();
    ball->addComponent<MeshRenderer>()->loadModel("assets/defaultAssets/Models/sphere.fbx");
    ball->setLocalScale(vec3(0.1f));
    getParent()->addChild(ball);
    ball->setLocalPosition(vec3(0, 1.2, 0));
    ball->addComponent<BallLauncher>();
}
