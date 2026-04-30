/*
 * Written by: AwokenOwen
 * Last Updated: March 28th 2026
 */

#pragma once
#include "Component.h"
#include <glm.hpp>
#include <map>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <vector>
#include <string>

/**
 * @brief Struct for using freetype and displaying chars to the screen
 */
struct Character {
    /**
     * @brief OpenGL texture of the character
     */
    unsigned int TextureID;
    /**
     * @brief Size of glyph
     */
    glm::ivec2   Size;
    /**
     * @brief Offset from baseline to left/top of glyph
     */
    glm::ivec2   Bearing;
    /**
    * Offset to advance to next glyph
     */
    long Advance;
};

/**
 * @brief Forward Declaration of the Mesh Class
 */
class Mesh;
/**
 * @brief Forward Declaration of the Material Class
 */
class Material;
/**
 * @brief Text Renderer Component used to display text onto the screen
 */
class TextRenderer : public Component{
public:
    /**
     * @brief Constructor for the Text Renderer
     *
     * @param parent
     */
    explicit TextRenderer(Object* parent);
    /**
     * @brief Setter for the font of the text returns -1 if failed
     *
     * @param path Path to the font being used
     * @return int
     */
    int setFont(const char* path);
    /**
     * @brief Component override of the update function, runs every frame
     */
    void update() override;
    /**
     * @brief Setter for the text to be displayed
     *
     * @param text The new text to be displayed
     */
    void setText(const std::string& text);
    /**
     * @brief Setter for the color of the text
     *
     * @param color The new color of the text
     */
    void setColor(glm::vec3 color);

    Material* getMaterial();
private:
    /**
     * @brief Map of chars to C++ struct characters used to print to the screen
     */
    std::map<char, Character> m_characters;
    /**
     * @brief Freetype library
     */
    FT_Library m_ft{};
    /**
     * @brief Freetype face
     */
    FT_Face m_face{};

    /**
     * @brief Material for all the text objects
     */
    Material* m_material{};

    /**
     * @brief Vector of the meshes for each letter
     */
    std::vector<Mesh*> m_meshes{};
    /**
     * @brief Vector of characters in the text to display on screen
     */
    std::vector<Character> m_text{};
    /**
     * @brief Color of the text
     */
    glm::vec3 m_color{};
};