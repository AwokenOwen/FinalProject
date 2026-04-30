#include "Material.h"

#include <filesystem>

#include "ResourceManager.h"
#include "GameManager.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "Object.h"
#include "Scene.h"


Material::Material()
{
    setShaderProgram("assets/defaultAssets/Shaders/default.vert", "assets/defaultAssets/Shaders/default.frag");
    m_type = MaterialType::DEFAULT_LIT;
    m_screenAnchorPoint = UIAnchorPoints::CENTER;
    m_twoSided = false;
}

Material::Material(const char* vertexShaderPath, const char* fragmentShaderPath)
{
    setShaderProgram(vertexShaderPath, fragmentShaderPath);
    m_type = MaterialType::DEFAULT_LIT;
}

void Material::setParent(Object *parent) {
    p_parent = parent;
}

unsigned int Material::getShaderProgram() const {
    return m_shaderProgram;
}

void Material::addTexture(const char* path)
{
    m_textures.push_back(Resource.loadImage(path));
}

void Material::addTexture(const unsigned int texture) {
    m_textures.push_back(texture);
}

void Material::setTexture(const char* path, const int index)
{
    if (index >= m_textures.size()) {
        Resource.loadImage(path);
    }
    m_textures[index] = Resource.loadImage(path);
}

void Material::setTexture(const unsigned int texture, const int index) {
    if (index >= m_textures.size()) {
        addTexture(texture);
    }
    m_textures[index] = texture;
}

void Material::setSkyboxTexture(const vector<const char*> &paths)
{
    m_skyboxTexture = Resource.loadCubeMap(paths);
}

void Material::setSkyboxTexture(unsigned int cubeMap) {
    m_skyboxTexture = cubeMap;
}

void Material::setMaterialType(const MaterialType materialType)
{
    m_type = materialType;
}

MaterialType Material::getMaterialType() const {
    return m_type;
}

void Material::setScreenAnchorPoint(const UIAnchorPoints anchorPoint)
{
    m_screenAnchorPoint = anchorPoint;
}

UIAnchorPoints Material::getScreenAnchorPoint() const {
    return m_screenAnchorPoint;
}

void Material::setTextAnchorPoint(const UIAnchorPoints anchorPoint) {
    m_textureAnchorPoint = anchorPoint;
}

UIAnchorPoints Material::getTextAnchorPoint() const {
    return m_textureAnchorPoint;
}

void Material::setTwoSided(const bool twoSided)
{
    m_twoSided = twoSided;
}

bool Material::getTwoSided() const {
    return m_twoSided;
}

void Material::setTransparent(const bool transparent) {
    p_parent->setTransparent(transparent);
    m_transparent = transparent;
    Game.getActiveScene()->refreshTransparency();
}

bool Material::getTransparent() const {
    return m_transparent;
}

void Material::loadTextures()
{
    for (int i = 0; i < m_textures.size(); i++)
    {
        setUniform<int>(string("textures[" + to_string(i) + "]"), i);

        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, m_textures[i]);
    }

    glActiveTexture(GL_TEXTURE0 + m_textures.size());
    setUniform<int>("skybox", + m_textures.size());
    glBindTexture(GL_TEXTURE_CUBE_MAP, Game.getActiveScene()->getSkyboxTexture());

    glActiveTexture(GL_TEXTURE0 + m_textures.size() + 1);
    setUniform<int>("irradianceMap", + m_textures.size() + 1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, Game.getActiveScene()->getIrradianceMapTexture());

    glActiveTexture(GL_TEXTURE0 + m_textures.size() + 2);
    setUniform<int>("prefilteredMap", + m_textures.size() + 2);
    glBindTexture(GL_TEXTURE_CUBE_MAP, Game.getActiveScene()->getPrefilterTexture());

    glActiveTexture(GL_TEXTURE0 + m_textures.size() + 3);
    setUniform<int>("BRDFMap", + m_textures.size() + 3);
    glBindTexture(GL_TEXTURE_2D, Game.getActiveScene()->getBrdfTexture());
}

void Material::setShaderProgram(const char* vertexShaderPath, const char* fragmentShaderPath)
{
    auto path = std::string(vertexShaderPath);
    path += fragmentShaderPath;
    unsigned int result = Resource.getShaderProgramFromMap(path);
    if (result != -1)
    {
        m_shaderProgram = result;
        return;
    }

    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vShaderFile.open(vertexShaderPath);
        fShaderFile.open(fragmentShaderPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
    }
    const char* vertexShaderSource = vertexCode.c_str();
    const char* fragmentShaderSource = fragmentCode.c_str();

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    m_shaderProgram = glCreateProgram();

    glAttachShader(m_shaderProgram, vertexShader);
    glAttachShader(m_shaderProgram, fragmentShader);
    glLinkProgram(m_shaderProgram);

    glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(m_shaderProgram, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    Resource.addShaderProgramToMap(path, m_shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}
