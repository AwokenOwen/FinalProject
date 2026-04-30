#include "Scene.h"
#include <iostream>
#include <string>
#include <fstream>
#include "Camera.h"
#include "Object.h"
#include "DirectionalLight.h"
#include "MeshRenderer.h"
#include "Material.h"
#include <algorithm>

#include "ResourceManager.h"
#include "WindowManager.h"

void Scene::awake()
{
	p_screen = new Object(false);
	auto m = p_screen->addComponent<MeshRenderer>();
	m->loadModel("assets/defaultAssets/Models/image.fbx");
	m->getMaterials()[0]->setShaderProgram(
		"assets/defaultAssets/Shaders/defaultPostProcess.vert",
		"assets/defaultAssets/Shaders/defaultPostProcess.frag");
	Window.setFrameBufferObject(p_screen);
	Window.sendFrameBufferTexture();

    if (m_camera == nullptr)
    {
        m_camera = new Camera();
        std::cout << "Camera Created" << std::endl;
    }
}

void Scene::start() const {
	for (const auto i : m_inScene)
	{
		if (i->getActiveState())
		{
			i->start();
		}
	}
}

void Scene::update() const {
	Window.activateFrameBuffer();
	for (const auto i : m_inSceneOpaque)
	{
		if (i->getActiveState())
		{
			i->update();
		}
	}
	for (const auto i : m_inSceneTransparent)
	{
		if (i->getActiveState())
		{
			i->update();
		}
	}
}

void Scene::lateUpdate()
{
	Window.deactivateFrameBuffer();
	p_screen->update();
	if (m_toBeAdded.size() > 0) {
		for (auto i : m_toBeAdded)
		{
			m_inScene.push_back(i);
		}
		m_toBeAdded.clear();
		m_toBeAdded.clear();
		refreshTransparency();
	}

	for (const auto & i : m_toBeDeleted)
	{
		i->destroy();
	}
	m_toBeDeleted.clear();
}

string Scene::getName()
{
	return m_name;
}

void Scene::instantiate(Object* obj)
{
	m_toBeAdded.push_back(obj);
}

void Scene::setDirectionalLight(DirectionalLight *directionalLight) {
	m_directionalLight = directionalLight;
}

DirectionalLight * Scene::getDirectionalLight() const {
	return m_directionalLight;
}

void Scene::addPointLight(PointLight *pointLight) {
	m_pointLights.push_back(pointLight);
}

vector<PointLight *> Scene::getPointLights() {
	return m_pointLights;
}

Camera* Scene::getCamera() const {
    return m_camera;
}

void Scene::setSkybox(unsigned int cubeMapTexture) {
	MeshRenderer* m;
	m_skyboxTexture = cubeMapTexture;
	m_irradianceMapTexture = Resource.makeIrradianceMap(m_skyboxTexture);
	m_prefilterTexture = Resource.makePrefilterMap(m_skyboxTexture);
	m_brdfTexture = Resource.makeBRDFMap();
	if (m_skybox == nullptr)
	{
		m_skybox = new Object;
		m = m_skybox->addComponent<MeshRenderer>();
	}
	else
	{
		m = m_skybox->getComponent<MeshRenderer>();
	}
	m->loadModel("assets/defaultAssets/Models/cube.fbx");
	m->getMaterials()[0]->setMaterialType(SKYBOX);
	m->setShaderProgram("assets/defaultAssets/Shaders/skybox.vert", "assets/defaultAssets/Shaders/skybox.frag");
}

void Scene::loadDefaultSkybox()
{
	setSkybox(Resource.loadHDR("assets/defaultAssets/Skybox/skybox.hdr"));
}

void Scene::refreshTransparency() {
	m_inSceneOpaque.clear();
	m_inSceneTransparent.clear();
	for (const auto i : m_inScene) {
		if (i->getTransparent()) {
			m_inSceneTransparent.push_back(i);
		}else {
			m_inSceneOpaque.push_back(i);
		}
	}
	ranges::sort(m_inSceneTransparent, [this](const Object* a, const Object* b) {
		const float distance_a = glm::length(m_camera->getWorldPosition() - a->getWorldPosition());
		const float distance_b = glm::length(m_camera->getWorldPosition() - b->getWorldPosition());
		return distance_a > distance_b;
	});
}

unsigned int Scene::getSkyboxTexture() {
	return m_skyboxTexture;
}

unsigned int Scene::getIrradianceMapTexture() {
	return m_irradianceMapTexture;
}

unsigned int Scene::getPrefilterTexture() {
	return m_prefilterTexture;
}

unsigned int Scene::getBrdfTexture() {
	return m_brdfTexture;
}
