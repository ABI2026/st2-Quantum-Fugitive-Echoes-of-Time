#pragma once
#include <SFML/Graphics/RenderWindow.hpp>

#include "Button.h"
#include "Eventsystem.h"
#include "Layer.h"
#include "Utils/Log.h"

class Menu : public Layer
{
	int m_selected = -1;
	bool execute = false;
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
		const sf::Vector2f mouse_pos = eventsystem->get_mouse_position();

		if (eventsystem->get_key_action(sf::Keyboard::Key::Enter) == Eventsystem::action_pressed) {
			execute = true;
		}

		if (eventsystem->get_key_action(sf::Keyboard::Key::Down) == Eventsystem::action_pressed) {
			m_selected = m_selected + 1 == static_cast<int>(buttons.size()) ? 0 : m_selected + 1;
			buttons[m_selected]->setColors(sf::Color(200, 200, 200), sf::Color(200, 200, 200), sf::Color(150, 150, 150));
		}
		else if (eventsystem->get_key_action(sf::Keyboard::Key::Up) == Eventsystem::action_pressed) {
			m_selected = m_selected - 1 < 0 ? static_cast<int>(buttons.size()) - 1 : m_selected - 1;
			buttons[m_selected]->setColors(sf::Color(200, 200, 200), sf::Color(200, 200, 200), sf::Color(150, 150, 150));
		}
		else {
			for (auto& button : buttons) {
				if (buttons[m_selected] != button) {
					button->setColors(sf::Color::White, sf::Color(200, 200, 200), sf::Color(150, 150, 150));
				}
			}
		}

		LOG_INFO("selected {}", m_selected);

		for(auto& button:buttons)
		{
			button->update((sf::Vector2i)mouse_pos, eventsystem->get_mouse_button_action(sf::Mouse::Button::Left) == Eventsystem::action_released);
		}

		if (buttons[0]->isClicked(sf::Vector2i(mouse_pos))) {
			m_selected = 0;
			execute = true;
		}
		else if (buttons[1]->isClicked(sf::Vector2i(mouse_pos))) {
			m_selected = 1;
			execute = true;
		}
		else if (buttons[2]->isClicked(sf::Vector2i(mouse_pos))) {
			m_selected = 2;
			execute = true;
		}
	}

	void render(sf::RenderWindow& window) override
	{
		//render buttons waiting for button class to be finished
		if (execute == true && m_selected == 0) {
			execute = false;
			LOG_INFO("Start");
		}
		else if (execute == true && m_selected == 1) {
			execute = false;
			LOG_INFO("Optionen");
		}
		else if (execute == true && m_selected == 2) {
			execute = false;
			window.close();
		}

		for (auto& button : buttons)
		{
			button->draw(window);
		}
	}
};

