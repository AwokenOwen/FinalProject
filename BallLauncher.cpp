//
// Created by awokenowen on 4/19/26.
//

#include "BallLauncher.h"

#include "Camera.h"
#include "GameManager.h"
#include "Object.h"
#include "Scene.h"

BallLauncher::BallLauncher(Object *parent) : Component(parent){

}

void BallLauncher::update() {

    if (m_move) {
        velocity.y -= gravity * Game.getDeltaTime();

        getParent()->setLocalPosition(getParent()->getLocalPosition() + (velocity * static_cast<float>(Game.getDeltaTime())));

        if (getParent()->getLocalPosition().y < 0) {
            velocity.y = -1 * velocity.y;
            vec3 pos = getParent()->getLocalPosition();
            pos.y = 0;
            getParent()->setLocalPosition(pos);
        }

    }
    Component::update();
}

void BallLauncher::launch(vec3 dir, float power) {
    getParent()->setLocalPosition(getParent()->getWorldPosition());
    getParent()->setParent(nullptr);
    velocity = dir * -power;
    velocity.x *= -1;
    m_move = true;
}
