/*
 * Written by: AwokenOwen
 * Last Updated: March 26th 2026
 *
 * ADD EVENT REMOVE FOR OBJECT FUNCTION
 */

#pragma once
#include <functional>
#include <vector>

using namespace std;

class Object;
/**
 * @brief Event class for all events needed
 *
 * @tparam R the types of information being passed by the event
 */
template<typename... R>
class Event
{
public:

	/**
	 * @brief Event Constructor
	 *
	 * @param owner Object pointer to owner of event
	 */
	explicit Event(Object* owner = nullptr);

	/**
	 * @brief Turns all functions into lambda functions so they can be stored as listeners
	 *
	 * @tparam T Class of member function
	 * @param object Pointer to the Object
	 * @param func The function getting added as a listener
	 */
	template<typename T>
	void add(T* object, void(T::* func)(R...));

	/**
	 * @brief Call the event with the necessary information to be passed to all Objects. Will cause error if caller is not equal to p_owner
	 *
	 * @param args The inputs determined by R required to call the Event
	 * @param caller pointer to the Object that calls the event
	 */
	void callEvent(R... args, const Object* caller = nullptr);

private:
	/**
	 * @brief Vector of lambda functions ready to be called on callEvent()
	 */
	vector<function<void(R...)>> m_functions;

	Object* p_owner;
};

template<typename ... R>
Event<R...>::Event(Object *owner) {
	p_owner = owner;
}

template<typename ...R>
template<typename T>
void Event<R...>::add(T* object, void(T::* func)(R...))
{
	function<void(R...)> lambda = [object, func](R... args)
	{
		(object->*func)(args...);
	};

	m_functions.push_back(lambda);
}

template<typename ...R>
 void Event<R...>::callEvent(R... args, const Object* caller)
{
	if (caller != p_owner && p_owner != nullptr) {
		// print error "Caller Object is required to be the Owner of the Event"
		return;
	}
	for (int i = 0; i < m_functions.size(); i++)
	{
		m_functions[i](args...);
	}
}