#include "Eventsystem.h"

Eventsystem::Eventsystem()
{
}

void Eventsystem::update_events(const sf::Window& window, const sf::Event& event)
{
	if (event.type == sf::Event::LostFocus)
		focus = false;
	else if (event.type == sf::Event::GainedFocus)
		focus = true;

	if (!focus)
		return;
	switch (event.type)
	{
		//TODO: ENTSCHEIDEN OB ANDERE EVENTS AUCH GETRACT WERDEN SOLLTEN WIE RESIZE ETC.
		//case sf::Event::Closed: break;
		//case sf::Event::Resized: break;
		//case sf::Event::TextEntered: break;
	case sf::Event::KeyPressed: //fallthrough
	case sf::Event::KeyReleased:
	{
		const sf::Keyboard::Key key = event.key.code;
		if (!m_key_states.contains(key))
			break;
		const bool down = (event.type == sf::Event::KeyPressed);
		const int8_t action = (down) ? (m_key_states[key] ? 2 : 1) : (0);
		m_key_actions[key] = action;
		m_key_states[key] = down;
		if (m_key_events_callbacks[key])
			m_key_events_callbacks[key](key, action);

	}
	break;
	//case sf::Event::MouseWheelMoved: break;
	//case sf::Event::MouseWheelScrolled: break;
	case sf::Event::MouseButtonPressed://fallthrough
	case sf::Event::MouseButtonReleased:
	{
		const sf::Mouse::Button button = event.mouseButton.button;
		if (!m_mouse_button_states.contains(button))
			break;
		const bool down = (event.type == sf::Event::MouseButtonPressed);
		const int8_t action = (down) ? (m_mouse_button_states[button] ? 2 : 1) : (0);
		m_mouse_button_actions[button] = action;
		m_mouse_button_states[button] = down;
		if (m_mouse_button_events_callbacks[button])
			m_mouse_button_events_callbacks[button](button, action);
	}
	break;
	//case sf::Event::MouseMoved: break;
	//case sf::Event::MouseEntered: break;
	//case sf::Event::MouseLeft: break;
	//case sf::Event::JoystickButtonPressed: break;
	//case sf::Event::JoystickButtonReleased: break;
	//case sf::Event::JoystickMoved: break;
	//case sf::Event::JoystickConnected: break;
	//case sf::Event::JoystickDisconnected: break;
	//case sf::Event::TouchBegan: break;
	//case sf::Event::TouchMoved: break;
	//case sf::Event::TouchEnded: break;
	//case sf::Event::SensorChanged: break;

	default: break;
	}
}

void Eventsystem::add_key_listener(sf::Keyboard::Key key)
{
	m_key_states.insert_or_assign(key, sf::Keyboard::isKeyPressed(key));
	if (!m_key_actions.contains(key))
		m_key_actions.insert({ key,0 });
	if (!m_key_events_callbacks.contains(key))
		m_key_events_callbacks.insert({ key,{} });

}

bool Eventsystem::get_key_state(sf::Keyboard::Key key)
{
	return m_key_states[key];
}

bool Eventsystem::get_key_action(sf::Keyboard::Key key)
{
	return m_key_actions[key];
}

void Eventsystem::set_key_callback(sf::Keyboard::Key key, const std::function<void(sf::Keyboard::Key, int)>& callback)
{
	m_key_events_callbacks.insert_or_assign(key, callback);
	if (!m_key_actions.contains(key))
		m_key_actions.insert({ key,0 });
	if (!m_key_states.contains(key))
		m_key_states.insert({ key,sf::Keyboard::isKeyPressed(key) });
}

void Eventsystem::add_mouse_button_listener(sf::Mouse::Button button)
{
	m_mouse_button_states.insert_or_assign(button, sf::Mouse::isButtonPressed(button));
	if (!m_mouse_button_actions.contains(button))
		m_mouse_button_actions.insert({button,-1});
	if (!m_mouse_button_events_callbacks.contains(button))
		m_mouse_button_events_callbacks.insert({ button,{} });
}

bool Eventsystem::get_mouse_button_state(sf::Mouse::Button button)
{
	return m_mouse_button_states[button];

}

bool Eventsystem::get_mouse_button_action(sf::Mouse::Button button)
{
	return m_mouse_button_actions[button];

}

void Eventsystem::set_mouse_button_callback(sf::Mouse::Button button, const std::function<void(sf::Mouse::Button, int)>& callback)
{
	m_mouse_button_events_callbacks.insert_or_assign(button, callback);
	if (!m_mouse_button_actions.contains(button))
		m_mouse_button_actions.insert({ button,-1 });
	if (!m_mouse_button_states.contains(button))
		m_mouse_button_states.insert({ button,sf::Mouse::isButtonPressed(button) });
}
