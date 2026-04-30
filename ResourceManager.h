/*
 * Written by: AwokenOwen
 * Last Updated: March 25th 2026
 */

#pragma once
#include <cstdint>
#include <string>
#include <map>
#include <vector>

#include "vec3.hpp"
#include "AL/al.h"

using namespace std;

/**
 * @brief Singleton macro
 */
#define Resource ResourceManager::getInstance()

struct SoundData {
	std::uint8_t m_channels;
	std::int32_t m_sampleRate;
	std::uint8_t m_bitsPerSample;
	int m_data_size;
	ALenum m_format;

	char* m_data;

	SoundData(std::uint8_t channels, std::int32_t sampleRate, std::uint8_t bitsPerSample, int dataSize) {
		m_channels = channels;
		m_sampleRate = sampleRate;
		m_bitsPerSample = bitsPerSample;
		m_data_size = dataSize;

		if(m_channels == 1 && m_bitsPerSample == 8)
			m_format = AL_FORMAT_MONO8;
		else if(m_channels  == 1 && m_bitsPerSample == 16)
			m_format = AL_FORMAT_MONO16;
		else if(m_channels  == 2 && m_bitsPerSample == 8)
			m_format = AL_FORMAT_STEREO8;
		else if(m_channels  == 2 && m_bitsPerSample == 16)
			m_format = AL_FORMAT_STEREO16;

		m_data = new char[m_data_size];
	};

	~SoundData() {
		delete[] m_data;
	}
};

/**
 * @brief Forward Declaration for the Mesh class
 */
class Mesh;
/**
 * @brief Manger that handles loading models, images, and shaders from files on the hard drive
 */
class ResourceManager
{
public:
	/**
	 * @brief Start the Resource Manager make folders and files necessary for running the game if not made already. Return 0 if successful, -1 if unsuccessful
	 *
	 * @return int
	 */
	static int initialize();

	/**
	 * @brief close files and stop the Resource Manager
	 */
	static void terminate();

	/**
	 * @brief Singleton get function
	 *
	 * @return ResourceManager
	 */
	static ResourceManager& getInstance();

	/**
	 * @brief grab a scene from the assets folder and load it into the World Manager. Return 0 if successful and -1 if unsuccessful
	 *
	 * @param name name of the scene
	 * @return int
	 */
	static int loadScene(string name);

	/**
	 * @brief Loads image from file and returns an unsigned int representing the OpenGL texture
	 *
	 * @param path Path to image file
	 * @return unsigned int
	 */
	unsigned int loadImage(const char* path);

	/**
	 * @brief Loads image files from folder and turns it into a cube map. Returns an unsigned int representing the OpenGL cube map
	 *
	 * @param paths Path to the folder holding the cube map images
	 * @return unsigned int
	 */
	static unsigned int loadCubeMap(const vector<const char*> &paths);

	unsigned int loadHDR(const char* path);

	/**
	 * @brief Tries to get meshes from the Map of OpenGL mesh vectors using the path as a key. If found returns the vector if not return an empty vector.
	 *
	 * @param path Path to the meshes
	 * @return vector<Mesh*>
	 */
	vector<Mesh*> getMeshFromMap(string path);
	/**
	 * @brief Inserts a vector of meshes into the mesh map using the path to the meshes as the key
	 *
	 * @param path Path to the meshes
	 * @param meshes The vector of meshes from that path
	 */
	void addMeshToMap(string path, vector<Mesh*> meshes);

	/**
	 * @brief Tries to get an unsigned int representing an OpenGL shader program from map using a combination of the vertex and fragment shader paths as a key
	 *
	 * @param path A combination of the vertex and fragment shader paths
	 * @return unsigned int
	 */
	unsigned int getShaderProgramFromMap(string path);
	/**
	 * @brief Inserts an unsigned int representing an OpenGL shader program into the map using the combination of the vertex and fragment shader paths as a key
	 *
	 * @param path A combination of the vertex and fragment shader paths
	 * @param shaderProgram An unsigned int representing an OpenGL shader program
	 */
	void addShaderProgramToMap(string path, unsigned int shaderProgram);

	/**
	 * @brief Loading function for the sounds
	 *
	 * @param path Path to the WAV file
	 * @return SoundData
	 */
	SoundData* loadSound(string path);

	unsigned int makeIrradianceMap(unsigned int cubeMap);

	unsigned int makePrefilterMap(unsigned int cubeMap);

	unsigned int makeBRDFMap();

	void setProceduralSkyShader(string path);

private:
	/**
	 * @brief Private constructor
	 */
	ResourceManager();

	std::int32_t convert_to_int(char *buffer, std::size_t len);
	/**
	 * @brief Map for already loaded meshes to save time on file loading
	 */
	map<string, vector<Mesh*>> m_meshMap;
	/**
	 * @brief Map for already loaded textures to save time on file loading
	 */
	map<string, unsigned int> m_textureMap;
	/**
	 * @brief Map for already loaded shaders to save time on file loading
	 */
	map<string, unsigned int> m_shaderMap;
	/**
	 * @brief Map for already loaded sounds to save time on file loading
	 */
	map<string, SoundData*> m_soundMap;

	unsigned int captureFBO, captureRBO;
};