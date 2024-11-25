#include "Eventsystem.h"

#include <ranges>

#include "imgui-SFML.h"
#include "Utils/Log.h"

Eventsystem::Eventsystem(const sf::RenderWindow& window)
{
	m_window_size = window.getSize();
}

void Eventsystem::process_events(sf::RenderWindow& window, const sf::Event& event)
{
	if (event.type == sf::Event::LostFocus)
		m_focus = false;
	else if (event.type == sf::Event::GainedFocus)
		m_focus = true;

	if (!m_focus)
		return;

	switch (event.type)
	{
		//ENTSCHEIDEN OB ANDERE EVENTS AUCH GETRACT WERDEN SOLLTEN WIE RESIZE ETC.

	case sf::Event::Closed:
		window.close();
		break;
	case sf::Event::Resized: 
	{
		m_window_size.x = event.size.width;
		m_window_size.y = event.size.height;
		sf::View new_view = window.getView();
		new_view.setCenter(m_window_size.x/2.f, m_window_size.y/2.f);
		new_view.setSize(static_cast<float>(m_window_size.x), static_cast<float>(m_window_size.y));
		window.setView(new_view);
	}
		break;
		//case sf::Event::TextEntered: break;
	case sf::Event::KeyPressed: //fallthrough
	case sf::Event::KeyReleased:
	{
		const sf::Keyboard::Key key = event.key.code;
		if (!m_key_states.contains(key))
			break;
		const bool down = (event.type == sf::Event::KeyPressed);
		const action action = (down) ? (m_key_states[key] ? action_repeat : action_pressed) : (action_released);
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
		const action action = (down) ? (m_mouse_button_states[button] ? action_repeat : action_pressed) : (action_released);
		m_mouse_button_actions[button] = action;
		m_mouse_button_states[button] = down;
		if (m_mouse_button_events_callbacks[button])
			m_mouse_button_events_callbacks[button](button, action);
	}
	break;
	case sf::Event::MouseMoved:
	{
		const sf::Vector2f new_mouse_position = window.mapPixelToCoords({event.mouseMove.x, event.mouseMove.y});
		m_mouse_offset = m_mouse_position - new_mouse_position;
		m_mouse_position = new_mouse_position;
	}
		break;
	//case sf::Event::MouseEntered: break;
	case sf::Event::MouseLeft:
		m_mouse_offset = {0.f,0.f};
		m_mouse_position = {-1.f,-1.f};
		break;
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

void Eventsystem::handle_updates(sf::RenderWindow& window)
{
	for (auto& action : m_key_actions | std::views::values)
	{
		action = action_none;
	}

	for (auto& action : m_mouse_button_actions | std::views::values)
	{
		action = action_none;
	}

	m_mouse_offset = { 0,0 };

	sf::Event event{};
	while (window.pollEvent(event))
	{
		ImGui::SFML::ProcessEvent(window, event);
		process_events(window, event);
	}

	if (window.getSize() != m_window_size)
	{
		m_window_size = window.getSize();
	}
}

void Eventsystem::add_key_listener(sf::Keyboard::Key key)
{
	m_key_states.insert_or_assign(key, sf::Keyboard::isKeyPressed(key));
	if (!m_key_actions.contains(key))
		m_key_actions.insert({ key,action_none });
	if (!m_key_events_callbacks.contains(key))
		m_key_events_callbacks.insert({ key,{} });

}

bool Eventsystem::get_key_state(const sf::Keyboard::Key key) const
{
	return m_key_states.at(key);
}

Eventsystem::action Eventsystem::get_key_action(const sf::Keyboard::Key key) const
{
	return m_key_actions.at(key);
}

void Eventsystem::set_key_callback(sf::Keyboard::Key key, const std::function<void(sf::Keyboard::Key, action)>& callback)
{
	m_key_events_callbacks.insert_or_assign(key, callback);
	if (!m_key_actions.contains(key))
		m_key_actions.insert({ key,action_none });
	if (!m_key_states.contains(key))
		m_key_states.insert({ key,sf::Keyboard::isKeyPressed(key) });
}

void Eventsystem::add_mouse_button_listener(sf::Mouse::Button button)
{
	m_mouse_button_states.insert_or_assign(button, sf::Mouse::isButtonPressed(button));
	if (!m_mouse_button_actions.contains(button))
		m_mouse_button_actions.insert({ button,action_none });
	if (!m_mouse_button_events_callbacks.contains(button))
		m_mouse_button_events_callbacks.insert({ button,{} });
}

bool Eventsystem::get_mouse_button_state(const sf::Mouse::Button button) const
{
	return m_mouse_button_states.at(button);

}

Eventsystem::action Eventsystem::get_mouse_button_action(const sf::Mouse::Button button) const
{
	return m_mouse_button_actions.at(button);

}

void Eventsystem::set_mouse_button_callback(sf::Mouse::Button button, const std::function<void(sf::Mouse::Button, action)>& callback)
{
	m_mouse_button_events_callbacks.insert_or_assign(button, callback);
	if (!m_mouse_button_actions.contains(button))
		m_mouse_button_actions.insert({ button,action_none });
	if (!m_mouse_button_states.contains(button))
		m_mouse_button_states.insert({ button,sf::Mouse::isButtonPressed(button) });
}

sf::Vector2f Eventsystem::get_mouse_position() const
{
	return m_mouse_position;
}

sf::Vector2f Eventsystem::get_mouse_offset() const
{
	return m_mouse_offset;
}

sf::Vector2u Eventsystem::get_window_size() const
{
	return m_window_size;
}

bool Eventsystem::has_focus() const
{
	return m_focus;
}
