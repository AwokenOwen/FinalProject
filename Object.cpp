#include "Object.h"
#include "Component.h"
#include "iostream"
#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/matrix_decompose.hpp>

#include "GameManager.h"

Object::Object(const bool inWorld)
{
	//set default variables and instantiate
	m_localPosition = vec3(0, 0, 0);
	m_localRotation = quat(vec3(0, 0, 0));
	m_localScale = vec3(1, 1, 1);

	m_worldPosition = vec3(0, 0, 0);
	m_worldRotation = quat(vec3(0, 0, 0));
	m_worldScale = vec3(1, 1, 1);

	setActive(true);

	if (inWorld)
		Game.instantiate(this);
}

//called once at the start of the frame it is enabled on
void Object::onEnable()
{
	for (const auto & m_component : m_components)
	{
		if (m_component->getActiveState())
		{
			m_component->onEnable();
		}
	}

	m_activeState = true;
}

//called on the first frame if enabled
void Object::start()
{
	for (const auto & m_component : m_components)
	{
		if (m_component->getActiveState())
		{
			m_component->start();
		}
	}
}

//called every frame
void Object::update()
{
	worldModelMatrix = localModelMatrix();
	Object* current = m_parent;
	while (current != nullptr) {
		worldModelMatrix = current->localModelMatrix() * worldModelMatrix;
		current = current->m_parent;
	}

	m_forward = vec3(worldModelMatrix[0][2], worldModelMatrix[1][2], worldModelMatrix[2][2]);
	m_up = vec3(worldModelMatrix[0][1], worldModelMatrix[1][1], worldModelMatrix[2][1]);
	m_right = vec3(worldModelMatrix[0][0], worldModelMatrix[1][0], worldModelMatrix[2][0]);

	vec3 skew;
	vec4 perspective;

	decompose(worldModelMatrix, m_worldScale, m_worldRotation, m_worldPosition, skew, perspective);

	for (const auto & m_component : m_components)
	{
		if (m_component->getActiveState())
		{
			m_component->update();
		}
	}
}

//called on the last frame it is enabled
void Object::onDisable()
{
	for (const auto & m_component : m_components)
	{
		if (m_component->getActiveState())
		{
			m_component->onDisable();
		}
	}
	m_activeState = false;
}

void Object::destroy()
{
	onDisable();

	m_components.clear();

	for (const auto & i : m_children)
	{
		i->destroy();
	}
	delete this;
}

//return parent object if there is one
Object* Object::getParent() const {
	return m_parent;
}

/*
 *	CHANGE THIS || MAKE IT EVENT BASED TO RUN THOSE EVENTS
 */
void Object::setActive(const bool activeState)
{
	if (activeState != m_activeState)
	{
		if (activeState)
		{
			onEnable();
		}
		else
		{
			onDisable();
		}
		m_activeState = activeState;
	}
}

bool Object::getActiveState() const {
	return m_activeState;
}

mat4 Object::localModelMatrix() const
{
	auto model = mat4(1.0f);

	model = translate(model, getLocalPosition());
	model = rotate(model, angle(getLocalRotation()), axis(getLocalRotation()));
	model = scale(model, getLocalScale());

	return model;
}

void Object::setParent(Object* parent)
{
	m_parent = parent;
}


int Object::addChild(Object* child) 
{
	auto current = this;
	while (current != nullptr)
	{
		if (current == child)
		{
			std::cout << "Error: Child is currently a parent of Object\n";
			return 1;
		}
		current = current->getParent();
	}

	child->setParent(this); 
	m_children.push_back(child);
	return 0;
}

mat4 Object::getModelMatrix() const {
	return worldModelMatrix;
}

vec3 Object::getWorldPosition() const {
	return m_worldPosition;
}

quat Object::getWorldRotation() const {
	return m_worldRotation;
}

vec3 Object::getWorldScale() const {
	return m_worldScale;
}

vec3 Object::getForward() const {
	return m_forward;
}

vec3 Object::getRight() const {
	return m_right;
}

vec3 Object::getUp() const {
	return m_up;
}

vec3 Object::getLocalPosition() const {
	return m_localPosition;
}

quat Object::getLocalRotation() const {
	return m_localRotation;
}

vec3 Object::getLocalScale() const {
	return m_localScale;
}

void Object::setLocalPosition(const vec3 position)
{
	m_localPosition = position;
}

void Object::setLocalRotation(const quat rotation)
{
	m_localRotation = rotation;
}

void Object::setLocalScale(const vec3 scale)
{
	m_localScale = scale;
}

void Object::setTransparent(const bool transparent) {
	m_transparent = transparent;
}

bool Object::getTransparent() const {
	return m_transparent;
}
