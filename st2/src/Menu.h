#pragma once
#include <SFML/Graphics/RenderWindow.hpp>

#include "Button.h"
#include "Eventsystem.h"
#include "Layer.h"
#include "Utils/Log.h"

class Menu : public Layer
{
	int m_selected = -1;
	std::vector<std::shared_ptr<Button>> buttons;
public:

	Menu()
	{
		buttons.emplace_back(std::make_shared<Button>(sf::Vector2f{ 100.f,100.f }, sf::Vector2f{ 200.f,50.f }, "start"));
		buttons.emplace_back(std::make_shared<Button>(sf::Vector2f{ 100.f,200.f }, sf::Vector2f{ 200.f,50.f }, "optionen"));
		buttons.emplace_back(std::make_shared<Button>(sf::Vector2f{ 100.f,300.f }, sf::Vector2f{ 200.f,50.f }, "schlieﬂen"));
	}

	void update(std::shared_ptr<Eventsystem>& eventsystem) override
	{
		const sf::Vector2i mouse_pos = eventsystem->get_mouse_position();

		if (eventsystem->get_key_action(sf::Keyboard::Key::Down) == Eventsystem::action_pressed)
			m_selected = m_selected + 1 == static_cast<int>(buttons.size()) ? 0 : m_selected + 1;
		if (eventsystem->get_key_action(sf::Keyboard::Key::Up) == Eventsystem::action_pressed)
			m_selected = m_selected - 1 < 0 ? static_cast<int>(buttons.size()) - 1 : m_selected - 1;

		LOG_INFO("selected {}", m_selected);

		for(auto& button:buttons)
		{
			button->update(mouse_pos, eventsystem->get_mouse_button_action(sf::Mouse::Button::Left) == Eventsystem::action_released);
		}

		//mouse over button needs to be handled

	}

	void render(sf::RenderWindow& window) override
	{
		//render buttons waiting for button class to be finished
		for (auto& button : buttons)
		{
			button->draw(window);
		}
	}
};

