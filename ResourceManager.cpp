#define STB_IMAGE_IMPLEMENTATION 
#include "stb_image.h"

#include "ResourceManager.h"

#include <cstring>
#include <fstream>

#include "Mesh.h"
#include <iostream>

#include "GameManager.h"
#include "Material.h"
#include "MeshRenderer.h"
#include "Object.h"
#include "Scene.h"
#include "WindowManager.h"
#include "ext/matrix_clip_space.hpp"
#include "ext/matrix_transform.hpp"
#include "glad/glad.h"

int ResourceManager::initialize()
{
	return 0;
}

void ResourceManager::terminate()
{

}

ResourceManager& ResourceManager::getInstance()
{
	static ResourceManager single;
	return single;
}

int ResourceManager::loadScene(string name)
{
	return 0;
}

unsigned int ResourceManager::loadImage(const char* path)
{
	// If Texture already loaded grab loaded texture
	auto mapTexture = m_textureMap.find(string(path));
	if (mapTexture != m_textureMap.end())
	{
		return mapTexture->second;
	}

	//Loading Texture
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping/filtering options (on currently bound texture)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load and generate the texture
	int width, height, nrChannels;
	unsigned char* data = stbi_load(path, &width, &height,
		&nrChannels, 0);
	if (data)
	{
		switch (nrChannels) {
		case 1:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED,
				GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			break;
		case 2:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, width, height, 0, GL_RG,
				GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			break;
		case 3:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
				GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			break;
		default:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
				GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			break;
		}
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	m_textureMap.insert({string(path), texture});

	return texture;
}

unsigned int ResourceManager::loadCubeMap(const vector<const char*> &paths)
{
	if (paths.size() != 6)
	{
		cout << "Number of cube map images is incorrect... Input 6" << endl;
		return -1;
	}

	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < paths.size(); i++)
	{
		const unsigned char *data = stbi_load(paths[i], &width, &height, &nrChannels, 0);
		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
		);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


	return textureID;
}

