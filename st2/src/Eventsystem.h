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
	//TODO: ENTSCHEIDEN OB STATES UND ACTIONS ZUSAMMENGEPACKT WERDEN SOLL MOMENTAN HABEN WIR KEIN SPEICHER PROBLEM ALSO WUERDE ICH DAS ERSTMAL SO LASSEN
	std::unordered_map<sf::Keyboard::Key,bool> m_key_states;
	std::unordered_map<sf::Keyboard::Key,int8_t> m_key_actions; 
	std::unordered_map<sf::Keyboard::Key,std::function<void(sf::Keyboard::Key,int)>> m_key_events_callbacks;

	std::unordered_map<sf::Mouse::Button,bool> m_mouse_button_states;
	std::unordered_map<sf::Mouse::Button, int8_t> m_mouse_button_actions;
	std::unordered_map<sf::Mouse::Button,std::function<void(sf::Mouse::Button, int)>> m_mouse_button_events_callbacks;
public:
	Eventsystem();

	void update_events(const sf::Window& window,const sf::Event& event);

	void add_key_listener(sf::Keyboard::Key key);
	bool get_key_state(sf::Keyboard::Key key);
	bool get_key_action(sf::Keyboard::Key key);
	void set_key_callback(sf::Keyboard::Key key, const std::function<void(sf::Keyboard::Key, int)>& callback);

	void add_mouse_button_listener(sf::Mouse::Button button);
	bool get_mouse_button_state(sf::Mouse::Button button);
	bool get_mouse_button_action(sf::Mouse::Button button);
	void set_mouse_button_callback(sf::Mouse::Button button, const std::function<void(sf::Mouse::Button, int)>& callback);
};

