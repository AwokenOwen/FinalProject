/*
 * Written by: AwokenOwen
 * Last Updated: March 28th 2026
 */

#pragma once
#include <string>
#include <vector>
#include "glm.hpp"
#include "Event.h"

using namespace glm;
using namespace std;

/**
 * @brief Forward Declaration for Camera class
 */
class Camera;
/**
 * @brief Forward Declaration for Object class
 */
class Object;
/**
 * @brief Forward Declaration for DirectionalLight class
 */
class DirectionalLight;
/**
 * @brief Forward Declaration for PointLight class
 */
class PointLight;
/**
 * @brief Class that handles all the objects that need to be currently loaded
 */
class Scene
{
public:
	/**
	 * @brief Default constructor
	 */
	Scene() = default;

	/**
	 * @brief Called once at the start of a scene
	 */
	void awake();
	/**
	 * @brief Called on the first frame if enabled
	 */
	void start() const;
	/**
	 * @brief Called every frame
	 */
	void update() const;
	/**
	 * @brief Called every frame after update
	 */
	void lateUpdate();
	/**
	 * @brief Getter for the name of the Scene
	 *
	 * @return string
	 */
	string getName();

	/**
	 * @brief Used to add an object into the list of objects in the scene
	 *
	 * @param obj The object to be added
	 */
	void instantiate(Object* obj);

	/**
	 * @brief Getter for ambient light color
	 *
	 * @return vec3
	 */
	[[nodiscard]] vec3 getAmbientColor() const;
	/**
	 * @brief Getter for ambient light power
	 *
	 * @return float
	 */
	[[nodiscard]] float getAmbientPower() const;

	/**
	 * @brief Setter for the single Directional light in the scene
	 *
	 * @param directionalLight The new directional light
	 */
	void setDirectionalLight(DirectionalLight* directionalLight);
	/**
	 * @brief Getter for the Directional Light
	 *
	 * @return DirectionalLight
	 */
	[[nodiscard]] DirectionalLight* getDirectionalLight() const;

	/**
	 * @brief Function to add a point light into the scene
	 *
	 * @param pointLight The Point light to be added
	 */
	void addPointLight(PointLight* pointLight);
	/**
	 * @brief Getter for the point light vector
	 *
	 * @return vector<PointLight*>
	 */
	vector<PointLight*> getPointLights();

	/**
	 * @brief Getter for the active camera
	 *
	 * @return Camera*
	 */
	[[nodiscard]] Camera* getCamera() const;

	/**
	 * @brief Setter for the active skybox
	 *
	 * @param paths Vector of paths to each image file of the cube map skybox
	 */
	void setSkybox(unsigned int cubeMapTexture);
	/**
	 * @brief Grabs the default skybox images and makes a skybox of it
	 */
	void loadDefaultSkybox();

	/**
	 * @brief When the transparency of an Object is changed it refreshes the vectors that draw them to draw them in the correct order
	 */
	void refreshTransparency();

	unsigned int getSkyboxTexture();
	unsigned int getIrradianceMapTexture();
	unsigned int getPrefilterTexture();
	unsigned int getBrdfTexture();

private:
	/**
	 * @brief Vector of all objects in the scene
	 */
	vector<Object*> m_inScene{};
	/**
	 * @brief Vector of objects in the scene
	 */
	vector<Object*> m_inSceneOpaque{};
	/**
	 * @brief Vector of objects in the scene
	 */
	vector<Object*> m_inSceneTransparent{};
	/**
	 * @brief Vector of objects that need to be added
	 */
	vector<Object*> m_toBeAdded{};
	/**
	 * @brief Vector of objects that need to be deleted
	 */
	vector<Object*> m_toBeDeleted{};

	/**
	 * @brief The name of the scene
	 */
	string m_name{};

	/**
	 * @brief The current directional light in the scene
	 *
	 * || Eventually add ability for multiple ||
	 */
	DirectionalLight* m_directionalLight{};
	/**
	 * @brief The current point lights in the scene
	 */
	vector<PointLight*> m_pointLights{};

	/**
	 * @brief The active camera
	 */
	Camera* m_camera{};
	/**
	 * @brief The active skybox
	 */
	Object* m_skybox{};

	unsigned int m_skyboxTexture{};
	unsigned int m_irradianceMapTexture{};
	unsigned int m_prefilterTexture{};
	unsigned int m_brdfTexture{};

	Object* p_screen{};
};