/*
 * Written by: AwokenOwen
 * Last Updated: March 26th 2026
 */

#pragma once
#include <string>

/**
 * @brief Singleton Macro
 */
#define Game GameManager::getInstance()

class Object;
class Scene;
/**
 * @brief Singleton class that runs the engine
 */
class GameManager
{
public:
	/**
	 * @brief Starts up all managers and returns 0 if successful and 1 if failed
	 *
	 * @return int
	 */
	static int initialize();

	/**
	 * @brief Starts the while loop that keeps the window open and sends updates to all Objects
	 */
	void run();

	/**
	 * @brief Shut Down all Managers freeing all data
	 */
	static void terminate();

	/**
	 * @brief GameManager Singleton get function
	 *
	 * @return static GameManager Instance
	 */
	static GameManager& getInstance();

	/**
	 * @brief Loads a new scene based on the name given
	 *
	 * @param name The string name of the new active scene
	 * @return
	 */
	int loadScene(const std::string& name);

	/**
	 * @brief Getter for the active Scene
	 *
	 * @return Scene*
	 */
	[[nodiscard]] Scene* getActiveScene() const;

	/**
	 * @brief Setter for the starting scene upon opening
	 *
	 * @param name The string name of the new base scene
	 */
	void setBaseScene(const std::string &name);

	/**
	 * @brief Getter for the name of the current base scene
	 *
	 * @return string
	 */
	std::string getBaseScene();

	/**
	 * @brief Function called to add a scene to the active scene's object list
	 *
	 * @param obj
	 */
	void instantiate(Object* obj) const;

	/**
	 * @brief Returns the time in seconds that the program has run
	 *
	 * @return float
	 */
	static float getTime();
	/**
	 * @brief Returns the calculated delta time of the previous frame
	 *
	 * @return float
	 */
	[[nodiscard]] double getDeltaTime() const;

	/**
	 * @brief Sets the while loop in run() to finish
	 */
	static void quit();

	float getFps();

private:
	/**
	 * @brief Private Constructor for singleton functionality
	 */
	GameManager();
	/**
	 * @brief The difference between getTime() last frame and the current frame
	 */
	double m_deltaTime = 0;

	/**
	 * @brief Scene variable for the current active scene
	 */
	Scene* p_activeScene{};

	/**
	 * @brief Name of the starting scene
	 */
	std::string m_nameOfBaseScene = "Main";

	float m_fps{};
};