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
	if (event.is<sf::Event::FocusLost>())
		m_focus = false;
	else if (event.is<sf::Event::FocusGained>() )
		m_focus = true;

	if (!m_focus)
		return;


		//ENTSCHEIDEN OB ANDERE EVENTS AUCH GETRACT WERDEN SOLLTEN WIE RESIZE ETC.

	if (event.is<sf::Event::Closed>())
	{
		window.close();
	}
	else if(const auto resize = event.getIf<sf::Event::Resized>())
	{
		m_window_size.x = resize->size.x;
		m_window_size.y = resize->size.y;
		sf::View new_view = window.getView();
		new_view.setCenter({ static_cast<float>(m_window_size.x) / 2.f, static_cast<float>(m_window_size.y) / 2.f });
		new_view.setSize({ static_cast<float>(m_window_size.x), static_cast<float>(m_window_size.y) });
		window.setView(new_view);
	}
	else if (const auto key_pressed = event.getIf<sf::Event::KeyPressed>())
	{
		const sf::Keyboard::Key key = key_pressed->code;
		if (m_key_states.contains(key))
		{
			constexpr bool down = true;
			const action action = m_key_states[key] ? action_repeat : action_pressed;
			m_key_actions[key] = action;
			m_key_states[key] = down;
			if (m_key_events_callbacks[key])
				m_key_events_callbacks[key](key, action);
		}
	}
	else if (const auto key_released = event.getIf<sf::Event::KeyReleased>())
	{
		const sf::Keyboard::Key key = key_released->code;
		if (m_key_states.contains(key))
		{
			constexpr bool down = false;
			constexpr action action = action_released;
			m_key_actions[key] = action;
			m_key_states[key] = down;
			if (m_key_events_callbacks[key])
				m_key_events_callbacks[key](key, action);
		}
	}
	else if (const auto mouse_button_pressed = event.getIf<sf::Event::MouseButtonPressed>())
	{
		const sf::Mouse::Button button = mouse_button_pressed->button;
		if (m_mouse_button_states.contains(button))
		{
			constexpr bool down = true;
			const action action = m_mouse_button_states[button] ? action_repeat : action_pressed;
			m_mouse_button_actions[button] = action;
			m_mouse_button_states[button] = down;
			if (m_mouse_button_events_callbacks[button])
				m_mouse_button_events_callbacks[button](button, action);
		}
	}
	else if (const auto mouse_button_released = event.getIf<sf::Event::MouseButtonReleased>())
	{
		const sf::Mouse::Button button = mouse_button_released->button;
		if (m_mouse_button_states.contains(button))
		{
			constexpr bool down = false;
			constexpr action action = action_released;
			m_mouse_button_actions[button] = action;
			m_mouse_button_states[button] = down;
			if (m_mouse_button_events_callbacks[button])
				m_mouse_button_events_callbacks[button](button, action);
		}
	}
	else if(const auto MouseMoved = event.getIf<sf::Event::MouseMoved>())
	{
		const sf::Vector2f new_mouse_position = window.mapPixelToCoords({ MouseMoved->position.x, MouseMoved->position.y});
		m_mouse_offset = m_mouse_position - new_mouse_position;
		m_mouse_position = new_mouse_position;
	}
	else if (event.is<sf::Event::MouseLeft>()) 
	{
		m_mouse_offset = { 0.f,0.f };
		m_mouse_position = { -1.f,-1.f };
	}

}

void Eventsystem::handle_updates(sf::RenderWindow& window)
{
	for (auto& action : m_key_actions | std::views::values)
	{
		if(action != action_repeat)
			action = action_none;
	}

	for (auto& action : m_mouse_button_actions | std::views::values)
	{
		if (action != action_repeat)
			action = action_none;
	}

	m_mouse_offset = { 0,0 };

	while (const auto event = window.pollEvent())
	{
		ImGui::SFML::ProcessEvent(window, *event);
		process_events(window, *event);
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
	const auto data = m_key_states.find(key);
	if(data == m_key_states.end())
	{
#ifndef DIST
		LOG_ERROR("button:{} doesn't exist in m_key_states", static_cast<int>(key));
#endif
		return false;
	}
	return data->second;
}

Eventsystem::action Eventsystem::get_key_action(const sf::Keyboard::Key key) const
{
	const auto data = m_key_actions.find(key);
	if (data == m_key_actions.end()) 
	{
#ifndef DIST
		LOG_ERROR("key:{} doesn't exist in m_key_actions", static_cast<int>(key));
#endif
		return action_none;
	}
	return data->second;
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
	const auto data = m_mouse_button_states.find(button);
	if (data == m_mouse_button_states.end()) 
	{
#ifndef DIST
		LOG_ERROR("button:{} doesn't exist in m_mouse_button_states", static_cast<int>(button));
#endif
		return action_none;
	}
	return data->second;
}

Eventsystem::action Eventsystem::get_mouse_button_action(const sf::Mouse::Button button) const
{
	const auto data = m_mouse_button_actions.find(button);
	if (data == m_mouse_button_actions.end()) 
	{
#ifndef DIST
		LOG_ERROR("button:{} doesn't exist in m_mouse_button_actions", static_cast<int>(button));
#endif
		return action_none;
	}
	return data->second;
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
