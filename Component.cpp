#include "Component.h"

Component::Component(Object* parent)
{
	p_parent = parent;
	m_enabled = true;
}

void Component::onEnable()
{
	
}

void Component::start()
{

}

void Component::update()
{

}

void Component::onDisable()
{

}

Object* Component::getParent() const
{
	return p_parent;
}

void Component::setActive(const bool activeState)
{
	// Need to change to tell the Scene to call these functions rather than the component itself
	if (activeState != m_enabled)
	{
		if (activeState)
		{
			onEnable();
		}
		else
		{
			onDisable();
		}
		m_enabled = activeState;
	}
}

bool Component::getActiveState() const
{
	return m_enabled;
}
