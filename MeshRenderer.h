/*
 * Written by: AwokenOwen
 * Last Updated: March 28th 2026
 */

#pragma once
#include "Component.h"
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

using namespace std;

/**
 * @brief Forward Declaration of Material class
 */
class Material;
/**
 * @brief Forward Declaration of Mesh class
 */
class Mesh;
/**
 * @brief Component that draws meshes to the screen
 */
class MeshRenderer : public Component
{
public:
	/**
	 * @brief Constructor of MeshRenderer used to call parent constructor
	 *
	 * @param parent The parent object of the component
	 */
	explicit MeshRenderer(Object* parent);

	/**
	 * @brief Component override called every frame used to draw the meshes in the m_meshes vector
	 */
	void update() override;

	/**
	 * @brief Alternate to loadModel it's used to load specifically a cube as an inverted cube map
	 *
	 * @param paths Paths to the images used in the cube map
	 */
	void loadCubeMap(const vector<const char*> &paths);

	/**
	 * @brief Used to load a 3D or 2D model into the engine and attach it to the m_meshes vector
	 *
	 * @param path Path to the model
	 */
	void loadModel(const string &path);

	/**
	 * @brief Setter for the shader program for a specific material
	 *
	 * @param vertexShaderPath Path to vertex shader
	 * @param fragmentShaderPath Path to fragment shader
	 * @param index index of the material in the material vector
	 */
	void setShaderProgram(const char* vertexShaderPath, const char* fragmentShaderPath, int index = 0) const;

	/**
	 * @brief Getter for Material vector
	 *
	 * @return vector<Material*>
	 */
	[[nodiscard]] vector<Material*> getMaterials() const;

	void setMaterial(Material* material, int index = 0);

private:
	/**
	 * @brief Vector of 3D models to draw
	 */
	vector<Mesh*> m_meshes;
	/**
	 * @brief Vector of material associated with the 3D models
	 */
	vector<Material*> m_materials;

	/**
	 * @brief Helper function for loading models
	 *
	 * @param node Assimp node
	 * @param scene Assimp scene
	 * @param path path to model
	 */
	void processNode(const aiNode* node, const aiScene* scene, const string &path);
	/**
	 * @brief Helper function for loading models
	 *
	 * @param mesh Assimp mesh
	 * @param scene Assimp scene
	 * @return Mesh*
	 */
	Mesh* processMesh(aiMesh* mesh, const aiScene* scene);
};

