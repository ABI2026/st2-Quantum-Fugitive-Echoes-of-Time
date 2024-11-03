#pragma once
#include <functional>
#include <unordered_map>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Window.hpp>

class Eventsystem
{
	bool focus = true;
	std::unordered_map<sf::Keyboard::Key,bool> key_events;
	std::unordered_map<sf::Keyboard::Key,std::function<void(sf::Keyboard::Key,int)>> key_events_callbacks;
	std::unordered_map<sf::Mouse::Button,bool> mouse_events;
	std::unordered_map<sf::Mouse::Button,std::function<void(sf::Mouse::Button, int)>> mouse_events_callbacks;
public:
	Eventsystem();

	void Update_events(const sf::Window& window,const sf::Event& event);

	void add_key_listener(sf::Keyboard::Key key);
	bool get_key_event(sf::Keyboard::Key key);
	void set_key_callback(sf::Keyboard::Key key, const std::function<void(sf::Keyboard::Key, int)>& callback);

	void add_mouse_listener(sf::Mouse::Button button);
	bool get_mouse_event(sf::Mouse::Button button);
	void set_mouse_callback(sf::Mouse::Button button, const std::function<void(sf::Mouse::Button, int)>& callback);
};

