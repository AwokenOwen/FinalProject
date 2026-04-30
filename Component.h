/*
 * Written by: AwokenOwen
 * Last Updated: March 25th 2026
 */

#pragma once

/**
 * @brief Forward declaration of Object Class to store parent
 */
class Object;
/**
 * @brief Base Class for Logic to go on Objects
 */
class Component
{
public:
	/**
	 * Component Constructor
	 *
	 * @param parent The parent Object of the Component
	 */
	explicit Component(Object* parent);
	virtual ~Component() = default;

	/**
	 * @brief Called on the frame the Component is enabled on
	 */
	virtual void onEnable();
	/**
	 * @brief Called once at the start of the Scene if enabled
	 */
	virtual void start();
	/**
	 * @brief Called once every frame of the Scene if enabled
	 */
	virtual void update();
	/**
	 * @brief Called on the frame the Component is disabled on
	 */
	virtual void onDisable();

	/**
	 * @brief Gets the parent Object of the Component
	 * @return Object*
	 */
	[[nodiscard]] Object *getParent() const;

	/**
	 * @brief Gets the active state of the Component
	 *
	 * @return bool
	 */
	[[nodiscard]] bool getActiveState() const;
	/**
	 * @brief Sets the active state of the component
	 *
	 * @param activeState the new active state of the component
	 */
	void setActive(bool activeState);

private:
	/**
	 * @brief Parent Object of this component
	 */
	Object* p_parent = nullptr;

	/**
	 * @brief Active state of this component
	 */
	bool m_enabled;
};