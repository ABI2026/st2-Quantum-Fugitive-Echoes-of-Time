#pragma once
#include <SFML/Graphics/RenderWindow.hpp>

#include "Button.h"
#include "Eventsystem.h"
#include "Layer.h"
#include "Utils/Log.h"

class Menu : public Layer
{
	int m_selected = 0;
	std::vector<Button> buttons;
public:

	Menu()
	{
		buttons.resize(3);
	}

	void update(std::shared_ptr<Eventsystem>& eventsystem) override
	{
		const sf::Vector2f mouse_pos = eventsystem->get_mouse_position();
		if (eventsystem->get_key_action(sf::Keyboard::Key::Down) == Eventsystem::action_pressed)
			m_selected += 1;
		if (eventsystem->get_key_action(sf::Keyboard::Key::Up) == Eventsystem::action_pressed)
			m_selected -= 1;

		if (m_selected < 0)
			m_selected = buttons.size() - 1;
		else if (m_selected >= buttons.size())
			m_selected = 0;

		LOG_INFO("selcted {}", m_selected);
		//mouse over button needs to be handled

	}

	void render(sf::RenderWindow& window) override
	{
		//render buttons waiting for button class to be finished
	}
};

