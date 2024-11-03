#include "Eventsystem.h"

Eventsystem::Eventsystem()
{
}

void Eventsystem::Update_events(const sf::Window& window, const sf::Event& event)
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
		const bool down = (event.type == sf::Event::KeyPressed);
		const sf::Keyboard::Key key = event.key.code;
		if (!key_events.contains(key))
			break;
		const int action = (down) ? (key_events[key] ? 2 : 1) : (0);
		key_events[key] = down;
		if (key_events_callbacks[key])
			key_events_callbacks[key](key, action);

	}
	break;
	//case sf::Event::MouseWheelMoved: break;
	//case sf::Event::MouseWheelScrolled: break;
	case sf::Event::MouseButtonPressed://fallthrough
	case sf::Event::MouseButtonReleased:
	{
		const bool down = (event.type == sf::Event::MouseButtonPressed);
		const sf::Mouse::Button button = event.mouseButton.button;
		if (!mouse_events.contains(button))
			break;
		const int action = (down) ? (mouse_events[button] ? 2 : 1) : (0);

		mouse_events[button] = down;
		if (mouse_events_callbacks[button])
			mouse_events_callbacks[button](button, action);
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
	key_events.insert_or_assign(key, sf::Keyboard::isKeyPressed(key));
	if (!key_events_callbacks.contains(key))
		key_events_callbacks.insert({ key,{} });

}

bool Eventsystem::get_key_event(sf::Keyboard::Key key)
{
	return key_events[key];
}

void Eventsystem::set_key_callback(sf::Keyboard::Key key, const std::function<void(sf::Keyboard::Key, int)>& callback)
{
	key_events_callbacks.insert_or_assign(key, callback);
	if (!key_events.contains(key))
		key_events.insert({ key,sf::Keyboard::isKeyPressed(key) });
}

void Eventsystem::add_mouse_listener(sf::Mouse::Button button)
{
	mouse_events.insert_or_assign(button, sf::Mouse::isButtonPressed(button));
	if (!mouse_events_callbacks.contains(button))
		mouse_events_callbacks.insert({ button,{} });
}

bool Eventsystem::get_mouse_event(sf::Mouse::Button button)
{
	return mouse_events[button];

}

void Eventsystem::set_mouse_callback(sf::Mouse::Button button, const std::function<void(sf::Mouse::Button, int)>& callback)
{
	mouse_events_callbacks.insert_or_assign(button, callback);
	if (!mouse_events.contains(button))
		mouse_events.insert({ button,sf::Mouse::isButtonPressed(button) });
}
