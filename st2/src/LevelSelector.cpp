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
	case 0:
		layer_manager->push_layer(std::make_shared<Game>(1));
		break;
	case 1:
		layer_manager->push_layer(std::make_shared<Game>(2));
		break;
	case 2:
		layer_manager->pop_layer();
		break;
	default:
		return false;
		break;
	}
	m_selected = -1;
	return true;
}

LevelSelector::LevelSelector()
{
	m_buttons.emplace_back(std::make_shared<Button>(sf::Vector2f{ 100.f,100.f }, sf::Vector2f{ 200.f,50.f }, "level 1", sf::Color::Yellow));
	m_buttons.emplace_back(std::make_shared<Button>(sf::Vector2f{ 100.f,200.f }, sf::Vector2f{ 200.f,50.f }, "level 2", sf::Color::Yellow));
	m_buttons.emplace_back(std::make_shared<Button>(sf::Vector2f{ 100.f,300.f }, sf::Vector2f{ 200.f,50.f }, "zurrück", sf::Color::Yellow));
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
	constexpr float button_size = 50.f;
	const float total_height = static_cast<float>(m_buttons.size()) * button_size + (static_cast<float>(m_buttons.size()) - 1.f) * padding;
	const float start_y = (static_cast<float>(eventsystem->get_window_size().y) - total_height) / 2.f;

	for (uint8_t i = 0; i < m_buttons.size(); ++i)
	{
		m_buttons[i]->setPosition({ static_cast<float>(eventsystem->get_window_size().x) / 2.f - 100.f,start_y + static_cast<float>(i) * (50.f + padding) });
		m_buttons[i]->update(static_cast<sf::Vector2i>(mouse_pos), eventsystem->get_mouse_button_action(sf::Mouse::Button::Left) == Eventsystem::action_released);
		if (!m_buttons[i]->isClicked())
			continue;

		if (button_action(i, layer_manager))
			return;
	}


	if (eventsystem->get_key_action(sf::Keyboard::Key::Down) == Eventsystem::action_pressed)
		m_selected = m_selected + 1 == static_cast<int>(m_buttons.size()) ? 0 : m_selected + 1;
	if (eventsystem->get_key_action(sf::Keyboard::Key::Up) == Eventsystem::action_pressed)
		m_selected = m_selected - 1 < 0 ? static_cast<int>(m_buttons.size()) - 1 : m_selected - 1;

	if (eventsystem->get_mouse_button_action(sf::Mouse::Button::Left) == Eventsystem::action_pressed)
		m_selected = -1;

	if (m_selected != -1)
	{
		m_buttons[m_selected]->set_is_hovered(true);
	}

	if (eventsystem->get_key_action(sf::Keyboard::Key::Enter) == Eventsystem::action_released)
	{
		if (button_action(m_selected, layer_manager))
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
