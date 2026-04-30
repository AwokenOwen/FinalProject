//
// Created by awokenowen on 4/19/26.
//

#pragma once
#include <vector>

#include "Component.h"

class Spline;
class SpiderManager : public Component{
public:
    SpiderManager(Object* parent);
    void loadSpiders(int spiders);
    void addBall(Object* ball);
private:
    std::vector<Spline*> m_spidersSplines;
    std::vector<Object*> m_spidersObjects;
    std::vector<Object*> m_balls;
};
