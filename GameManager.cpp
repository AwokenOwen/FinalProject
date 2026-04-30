#include "GameManager.h"
#include "WindowManager.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "AudioManager.h"
#include "Scene.h"
#include "iostream"
#include <chrono>
#include <thread>

using namespace std;

int GameManager::initialize()
{
	cout << "Starting Engine...\n\n";

	if (Window.initialize())
	{
		cout << "WindowManager failed to start\n";
	}
	cout << "WindowManager started\n";
	if (Resource.initialize())
	{
		cout << "ResourceManager failed to start\n";
	}
	cout << "ResourceManager started\n";
	if (Audio.initialize()) {
		cout << "AudioManager failed to start\n";
	}
	cout << "AudioManager started\n";

	cout << "GameManager Started\n\n";

	cout << "Loading game with scene " << Game.getBaseScene() << "\n\n";
	Game.loadScene(Game.m_nameOfBaseScene);

	return 0;
}

void GameManager::run()
{
	cout << "Starting Game...\n\n";

	double lastTime = getTime();
	double targetFrameTime = 1.0 / 60.0;

	while (!glfwWindowShouldClose(Window.getWindow()))
	{
		Window.clear();

		glfwPollEvents();

		p_activeScene->update();
		p_activeScene->lateUpdate();

		Window.swap();
		double currentFrameTime = getTime();
		double deltaTime = currentFrameTime - lastTime;
		m_deltaTime = glm::max(deltaTime, targetFrameTime);
		m_fps = 1 / m_deltaTime;
		lastTime = currentFrameTime;
		std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(glm::max(0.0, targetFrameTime - deltaTime) * 1000)));
	}
}
void GameManager::terminate()
{
	cout << "Stopping Engine...\n\n";

	Window.terminate();
	cout << "Shut Down Window Manager\n";
	Input.terminate();
	cout << "Shut Down Input Manager\n";
	Resource.terminate(); 
	cout << "Shut Down Resource Manager\n";

	cout << "Shut Down Game Manager\n";
}

GameManager& GameManager::getInstance()
{
	static GameManager single;
	return single;
}

int GameManager::loadScene(const std::string& name) {
	//do stuff (set active Scene to the scene with name)
	//load all objects in the scene
	//this is the first frame technically
	p_activeScene = new Scene();
	p_activeScene->awake();
	p_activeScene->start();
	return 0;
}

Scene* GameManager::getActiveScene() const {
	return p_activeScene;
}

void GameManager::setBaseScene(const std::string &name) {
	m_nameOfBaseScene = name;
}

std::string GameManager::getBaseScene() {
	return m_nameOfBaseScene;
}

void GameManager::instantiate(Object *obj) const {
	p_activeScene->instantiate(obj);
}

float GameManager::getTime()
{
	return static_cast<float>(glfwGetTime());
}

double GameManager::getDeltaTime() const
{
	return m_deltaTime;
}

GameManager::GameManager() = default;

void GameManager::quit()
{
	glfwSetWindowShouldClose(Window.getWindow(), GLFW_TRUE);
}

float GameManager::getFps() {
	return m_fps;
}
