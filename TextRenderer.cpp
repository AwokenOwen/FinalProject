#include "TextRenderer.h"
#include "Material.h"
#include "Mesh.h"
#include "Object.h"
#include <iostream>
#include <bits/fs_fwd.h>

#include "glad/glad.h"

TextRenderer::TextRenderer(Object* parent) : Component(parent){
    setFont("assets/defaultAssets/Fonts/arial.ttf");

    m_material = new Material("assets/defaultAssets/Shaders/defaultUI.vert", "assets/defaultAssets/Shaders/text.frag");
    m_material->setMaterialType(UI);
    m_material->setParent(getParent());
    m_material->setTransparent(true);
    m_material->addTexture(static_cast<unsigned int>(0));
    m_color = vec3(1, 1, 1);
    m_material->setScreenAnchorPoint(TOP_RIGHT);
    m_material->setTextAnchorPoint(TOP_RIGHT);
    setColor(vec3(0.0));
}

int TextRenderer::setFont(const char *path) {
    if (FT_Init_FreeType(&m_ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    }

    if (FT_New_Face(m_ft, path, 0, &m_face))
    {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        return -1;
    }

    FT_Set_Pixel_Sizes(m_face, 0, 48);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

    for (unsigned char c = 0; c < 128; c++)
    {
        // load character glyph
        if (FT_Load_Char(m_face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        // generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            m_face->glyph->bitmap.width,
            m_face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            m_face->glyph->bitmap.buffer
        );
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // now store character for later use
        Character character = {
            texture,
            glm::ivec2(m_face->glyph->bitmap.width, m_face->glyph->bitmap.rows),
            glm::ivec2(m_face->glyph->bitmap_left, m_face->glyph->bitmap_top),
            m_face->glyph->advance.x
        };
        m_characters.insert(std::pair<char, Character>(c, character));
    }
    FT_Done_Face(m_face);
    FT_Done_FreeType(m_ft);
    return 0;
}

void TextRenderer::update() {

    m_material->setUniform<vec3>("color", m_color);

    for (int i = 0; i < m_meshes.size(); ++i) {
        m_material->setTexture(m_text[i].TextureID, 0);

        m_meshes[i]->draw();
    }

    Component::update();
}

void TextRenderer::setText(const std::string& text) {
    float x = 0, y = 0;

    float maxY = m_characters.at(text[0]).Size.y;
    float fullX = 0;
    for (int i = 0; i < text.size(); ++i) {
        const Character character = m_characters.at(text[i]);
        fullX += character.Advance >> 6;
    }

    switch (m_material->getTextAnchorPoint()) {
        case CENTER:
            x = -(fullX / 2.0f);
            y = -(maxY / 2.0f);
            break;
        case LEFT:
            y = -(maxY / 2.0f);
            break;
        case RIGHT:
            x = -fullX;
            y = -(maxY / 2.0f);
            break;
        case TOP:
            x = -(fullX / 2.0f);
            y = -maxY;
            break;
        case BOTTOM:
            x = -(fullX / 2.0f);
            break;
        case TOP_LEFT:
            y = -maxY;
            break;
        case TOP_RIGHT:
            x = -fullX;
            y = -maxY;
            break;
        case BOTTOM_LEFT:
            break;
        case BOTTOM_RIGHT:
            x = fullX;
            break;
        default:
            break;
    }

    m_text.clear();
    m_meshes.clear();
    for (int i = 0; i < text.size(); ++i) {
        Character character = m_characters.at(text[i]);
        float xpos = x + character.Bearing.x * 1;
        float ypos = y - (character.Size.y - character.Bearing.y) * 1;

        float w = character.Size.x * 1;
        float h = character.Size.y * 1;

        vector<Mesh::Vertex> vertices {
            Mesh::Vertex(vec3(xpos, ypos, 0), vec3(0, 0, 1), vec2(0, 1)),
            Mesh::Vertex(vec3(xpos + w, ypos, 0), vec3(0, 0, 1), vec2(1, 1)),
            Mesh::Vertex(vec3(xpos + w, ypos + h, 0), vec3(0, 0, 1), vec2(1, 0)),
            Mesh::Vertex(vec3(xpos, ypos + h, 0), vec3(0, 0, 1), vec2(0, 0)),
        };
        vector<unsigned int> indices {
            0, 1, 2,
            0, 2, 3,
        };
        auto m = new Mesh(vertices, indices);
        m->setMaterial(m_material);
        m->setParent(this);
        m_meshes.push_back(m);
        m_text.push_back(character);

        x += (character.Advance >> 6);
    }
}

void TextRenderer::setColor(const glm::vec3 color) {
    m_color = color;
}

Material * TextRenderer::getMaterial() {
    return m_material;
}
