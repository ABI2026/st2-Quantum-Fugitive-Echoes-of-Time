#pragma once
#include <functional>
#include <unordered_map>

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

class Eventsystem
{
public:
	typedef enum action : int8_t
	{
		action_none,
		action_released,
		action_pressed,
		action_repeat 
	} action;

	Eventsystem(const sf::RenderWindow& window);

	//VLLT SOLL EVENTSYSTEM AUCH ALLES ANDERE MACHEN ALSO POLLEVENTS ETC.
	void process_events(sf::RenderWindow& window,const sf::Event& event);

	void handle_updates(sf::RenderWindow& window);

	void add_key_listener(sf::Keyboard::Key key);

	[[nodiscard]] bool get_key_state(sf::Keyboard::Key key) const;

	[[nodiscard]] action get_key_action(sf::Keyboard::Key key) const;

	void set_key_callback(sf::Keyboard::Key key, const std::function<void(sf::Keyboard::Key, action)>& callback);

	void add_mouse_button_listener(sf::Mouse::Button button);

	[[nodiscard]] bool get_mouse_button_state(sf::Mouse::Button button) const;

	[[nodiscard]] action get_mouse_button_action(sf::Mouse::Button button) const;

	void set_mouse_button_callback(sf::Mouse::Button button, const std::function<void(sf::Mouse::Button, action)>& callback);


	[[nodiscard]] sf::Vector2f get_mouse_position() const;

	[[nodiscard]] sf::Vector2f get_mouse_offset() const;

	[[nodiscard]] sf::Vector2u get_window_size() const;

	[[nodiscard]] bool has_focus() const;

private:
	bool m_focus = true;

	std::unordered_map<sf::Keyboard::Key, bool> m_key_states;
	std::unordered_map<sf::Keyboard::Key, action> m_key_actions;
	std::unordered_map<sf::Keyboard::Key, std::function<void(sf::Keyboard::Key, action)>> m_key_events_callbacks;

	std::unordered_map<sf::Mouse::Button, bool> m_mouse_button_states;
	std::unordered_map<sf::Mouse::Button, action> m_mouse_button_actions;
	std::unordered_map<sf::Mouse::Button, std::function<void(sf::Mouse::Button, action)>> m_mouse_button_events_callbacks;

	sf::Vector2f m_mouse_position;
	sf::Vector2f m_mouse_offset;
	sf::Vector2u m_window_size;
};

