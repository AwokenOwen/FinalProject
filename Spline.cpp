//
// Created by awokenowen on 4/17/26.
//

#include "Spline.h"

#include "Camera.h"
#include "GameManager.h"
#include "Material.h"
#include "Object.h"
#include "Scene.h"
#include "WindowManager.h"
#include "glad/glad.h"

Spline::Spline(Object *parent) : Component(parent) {
    m_material = new Material("assets/Shaders/line.vert", "assets/Shaders/line.frag");
    m_material->setMaterialType(CUSTOM);
    glLineWidth(5.0f);
}

void Spline::loadPoints(std::vector<glm::vec3> points, int segments) {
    m_points.clear();
    m_controlPoints.clear();
    m_controlPoints = std::move(points);
    m_segments = segments;

    for (int i = 1; i < (m_loop ? m_controlPoints.size() + 1 : m_controlPoints.size() - 2); i++) {
        for (int j = 0; j < segments; j++) {
            float time = static_cast<float>(j)/segments;
            vec4 u = vec4(pow(time, 3), pow(time, 2), time, 1.0f);

            mat4 m = {
                -1, 3, -3, 1,
                2, -5, 4, -1,
                -1, 0, 1, 0,
                0, 2, 0, 0
            };
            m /= 2;
            m = glm::transpose(m);

            vec3 c1 = m_controlPoints[(i - 1) % m_controlPoints.size()];
            vec3 c2 = m_controlPoints[i % m_controlPoints.size()];
            vec3 c3 = m_controlPoints[(i + 1) % m_controlPoints.size()];
            vec3 c4 = m_controlPoints[(i + 2) % m_controlPoints.size()];

            float p_x = dot(m * vec4(c1.x, c2.x, c3.x, c4.x), u);
            float p_y = dot(m * vec4(c1.y, c2.y, c3.y, c4.y), u);
            float p_z = dot(m * vec4(c1.z, c2.z, c3.z, c4.z), u);

            m_points.emplace_back(p_x, p_y, p_z);
        }
    }
    setup();
}

void Spline::update() {
    if (m_doDraw) {
        draw();
    }
    Component::update();
}

void Spline::draw() {
    glUseProgram(m_material->getShaderProgram());

    m_material->setUniform<mat4>("model", getParent()->getModelMatrix());
    m_material->setUniform<mat4>("view", Game.getActiveScene()->getCamera()->getViewMatrix());
    m_material->setUniform<mat4>("projection", Window.getPerspectiveMatrix());

    glBindVertexArray(VAO);
    glDrawArrays(m_loop ? GL_LINE_LOOP : GL_LINE_STRIP, 0, m_points.size());
    glBindVertexArray(0);
}

void Spline::setDraw(bool draw) {
    m_doDraw = draw;
}

void Spline::setLoop(bool loop) {
    m_loop = loop;
    loadPoints(m_controlPoints, m_segments);
}

glm::vec3 Spline::getPoint(int index) {
    if (index < m_points.size())
        return glm::vec3(getParent()->getModelMatrix() * glm::vec4(m_points[index], 1.0));
    return glm::vec3(0.0f, 0.0f, 0.0f);
}

int Spline::getPointCount() {
    return m_points.size();
}

void Spline::setup() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    // 2. copy our vertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, m_points.size() * sizeof(float) * 3, &m_points[0], GL_STATIC_DRAW);
    // 3. then set our vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}
