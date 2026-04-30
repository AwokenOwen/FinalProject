//
// Created by awokenowen on 4/19/26.
//

#include "SpiderManager.h"

#include <cstdlib>
#include <ctime>

#include "MeshRenderer.h"
#include "Object.h"
#include "SpiderMovement.h"
#include "Spline.h"

SpiderManager::SpiderManager(Object *parent) : Component(parent) {
    std::srand(std::time(nullptr));
}

void SpiderManager::loadSpiders(int spiders) {
    for (int i = 0; i < spiders; i++) {
        std::vector<glm::vec3> controlPoints;
        for (int j = 0; j < 4; j++) {
            // Seed with current time to get different results each run
            int min = -90, max = 90;
            // Formula: rand() % (range_size) + offset
            int x = std::rand() % (max - min + 1) + min;
            int y = std::rand() % (max - min + 1) + min;

            controlPoints.emplace_back(static_cast<float>(x) / 100.0f, static_cast<float>(y) / 100.0f, 0.0f);
        }
        auto spiderSpline = new Object();
        getParent()->addChild(spiderSpline);
        spiderSpline->addComponent<Spline>()->loadPoints(controlPoints, 20);
        spiderSpline->setLocalPosition(glm::vec3(0.0f, 0.0f, -1.0f));
        spiderSpline->getComponent<Spline>()->setDraw();
        spiderSpline->getComponent<Spline>()->setLoop();

        auto spider = new Object();
        spider->addComponent<MeshRenderer>()->loadModel("assets/defaultAssets/Models/sphere.fbx");
        spider->addComponent<SpiderMovement>()->setMovementSpline(spiderSpline->getComponent<Spline>());
    }
}

void SpiderManager::addBall(Object *ball) {
    m_balls.push_back(ball);
}