unsigned int ResourceManager::loadHDR(const char *path) {
	stbi_set_flip_vertically_on_load(true);
	int width, height, nrComponents;
	float *data = stbi_loadf(path, &width, &height, &nrComponents, 0);
	unsigned int hdrTexture;
	if (data)
	{
		glGenTextures(1, &hdrTexture);
		glBindTexture(GL_TEXTURE_2D, hdrTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Failed to load HDR image." << std::endl;
		return -1;
	}

	//return hdrTexture;

	glGenFramebuffers(1, &captureFBO);
	glGenRenderbuffers(1, &captureRBO);

	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

	unsigned int envCubemap;
	glGenTextures(1, &envCubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);

	for (unsigned int i = 0; i < 6; ++i)
	{
		// note that we store each face with 16 bit floating point values
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F,
					 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	glm::mat4 captureViews[] =
	{
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	 };

	auto a = new Object(false);
	a->setActive(false);
	a->addComponent<MeshRenderer>()->loadModel("assets/defaultAssets/Models/cube.fbx");
	a->getComponent<MeshRenderer>()->getMaterials()[0]->setMaterialType(CUSTOM);
	a->getComponent<MeshRenderer>()->getMaterials()[0]->setShaderProgram(
		"assets/defaultAssets/Shaders/UnseenRenderingShaders/equirectangular.vert",
		"assets/defaultAssets/Shaders/UnseenRenderingShaders/equirectangular.frag"
	);

	a->getComponent<MeshRenderer>()->getMaterials()[0]->setUniform<mat4>("projection", captureProjection);

	glViewport(0, 0, 512, 512); // don't forget to configure the viewport to the capture dimensions.
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, hdrTexture);
	for (unsigned int i = 0; i < 6; ++i)
	{
		a->getComponent<MeshRenderer>()->getMaterials()[0]->setUniform<mat4>("view", captureViews[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
							   GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		a->update(); // renders a 1x1 cube
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	delete a;

	return envCubemap;
}

vector<Mesh*> ResourceManager::getMeshFromMap(string path)
{
	// If Texture already loaded grab loaded texture
	auto mapMesh = m_meshMap.find(string(path));
	if (mapMesh != m_meshMap.end())
	{
		return mapMesh->second;
	}
	return {};
}

void ResourceManager::addMeshToMap(string path, vector<Mesh*> meshes)
{
	m_meshMap.insert({ path, meshes });
}

unsigned int ResourceManager::getShaderProgramFromMap(string path)
{
	// If Texture already loaded grab loaded texture
	auto mapShader = m_shaderMap.find(string(path));
	if (mapShader != m_shaderMap.end())
	{
		return mapShader->second;
	}

	return -1;
}

void ResourceManager::addShaderProgramToMap(string path, unsigned int shaderProgram)
{
	m_shaderMap.insert({ path, shaderProgram });
}

SoundData* ResourceManager::loadSound(string path) {
	// If Texture already loaded grab loaded sound
	auto mapSound = m_soundMap.find(string(path));
	if (mapSound != m_soundMap.end())
	{
		return mapSound->second;
	}

	std::ifstream file(path, std::ios::binary);
    if (!file) {
        std::cerr << "Error: Unable to open the file" << std::endl;
    }

    char buffer[4];
    // the RIFF
    if(!file.read(buffer, 4))
    {
        std::cerr << "ERROR: could not read RIFF" << std::endl;
    }
    if(std::strncmp(buffer, "RIFF", 4) != 0)
    {
        std::cerr << "ERROR: file is not a valid WAVE file (header doesn't begin with RIFF)" << std::endl;
    }

    // the size of the file
    if(!file.read(buffer, 4))
    {
        std::cerr << "ERROR: could not read size of file" << std::endl;
    }

    // the WAVE
    if(!file.read(buffer, 4))
    {
        std::cerr << "ERROR: could not read WAVE" << std::endl;
    }
    if(std::strncmp(buffer, "WAVE", 4) != 0)
    {
        std::cerr << "ERROR: file is not a valid WAVE file (header doesn't contain WAVE)" << std::endl;
    }

    // "fmt/0"
    if(!file.read(buffer, 4))
    {
        std::cerr << "ERROR: could not read fmt/0" << std::endl;
    }

    // this is always 16, the size of the fmt data chunk
    if(!file.read(buffer, 4))
    {
        std::cerr << "ERROR: could not read the 16" << std::endl;
    }

    // PCM should be 1?
    if(!file.read(buffer, 2))
    {
        std::cerr << "ERROR: could not read PCM" << std::endl;
    }

    // the number of channels
    if(!file.read(buffer, 2))
    {
        std::cerr << "ERROR: could not read number of channels" << std::endl;
    }
    int channels = convert_to_int(buffer, 2);

    // sample rate
    if(!file.read(buffer, 4))
    {
        std::cerr << "ERROR: could not read sample rate" << std::endl;
    }
    int sampleRate = convert_to_int(buffer, 4);

    // (sampleRate * bitsPerSample * channels) / 8
    if(!file.read(buffer, 4))
    {
        std::cerr << "ERROR: could not read (sampleRate * bitsPerSample * channels) / 8" << std::endl;
    }

    // ?? dafaq
    if(!file.read(buffer, 2))
    {
        std::cerr << "ERROR: could not read dafaq" << std::endl;
    }

    // bitsPerSample
    if(!file.read(buffer, 2))
    {
        std::cerr << "ERROR: could not read bits per sample" << std::endl;
    }
    int bitsPerSample = convert_to_int(buffer, 2);

    // data or list chunk header
    if(!file.read(buffer, 4))
    {
        std::cerr << "ERROR: could not read data or list chunk header" << std::endl;
    }
    if(std::strncmp(buffer, "LIST", 4) == 0) {
        if(!file.read(buffer, 4)) {
            std::cerr << "ERROR: could not read list chunk header size" << std::endl;
        }

        int32_t listSize = convert_to_int(buffer, 4);

        char listChunk[listSize];
        if(!file.read(listChunk, listSize)) {
            std::cerr << "ERROR: could not read list chunk" << std::endl;
        }
        if(!file.read(buffer, 4))
        {
            std::cerr << "ERROR: could not read data chunk header" << std::endl;
        }
        if(std::strncmp(buffer, "data", 4) != 0)
        {
            std::cerr << "ERROR: file is not a valid WAVE file (doesn't have 'data' tag)" << std::endl;
        }
    }else {
        if(std::strncmp(buffer, "data", 4) != 0)
        {
            std::cerr << "ERROR: file is not a valid WAVE file (doesn't have 'data' tag)" << std::endl;
        }
    }
    // size of data
    if(!file.read(buffer, 4))
    {
        std::cerr << "ERROR: could not read data size" << std::endl;
    }
    int size = convert_to_int(buffer, 4);

    /* cannot be at the end of file */
    if(file.eof())
    {
        std::cerr << "ERROR: reached EOF on the file" << std::endl;
    }
    if(file.fail())
    {
        std::cerr << "ERROR: fail state set on the file" << std::endl;
    }

	auto soundData = new SoundData(channels, sampleRate, bitsPerSample, size);

    file.read(soundData->m_data, size);

	m_soundMap.insert(std::make_pair(path, soundData));

	file.close();

	return soundData;
}

unsigned int ResourceManager::makeIrradianceMap(unsigned int cubeMap) {
	unsigned int irradianceMap;
	glGenTextures(1, &irradianceMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
	for (unsigned int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0,
					 GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);

	const auto irradiance = new Object(false);
	irradiance->setActive(false);
	irradiance->addComponent<MeshRenderer>()->loadModel("assets/defaultAssets/Models/cube.fbx");
	irradiance->getComponent<MeshRenderer>()->getMaterials()[0]->setShaderProgram(
		"assets/defaultAssets/Shaders/UnseenRenderingShaders/irradiance.vert",
		"assets/defaultAssets/Shaders/UnseenRenderingShaders/irradiance.frag"
	);
	irradiance->getComponent<MeshRenderer>()->getMaterials()[0]->setMaterialType(CUSTOM);

	glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	glm::mat4 captureViews[] =
	{
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	 };

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);
	irradiance->getComponent<MeshRenderer>()->getMaterials()[0]->setUniform<int>("environmentMap", 0);

	glViewport(0, 0, 32, 32); // don't forget to configure the viewport to the capture dimensions.
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	irradiance->getComponent<MeshRenderer>()->getMaterials()[0]->setUniform<mat4>("projection", captureProjection);
	for (unsigned int i = 0; i < 6; ++i)
	{
		irradiance->getComponent<MeshRenderer>()->getMaterials()[0]->setUniform<mat4>("view", captureViews[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
							   GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceMap, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		irradiance->update();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	delete irradiance;

	return irradianceMap;
}

unsigned int ResourceManager::makePrefilterMap(unsigned int cubeMap) {
	unsigned int prefilterMap;
	glGenTextures(1, &prefilterMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
	for (unsigned int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 128, 128, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	const auto prefilter = new Object(false);
	prefilter->setActive(false);
	prefilter->addComponent<MeshRenderer>()->loadModel("assets/defaultAssets/Models/cube.fbx");
	prefilter->getComponent<MeshRenderer>()->getMaterials()[0]->setShaderProgram(
		"assets/defaultAssets/Shaders/UnseenRenderingShaders/irradiance.vert",
		"assets/defaultAssets/Shaders/UnseenRenderingShaders/prefilteredMap.frag"
	);
	prefilter->getComponent<MeshRenderer>()->getMaterials()[0]->setMaterialType(CUSTOM);

	glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	glm::mat4 captureViews[] =
	{
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	 };

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);
	prefilter->getComponent<MeshRenderer>()->getMaterials()[0]->setUniform<int>("environmentMap", 0);
	prefilter->getComponent<MeshRenderer>()->getMaterials()[0]->setUniform<mat4>("projection", captureProjection);

	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	unsigned int maxMipLevels = 5;
	for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
	{
		// reisze framebuffer according to mip-level size.
		unsigned int mipWidth  = 128 * std::pow(0.5, mip);
		unsigned int mipHeight = 128 * std::pow(0.5, mip);
		glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
		glViewport(0, 0, mipWidth, mipHeight);

		float roughness = (float)mip / (float)(maxMipLevels - 1);
		prefilter->getComponent<MeshRenderer>()->getMaterials()[0]->setUniform<float>("roughness", roughness);

		for (unsigned int i = 0; i < 6; ++i)
		{
			//prefilterShader.setMat4("view", captureViews[i]);
			prefilter->getComponent<MeshRenderer>()->getMaterials()[0]->setUniform<mat4>("view", captureViews[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
								   GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilterMap, mip);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			prefilter->update();
		}
	}

	delete prefilter;

	return prefilterMap;
}

unsigned int ResourceManager::makeBRDFMap() {
	unsigned int brdfLUTTexture;
	glGenTextures(1, &brdfLUTTexture);

	// pre-allocate enough memory for the LUT texture.
	glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, 0);
	// be sure to set wrapping mode to GL_CLAMP_TO_EDGE
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// then re-configure capture framebuffer object and render screen-space quad with BRDF shader.
	glDeleteFramebuffers(1, &captureFBO);
	glGenFramebuffers(1, &captureFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLUTTexture, 0);

	auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete: " << fboStatus << std::endl;

	glViewport(0, 0, 512, 512);

	auto brdf = new Object(false);
	brdf->setActive(false);
	brdf->addComponent<MeshRenderer>()->loadModel("assets/defaultAssets/Models/image.fbx");
	brdf->getComponent<MeshRenderer>()->getMaterials()[0]->setShaderProgram(
		"assets/defaultAssets/Shaders/defaultUI.vert",
		"assets/defaultAssets/Shaders/UnseenRenderingShaders/brdf.frag"
	);
	//brdf->getComponent<MeshRenderer>()->getMaterials()[0]->setMaterialType(CUSTOM);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	brdf->update();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	delete brdf;

	return brdfLUTTexture;
}

void ResourceManager::setProceduralSkyShader(string path) {
	glGenFramebuffers(1, &captureFBO);
	glGenRenderbuffers(1, &captureRBO);

	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

	unsigned int envCubemap;
	glGenTextures(1, &envCubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);

	for (unsigned int i = 0; i < 6; ++i)
	{
		// note that we store each face with 16 bit floating point values
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F,
					 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	glm::mat4 captureViews[] =
	{
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	 };

	auto a = new Object(false);
	a->setActive(false);
	a->addComponent<MeshRenderer>()->loadModel("assets/defaultAssets/Models/cube.fbx");
	a->getComponent<MeshRenderer>()->getMaterials()[0]->setMaterialType(CUSTOM);
	a->getComponent<MeshRenderer>()->getMaterials()[0]->setShaderProgram(
		"assets/defaultAssets/Shaders/skybox.vert",
		"assets/Shaders/shaderSkybox.frag"
	);

	a->getComponent<MeshRenderer>()->getMaterials()[0]->setUniform<mat4>("projection", captureProjection);

	glViewport(0, 0, 512, 512); // don't forget to configure the viewport to the capture dimensions.
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (unsigned int i = 0; i < 6; ++i)
	{
		a->getComponent<MeshRenderer>()->getMaterials()[0]->setUniform<mat4>("view", captureViews[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
							   GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		a->update(); // renders a 1x1 cube
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	delete a;

	Game.getActiveScene()->setSkybox(envCubemap);
}

ResourceManager::ResourceManager()
{
}

std::int32_t ResourceManager::convert_to_int(char *buffer, std::size_t len) {
	std::int32_t a = 0;
	if(std::endian::native == std::endian::little)
		std::memcpy(&a, buffer, len);
	else
		for(std::size_t i = 0; i < len; ++i)
			reinterpret_cast<char*>(&a)[3 - i] = buffer[i];
	return a;
}
