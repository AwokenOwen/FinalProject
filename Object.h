/*
 * Written by: AwokenOwen
 * Last Updated: March 28th 2026
 */

#pragma once
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/quaternion.hpp"
#include <vector>

using namespace glm;
using namespace std;

/**
 * @brief Forward Declaration of Component
 */
class Component;
/**
 * @brief Base class for all Objects that will be added to the game
 */
class Object
{
public:
	virtual ~Object() = default;

	/**
	 * @brief Default constructor adds itself to the world list
	 *
	 * @param inWorld whether the object gets loaded into the scene
	 */
	explicit Object(bool inWorld = true);

	/**
	 * @brief Called once at the start of the frame it is enabled on
	 */
	virtual void onEnable();
	/**
	 * @brief Called on the first frame if enabled
	 */
	virtual void start();
	/**
	 * @brief Called every frame
	 */
	virtual void update();
	/**
	 * @brief called on the last frame it is enabled
	 */
	virtual void onDisable();
	/**
	 * @brief to be called when destroying itself
	 */
	virtual void destroy();

	/**
	 * @brief get the parent object if there is one
	 *
	 * @return Object*
	 */
	[[nodiscard]] Object* getParent() const;

	/**
	 * @brief Adds a component to an Object and returns that component or of the component it already added returns already added component
	 *
	 * @tparam T Class of the component
	 * @return T*
	 */
	template <typename T> T* addComponent();
	/**
	 * @brief Getter for any component added to the Object
	 *
	 * @tparam T Component being looked for
	 * @return T*
	 */
	template <typename T> T* getComponent();

	/**
	 * @brief sets the current active state of the object to enabled or disabled and calls the correlated functions
	 *
	 * @param activeState new bool determining new active state
	 */
	void setActive(bool activeState);
	/**
	 * @brief Getter for the current active state
	 *
	 * @return bool
	 */
	[[nodiscard]] bool getActiveState() const;

	/**
	 * @brief Function to add a child object to another object returns 0 if successful and -1 if unsuccessful. Unsuccessful if there is a loop
	 *
	 * @param child The child object wanted to be added
	 * @return int
	 */
	int addChild(Object* child);

	/**
	 * @brief Getter for the World Model Matrix of Object
	 *
	 * @return
	 */
	[[nodiscard]] mat4 getModelMatrix() const;
	/**
	 * @brief Getter for world position
	 *
	 * @return vec3
	 */
	[[nodiscard]] vec3 getWorldPosition() const;
	/**
	 * @brief Getter for world rotation
	 *
	 * @return quat
	 */
	[[nodiscard]] quat getWorldRotation() const;
	/**
	 * @brief Getter for world scale
	 *
	 * @return vec3
	 */
	[[nodiscard]] vec3 getWorldScale() const;

	/**
	 * @brief Getter for Forward Vector
	 *
	 * @return vec3
	 */
	[[nodiscard]] vec3 getForward() const;
	/**
	 * @brief Getter for Right Vector
	 *
	 * @return vec3
	 */
	[[nodiscard]] vec3 getRight() const;
	/**
	 * @brief Getter for Up Vector
	 *
	 * @return vec3
	 */
	[[nodiscard]] vec3 getUp() const;

	/**
	 * @brief Getter for Local Position
	 *
	 * @return vec3
	 */
	[[nodiscard]] vec3 getLocalPosition() const;
	/**
	 * @brief Getter for Local Rotation
	 *
	 * @return quat
	 */
	[[nodiscard]] quat getLocalRotation() const;
	/**
	 * @brief Getter for Local Scale
	 *
	 * @return vec3
	 */
	[[nodiscard]] vec3 getLocalScale() const;

	/**
	 * @brief Setter for Local Position
	 *
	 * @param position new position
	 */
	void setLocalPosition(vec3 position);
	/**
	 * @brief Setter for Local Rotation
	 *
	 * @param rotation new rotation
	 */
	void setLocalRotation(quat rotation);
	/**
	 * @brief Setter for Local Scale
	 *
	 * @param scale new scale
	 */
	void setLocalScale(vec3 scale);

	/**
	 * @brief Setter for the transparent variable
	 *
	 * @param transparent The new transparent variable
	 */
	void setTransparent(bool transparent);
	/**
	 * @brief Getter for the transparent variable. True if the object renders and is transparent. False otherwise
	 *
	 * @return bool
	 */
	[[nodiscard]] bool getTransparent() const;
	/**
	 * @brief Setter for the parent Object of this object
	 *
	 * @param parent new Parent Object
	 */
	void setParent(Object* parent);

protected:
	/**
	 * @brief Helper function for creating the world variables and forward, right, and up
	 *
	 * @return mat4
	 */
	[[nodiscard]] mat4 localModelMatrix() const;

	/**
	 * @brief Parent object of this object, nullptr if no parent
	 */
	Object* m_parent = nullptr;

	/**
	 * @brief Vector of child objects connected to this Object
	 */
	vector<Object*> m_children;

	/**
	 * @brief Vector of components on this Object
	 */
	vector<Component*> m_components;

	/**
	 * @brief Holder of the active state of the object
	 */
	bool m_activeState{};

	/**
	 * @brief The World Model Matrix of the object updated every frame
	 */
	mat4 worldModelMatrix{};
	/**
	 * @brief The position of the Object in local space
	 */
	vec3 m_localPosition{};
	/**
	 * @brief The rotation of the Object in local space and in a quaternion to avoid gimble lock
	 */
	quat m_localRotation{};
	/**
	 * @brief The scale of the Object in local space
	 */
	vec3 m_localScale{};

	/**
	 * @brief The position of the Object in world space
	 */
	vec3 m_worldPosition{};
	/**
	 * @brief The rotation of the Object in world pace and in a quaternion to avoid gimble lock
	 */
	quat m_worldRotation{};
	/**
	 * @brief The scale of the Object in world space
	 */
	vec3 m_worldScale{};

	/**
	 * @brief The forward vector of the Object in world space
	 */
	vec3 m_forward{};
	/**
	 * @brief The right vector of the Object in world space
	 */
	vec3 m_right{};
	/**
	 * @brief The up vector of the Object in world space
	 */
	vec3 m_up{};

	/**
	 * @brief Bool representing if the Object renders and is transparent, false otherwise
	 */
	bool m_transparent{};
};

template<typename T>
inline T* Object::addComponent()
{
	static_assert(std::is_base_of<Component, T>());

	for (auto & m_component : m_components)
	{
		if (dynamic_cast<T*>(m_component))
		{
			return dynamic_cast<T*>(m_component);
		}
	}

	T* a = new T(this);

	m_components.push_back(a);
	return a;
}

template<typename T>
inline T* Object::getComponent()
{
	static_assert(std::is_base_of<Component, T>());

	for (auto & m_component : m_components)
	{
		if (dynamic_cast<T*>(m_component))
		{
			return dynamic_cast<T*>(m_component);
		}
	}

	return nullptr;
}
