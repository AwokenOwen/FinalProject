#include "Mesh.h"
#include "Material.h"
#include "glad/glad.h"
#include "WindowManager.h"
#include "Camera.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include <gtc/type_ptr.hpp>
#include "MeshRenderer.h"
#include "GameManager.h"
#include "Scene.h"

Mesh::Mesh(const vector<Vertex> &vertices, const vector<unsigned int> &indices)
{
    m_vertices = vertices;
    m_indices = indices;

    setupMesh();
}

void Mesh::setupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Mesh::Vertex), &m_vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int),
        &m_indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), static_cast<void *>(nullptr));
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), reinterpret_cast<void *>(offsetof(Mesh::Vertex, m_normal)));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), reinterpret_cast<void *>(offsetof(Mesh::Vertex, m_texCoords)));

    glBindVertexArray(0);
}

void Mesh::setUpShaderMatrices(const unsigned int shaderProgram) const {
    // Vertex Matrix Stuff
    mat4 modelMatrix = getParent()->getParent()->getModelMatrix();
    mat4 projectionMatrix = Window.getPerspectiveMatrix();
    mat4 viewMatrix = Game.getActiveScene()->getCamera()->getViewMatrix();

    if (m_material->getMaterialType() == SKYBOX)
    {
        viewMatrix = mat4(mat3(viewMatrix));
    }

    if (m_material->getMaterialType() == GUI || m_material->getMaterialType() == UI)
    {
        auto UIPos = m_parent->getParent()->getWorldPosition();
        vec3 UIScale = m_parent->getParent()->getWorldScale();
        if (m_material->getMaterialType() == UI) {
            UIPos = vec3(UIPos.x / Window.getWindowWidth(), UIPos.y / Window.getWindowHeight(), UIPos.z);
            UIScale = vec3(UIScale.x / Window.getWindowWidth(), UIScale.y / Window.getWindowHeight(), UIScale.z);
        }

        switch (m_material->getScreenAnchorPoint())
        {
        case TOP:
            UIPos.y += 1 - UIScale.y;
            break;
        case BOTTOM:
            UIPos.y += -(1 - UIScale.y);
            break;
        case RIGHT:
            UIPos.x += 1 - UIScale.x;
            break;
        case LEFT:
            UIPos.x += -(1 - UIScale.x);
            break;
        case TOP_RIGHT:
            UIPos.x += 1 - UIScale.x;
            UIPos.y += 1 - UIScale.y;
            break;
        case BOTTOM_RIGHT:
            UIPos.x += 1 - UIScale.x;
            UIPos.y += -(1 - UIScale.y);
            break;
        case TOP_LEFT:
            UIPos.y += 1 - UIScale.y;
            UIPos.x += -(1 - UIScale.x);
            break;
        case BOTTOM_LEFT:
            UIPos.y += -(1 - UIScale.y);
            UIPos.x += -(1 - UIScale.x);
            break;
        default:
            break;
        }

        UIPos += getParent()->getParent()->getWorldPosition();

        vec3 rotationVec = eulerAngles(getParent()->getParent()->getWorldRotation());
        rotationVec.z = 0;
        const auto rotation = quat(rotationVec);

        const mat4 transformMatrix = translate(mat4(1.0f), UIPos);
        const mat4 rotationMatrix = rotate(mat4(1.0f), angle(rotation), axis(rotation));
        const mat4 scaleMatrix = scale(mat4(1.0), UIScale);

        modelMatrix = transformMatrix * rotationMatrix * scaleMatrix;
    }

    m_material->setUniform<mat4>("model", modelMatrix);

    const int viewLoc = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    const int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
}

void Mesh::setUpShaderVariables(const unsigned int shaderProgram)
{
    const vec3 cameraPosition = Game.getActiveScene()->getCamera()->getWorldPosition();

    const int cameraLoc = glGetUniformLocation(shaderProgram, "camPos");
    glUniform3f(cameraLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);
}

void Mesh::setUpDirectionalLight(const unsigned int shaderProgram) const {
    // Directional Light
    if (Game.getActiveScene()->getDirectionalLight() != nullptr)
    {
        const vec3 direction = m_material->getMaterialType() == MaterialType::CUBE_MAP ?
            Game.getActiveScene()->getDirectionalLight()->getDirection() : -Game.getActiveScene()->getDirectionalLight()->getDirection();
        const float power = Game.getActiveScene()->getDirectionalLight()->getPower();
        const vec3 color = Game.getActiveScene()->getDirectionalLight()->getColor();

        const int directionLoc = glGetUniformLocation(shaderProgram, "dirLightDir");
        glUniform3f(directionLoc, direction.x, direction.y, direction.z);

        const int powerLoc = glGetUniformLocation(shaderProgram, "dirLightPow");
        glUniform1f(powerLoc, power);

        const int colorLoc = glGetUniformLocation(shaderProgram, "dirLightColor");
        glUniform3f(colorLoc, color.x, color.y, color.z);
    }
}

void Mesh::setUpPointLights(const unsigned int shaderProgram)
{
    for (int i = 0; i < Game.getActiveScene()->getPointLights().size(); i++)
    {
        // Point Lights
        const vec3 position = Game.getActiveScene()->getPointLights()[i]->getWorldPosition();
        const float power = Game.getActiveScene()->getPointLights()[i]->getPower();
        const vec3 color = Game.getActiveScene()->getPointLights()[i]->getColor();

        string command = string("lightPositions[") + to_string(i) + "]";
        const int positionLoc = glGetUniformLocation(shaderProgram, command.c_str());
        glUniform3f(positionLoc, position.x, position.y, position.z);

        command = string("lightPowers[") + to_string(i) + "]";
        const int powerLoc = glGetUniformLocation(shaderProgram, command.c_str());
        glUniform1f(powerLoc, power);

        command = string("lightColors[") + to_string(i) + "]";
        const int colorLoc = glGetUniformLocation(shaderProgram, command.c_str());
        glUniform3f(colorLoc, color.x, color.y, color.z);
    }
}

void Mesh::draw()
{
    // draw mesh
    glDisable(GL_CULL_FACE);
    if (m_material->getMaterialType() != CUSTOM) {
        if (!m_material->getTwoSided())
        {
            glEnable(GL_CULL_FACE);

            if (m_material->getMaterialType() == MaterialType::CUBE_MAP || m_material->getMaterialType() == MaterialType::SKYBOX)
            {
                glCullFace(GL_FRONT);
            }
            else {
                glCullFace(GL_BACK);
            }
        }
    }

    if (m_material->getTransparent()) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }else {
        glDisable(GL_BLEND);
    }
    const unsigned int shaderProgram = m_material->getShaderProgram();

    glUseProgram(shaderProgram);

    if (m_material->getMaterialType() != CUSTOM) {
        // Load Textures
        m_material->loadTextures();
        setUpShaderMatrices(shaderProgram);
        setUpShaderVariables(shaderProgram);
        setUpDirectionalLight(shaderProgram);
        setUpPointLights(shaderProgram);
    }

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indices.size()), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}


Component* Mesh::getParent() const {
    return m_parent;
}

void Mesh::setParent(Component* parent)
{
    this->m_parent = parent;
}

Material * Mesh::getMaterial() const {
    return m_material;
}

void Mesh::setMaterial(Material *material) {
    m_material = material;
}
