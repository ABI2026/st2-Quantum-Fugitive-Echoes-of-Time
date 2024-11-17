#include "Menu.h"

#include "Button.h"
#include "Eventsystem.h"
#include "LayerManager.h"
#include "Utils/Log.h"

bool Menu::button_action(int selected, std::shared_ptr<LayerManager>& layer_manager)
{
	switch (selected)
	{
	case 0:
		//layer_manager->push_layer(std::make_shared<LevelSelector>());
		break;
	case 1:
		//layer_manager->push_layer(std::make_shared<Options>());
		break;
	case 2:
		layer_manager->pop_layer();
		break;
	default:
		return false;
		break;
	}
	return true;
}

Menu::Menu()
{
	buttons.emplace_back(std::make_shared<Button>(sf::Vector2f{ 100.f,100.f }, sf::Vector2f{ 200.f,50.f }, "start"));
	buttons.emplace_back(std::make_shared<Button>(sf::Vector2f{ 100.f,200.f }, sf::Vector2f{ 200.f,50.f }, "optionen"));
	buttons.emplace_back(std::make_shared<Button>(sf::Vector2f{ 100.f,300.f }, sf::Vector2f{ 200.f,50.f }, "schlieﬂen"));
}

void Menu::update(std::shared_ptr<Eventsystem>& eventsystem, std::shared_ptr<LayerManager>& layer_manager, double deltatime)
{
	const sf::Vector2f mouse_pos = eventsystem->get_mouse_position();

	if (eventsystem->get_key_action(sf::Keyboard::Key::Down) == Eventsystem::action_pressed)
		m_selected = m_selected + 1 == static_cast<int>(buttons.size()) ? 0 : m_selected + 1;
	if (eventsystem->get_key_action(sf::Keyboard::Key::Up) == Eventsystem::action_pressed)
		m_selected = m_selected - 1 < 0 ? static_cast<int>(buttons.size()) - 1 : m_selected - 1;
	if(eventsystem->get_key_action(sf::Keyboard::Key::Up) == Eventsystem::action_released)
	{
		if (button_action(m_selected, layer_manager))
			return;
	}


	const float padding = 20.0f;
	const float total_height = 3 * 50.f + 2 * padding;
	const float start_y = (eventsystem->get_window_size().y - total_height) / 2.f;

	for(uint8_t i = 0; i < buttons.size(); ++i )
	{
		buttons[i]->setPosition({float(eventsystem->get_window_size().x / 2u) - 100.f,start_y + float(i*(50.f + padding))});
		buttons[i]->update(static_cast<sf::Vector2i>(mouse_pos), eventsystem->get_mouse_button_action(sf::Mouse::Button::Left) == Eventsystem::action_released);
		if(!buttons[i]->isClicked())
			continue;

		if (button_action(i, layer_manager))
			return;

	}
}

void Menu::render(sf::RenderWindow& window)
{
	for (auto& button : buttons)
	{
		button->draw(window);
	}
}

LayerID Menu::get_layer_id()
{
	return LayerID::main_menu;
}
