//
// Created by awokenowen on 3/29/26.
//

#include "MovementComponent.h"

#include <iostream>
#include <ostream>

#include "Object.h"
#include "GameManager.h"

MovementComponent::MovementComponent(Object *parent) : Component(parent){
    Input.addToKeyboardEvent(this, &MovementComponent::handleKeyboard);
    Input.addToMouseMoveEvent(this, &MovementComponent::handleMouse);
    phi = glm::pi<float>() / 2.0f;
    theta = glm::pi<float>() / 2.0f;
}

void MovementComponent::handleKeyboard(KeyboardContext context) {
    if (context.getAction() == GLFW_REPEAT)
        return;

    if (context.getKey() == GLFW_KEY_W) {
        if (context.getAction() == GLFW_PRESS) {
            movementInput.y += 1.0f;
        }else {
            movementInput.y -= 1.0f;
        }
    }

    if (context.getKey() == GLFW_KEY_S) {
        if (context.getAction() == GLFW_PRESS) {
            movementInput.y -= 1.0f;
        }else {
            movementInput.y += 1.0f;
        }
    }

    if (context.getKey() == GLFW_KEY_A) {
        if (context.getAction() == GLFW_PRESS) {
            movementInput.x += 1.0f;
        }else {
            movementInput.x -= 1.0f;
        }
    }

    if (context.getKey() == GLFW_KEY_D) {
        if (context.getAction() == GLFW_PRESS) {
            movementInput.x -= 1.0f;
        }else {
            movementInput.x += 1.0f;
        }
    }
}

void MovementComponent::handleMouse(glm::vec2 pos, glm::vec2 delta) {
    phi += delta.y * 0.005f;
    phi = clamp(phi, 0.1f, glm::pi<float>() - 0.1f);

    theta += delta.x * 0.005f;
    theta = mod(theta, 2.0f * glm::pi<float>());
}

void MovementComponent::update() {

    vec3 forward = vec3
    (
        (float)sin(phi) * (float)cos(theta),
        (float)cos(phi),
        (float)sin(phi) * (float)sin(theta)
    );

    forward = normalize(forward);

    vec3 right = getParent()->getRight();

    vec3 moveDirection = forward * movementInput.y + right * movementInput.x;
    moveDirection.y = 0;
    if (length(moveDirection) > 0.0f) {
        moveDirection = normalize(moveDirection);
    }

    moveDirection *= 3.0f * Game.getDeltaTime();

    if (m_move) {
        getParent()->setLocalPosition(getParent()->getLocalPosition() + moveDirection);
        getParent()->setLocalRotation(qua(lookAt(getParent()->getWorldPosition(), getParent()->getWorldPosition() - forward, vec3(0,1,0))));
    }else {
        getParent()->setLocalRotation(qua(lookAt(getParent()->getWorldPosition(), getParent()->getWorldPosition() - vec3(0,0,1), vec3(0,1,0))));
    }

    Component::update();
}

void MovementComponent::setMove(bool move) {
    m_move = move;
}
