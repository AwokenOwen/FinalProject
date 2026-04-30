//
// Created by awokenowen on 4/17/26.
//

#pragma once
#include <vector>

#include "Component.h"
#include "glm.hpp"

class Material;
class Object;
class Spline : public Component{
public:
    Spline(Object *parent);
    void loadPoints(std::vector<glm::vec3> points, int segments);

    void update() override;

    void draw();

    void setDraw(bool draw = true);
    void setLoop(bool loop = true);

    glm::vec3 getPoint(int index);
    int getPointCount();

private:
    bool m_doDraw = false;
    bool m_loop = false;
    int m_segments{};

    std::vector<glm::vec3> m_controlPoints;
    std::vector<glm::vec3> m_points{};

    unsigned int VAO{}, VBO{};

    void setup();

    Material* m_material{};
};
