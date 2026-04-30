//
// Created by awokenowen on 4/19/26.
//

#pragma once
#include "Component.h"
#include "glm.hpp"


class BallLauncher : public Component{
public:
    BallLauncher(Object* parent);

    void update() override;

    void launch(glm::vec3 dir, float power);

private:
    glm::vec3 velocity{};
    float gravity{0.981f};

    bool m_move{};
};
