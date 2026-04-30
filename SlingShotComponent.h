//
// Created by awokenowen on 4/19/26.
//

#pragma once
#include "Component.h"
#include "InputManager.h"

class SlingShotComponent : public Component{
public:
    SlingShotComponent(Object* parent);

    void update() override;

private:
    float m_useDistance{};
    bool m_interacting{};
    float m_distance{};
    Object* m_player;

    float m_power{};
    bool pressing{};

    void handleKeyboard(KeyboardContext context);
    void handleMouseButton(MouseButtonContext context);
    void handleMouse(glm::vec2 pos, glm::vec2 delta);

    void reload();

    float theta{};
    float phi{};

    Object* powerMeter{};
    Object* ball;
};
