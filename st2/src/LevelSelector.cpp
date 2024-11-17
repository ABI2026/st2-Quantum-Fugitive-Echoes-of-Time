#include "LevelSelector.h"

#include "Button.h"
#include "Eventsystem.h"
#include "LayerManager.h"
#include "Game.h"
#include "Utils/Log.h"
#include "Utils/Soundsystem.h"

bool LevelSelector::button_action(const int selected, const std::shared_ptr<LayerManager>& layer_manager)
{
	switch (selected)
	{
	case 0: //fallthrough
	case 1:	//fallthrough
	case 2:	//fallthrough
	case 3:	//fallthrough
	case 4:	
		layer_manager->push_layer(std::make_shared<Game>(selected+1));
		break;
	case 5:
		layer_manager->push_layer(std::make_shared<Game>(-1));
		break;
	case 6:
		layer_manager->pop_layer();
		break;
	default:
		return false;
		break;
	}
	m_selected_x = -1;
	m_selected_y = -1;
	return true;
}

LevelSelector::LevelSelector()
{
	m_buttons.emplace_back(std::make_shared<Button>(sf::Vector2f{ 40.f,145.f }, sf::Vector2f{ 200.f,50.f }, "level 1", sf::Color::Yellow));
	m_buttons.emplace_back(std::make_shared<Button>(sf::Vector2f{ 260.f,145.f }, sf::Vector2f{ 200.f,50.f }, "level 2", sf::Color::Yellow));
	m_buttons.emplace_back(std::make_shared<Button>(sf::Vector2f{ 480.f,145.f }, sf::Vector2f{ 200.f,50.f }, "level 3", sf::Color::Yellow));
	m_buttons.emplace_back(std::make_shared<Button>(sf::Vector2f{ 40.f,215.f }, sf::Vector2f{ 200.f,50.f }, "level 4", sf::Color::Yellow));
	m_buttons.emplace_back(std::make_shared<Button>(sf::Vector2f{ 260.f,215.f }, sf::Vector2f{ 200.f,50.f }, "level 5", sf::Color::Yellow));
	m_buttons.emplace_back(std::make_shared<Button>(sf::Vector2f{ 480.f,215.f }, sf::Vector2f{ 200.f,50.f }, "endlos", sf::Color::Yellow));
	m_buttons.emplace_back(std::make_shared<Button>(sf::Vector2f{ 260.f,285.f }, sf::Vector2f{ 200.f,50.f }, "zurrück", sf::Color::Yellow));
}

void LevelSelector::update(std::shared_ptr<Eventsystem>& eventsystem, std::shared_ptr<LayerManager>& layer_manager,
	std::shared_ptr<Soundsystem>& soundsystem, sf::RenderWindow& window, double deltatime)
{
	if (eventsystem->get_key_action(sf::Keyboard::Key::Escape) == Eventsystem::action_pressed)
	{
		layer_manager->pop_layer();
		return;
	}

	const sf::Vector2f mouse_pos = eventsystem->get_mouse_position();

	constexpr float padding = 20.0f;
	constexpr float button_size_y = 50.f;
	constexpr float total_height = 3.f * button_size_y + 2.f * padding;
	const float start_y = (static_cast<float>(eventsystem->get_window_size().y) - total_height) / 2.f;
	constexpr float button_size_x = 200.f;
	constexpr float total_width = 3.f * button_size_x + 2.f * padding;
	const float start_x = (static_cast<float>(eventsystem->get_window_size().x) - total_width) / 2.f;

	for (uint8_t i = 0; i < m_buttons.size(); ++i)
	{
		m_buttons[i]->setPosition(
			{
			i == 6 ? start_x + (button_size_x + padding) : start_x + static_cast<float>(i % 3) * (button_size_x + padding),
			start_y + static_cast<float>(std::floor(i/3)) * (button_size_y + padding)
			});
		m_buttons[i]->update(static_cast<sf::Vector2i>(mouse_pos), eventsystem->get_mouse_button_action(sf::Mouse::Button::Left) == Eventsystem::action_released);
		if (!m_buttons[i]->isClicked())
			continue;

		if (button_action(i, layer_manager))
			return;
	}


	if (eventsystem->get_key_action(sf::Keyboard::Key::Down) == Eventsystem::action_pressed)
		m_selected_y = m_selected_y + 1 == 3 ? 0 : m_selected_y + 1;
	if (eventsystem->get_key_action(sf::Keyboard::Key::Up) == Eventsystem::action_pressed)
		m_selected_y = m_selected_y - 1 < 0 ? 2 : m_selected_y - 1;

	if (eventsystem->get_key_action(sf::Keyboard::Key::Right) == Eventsystem::action_pressed)
		m_selected_x = m_selected_x + 1 == 3 ? 0 : m_selected_x + 1;
	if (eventsystem->get_key_action(sf::Keyboard::Key::Left) == Eventsystem::action_pressed)
		m_selected_x = m_selected_x - 1 < 0 ? 2 : m_selected_x - 1;

	if ((m_selected_x == -1) ^ (m_selected_y == -1)) 
	{
		m_selected_x = m_selected_x == -1 ? 0 : m_selected_x;
		m_selected_y = m_selected_y == -1 ? 0 : m_selected_y;
	}

	if (eventsystem->get_mouse_button_action(sf::Mouse::Button::Left) == Eventsystem::action_pressed)
	{
		m_selected_x = -1;
		m_selected_y = -1;
	}

	const int selected = m_selected_y == 2 ? 6 : m_selected_x + m_selected_y * 3;


	if (m_selected_x != -1 && m_selected_y != -1)
	{
		m_buttons[selected]->set_is_hovered(true);
	}

	if (eventsystem->get_key_action(sf::Keyboard::Key::Enter) == Eventsystem::action_released)
	{
		if (button_action(selected, layer_manager))
			return;
	}
}


void LevelSelector::render(sf::RenderWindow& window)
{
	for (const auto& button : m_buttons)
	{
		button->draw(window);
	}
}

void LevelSelector::on_close()
{
	LOG_INFO("closed LevelSelector");

}

LayerID LevelSelector::get_layer_id()
{
	return LayerID::level_selector;
}
