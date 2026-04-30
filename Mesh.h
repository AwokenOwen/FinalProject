/*
 * Written by: AwokenOwen
 * Last Updated: March 28th 2026
 */

#pragma once
#include <vector>
#include "glm.hpp"

using namespace glm;
using namespace std;

/**
 * @brief Forward Declaration of Material class
 */
class Material;
/**
 * @brief Forward Declaration of Object class
 */
class Object;
/**
 * @brief Forward Declaration of Component
 */
class Component;
/**
 * @brief Class to hold data for objects to be drawn to the screen
 */
class Mesh
{
public:
	/**
	 * @brief Struct for vertex to store the position, normal and texCoords
	 */
	struct Vertex
	{
		/**
		 * @brief Position data for the vertex to be pushed to the shader
		 */
		vec3 m_position{};
		/**
		 * @brief Normal data for the vertex to be pushed to the shader
		 */
		vec3 m_normal{};
		/**
		 * @brief TexCoords data for the vertex to be pushed to the shader
		 */
		vec2 m_texCoords{};

		/**
		 * @brief Default constructor setting all to 0
		 */
		Vertex()
		{
			m_position = vec3(0.0f);
			m_normal = vec3(0.0f);
			m_texCoords = vec2(0.0f);
		}

		/**
		 * @brief Constructor where position is set, where everything else is set to 0
		 *
		 * @param position The Position data for the vertex
		 */
		explicit Vertex(const vec3 position)
		{
			m_position = position;
			m_normal = vec3(0.0f);
			m_texCoords = vec2(0.0f);
		}

		/**
		 * @brief Constructor where all data needs to be set
		 *
		 * @param position The Position data for the vertex
		 * @param normal The Normal data for the vertex
		 * @param texCoords The TexCoords data for the vertex
		 */
		explicit Vertex(const vec3 position, const vec3 normal, const vec2 texCoords) {
			m_position = position;
			m_normal = normal;
			m_texCoords = texCoords;
		}
	};

	/**
	 * @brief Constructor for a vector of vertices and indices to create the mesh
	 *
	 * @param vertices The vector of vertices
	 * @param indices The vector of indices
	 */
	Mesh(const vector<Vertex> &vertices, const vector<unsigned int> &indices);
	/**
	 * @brief Default deconstructor
	 */
	virtual ~Mesh() = default;

	/**
	 * @brief Getter for parent Component
	 *
	 * @return Component
	 */
	Component* getParent() const;
	/**
	 * @brief Setter for the parent variable
	 *
	 * @param parent The new Component Parent
	 */
	void setParent(Component* parent);

	/**
	 * @brief Getter for the material variable
	 *
	 * @return Material
	 */
	[[nodiscard]] Material* getMaterial() const;
	/**
	 * @brief Setter for the material variable
	 *
	 * @param material Material instance
	 */
	void setMaterial(Material* material);

	/**
	 * @brief For the draw call
	 */
	virtual void draw();

protected:
	/**
	 * @brief Mesh variables
	 */
	unsigned int VAO{}, VBO{}, EBO{};

private:
	/**
	 * @brief Creating the OpenGL buffers and pushing vertices (position, normal, texCoords)
	 */
	void setupMesh();
	/**
	 * @brief Helper function to push the matrices for the vertex shader
	 *
	 * @param shaderProgram The shader program to push the matrices to
	 */
	void setUpShaderMatrices(unsigned int shaderProgram) const;
	/**
	 * @brief Helper function to push the other uniforms
	 *
	 * @param shaderProgram The shader program to push the uniforms to
	 */
	static void setUpShaderVariables(unsigned int shaderProgram);
	/**
	 * @brief Helper function to push the uniforms for the directional light
	 *
	 * @param shaderProgram The shader program to push the directional light variables to
	 */
	void setUpDirectionalLight(unsigned int shaderProgram) const;
	/**
	 * @brief Helper function to push all the point lights to
	 *
	 * @param shaderProgram The shader program to push the point light data to
	 */
	static void setUpPointLights(unsigned int shaderProgram);

	/**
	 * @brief Material variable that handles this mesh
	 */
	Material* m_material{};
	/**
	 * @brief The Component parent variable of this mesh
	 */
	Component* m_parent{};
	/**
	 * @brief The vertice vector for the mesh
	 */
	vector<Vertex> m_vertices;
	/**
	 * @brief The indices vector for the mesh
	 */
	vector<unsigned int> m_indices;
};
