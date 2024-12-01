#include "Menu.h"

#include "Buttons/Button.h"
#include "Eventsystem.h"
#include "LayerManager.h"
#include "Utils/Log.h"
#include "Utils/Soundsystem.h"

Menu::Menu()
{
	m_buttons.emplace_back(std::make_shared<Button>());
	m_buttons.emplace_back(std::make_shared<Button>());
	m_buttons.emplace_back(std::make_shared<Button>());

	m_buttons[0]->set_layout(std::make_shared<TextLayout>("start", sf::Vector2f{ 260.f,145.f }, sf::Vector2f{ 200.f,50.f }));
	m_buttons[0]->set_behaviour(std::make_shared<AddLevelSelectLayer>());

	m_buttons[1]->set_layout(std::make_shared<TextLayout>("optionen", sf::Vector2f{ 260.f,215.f }, sf::Vector2f{ 200.f,50.f }));
	m_buttons[1]->set_behaviour(std::make_shared<AddOptionsMenu>());


	m_buttons[2]->set_layout(std::make_shared<TextLayout>("schließen", sf::Vector2f{ 260.f,285.f }, sf::Vector2f{ 200.f,50.f }));
	m_buttons[2]->set_behaviour(std::make_shared<PopLayer>());
}

void Menu::update(std::shared_ptr<Eventsystem>& eventsystem, std::shared_ptr<LayerManager>& layer_manager, 
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
		m_buttons[i]->set_position({ static_cast<float>(eventsystem->get_window_size().x) / 2.f - 100.f,start_y + static_cast<float>(i) * (50.f + padding) });
		m_buttons[i]->update(mouse_pos, eventsystem->get_mouse_button_action(sf::Mouse::Button::Left) == Eventsystem::action_released);
		if (!m_buttons[i]->is_clicked())
			continue;
		if (m_buttons[i]->on_click(layer_manager, soundsystem, window)) 
		{
			m_selected = -1;
			return;
		}
	}


	if (eventsystem->get_key_action(sf::Keyboard::Key::Down) == Eventsystem::action_pressed)
		m_selected = m_selected + 1 == static_cast<int>(m_buttons.size()) ? 0 : m_selected + 1;

	if (eventsystem->get_key_action(sf::Keyboard::Key::Up) == Eventsystem::action_pressed)
		m_selected = m_selected - 1 < 0 ? static_cast<int>(m_buttons.size()) - 1 : m_selected - 1;

	if (eventsystem->get_mouse_button_action(sf::Mouse::Button::Left) == Eventsystem::action_pressed)
		m_selected = -1;

	if (m_selected == -1)
		return;

	m_buttons[m_selected]->set_is_hovered(true);

	if (eventsystem->get_key_action(sf::Keyboard::Key::Enter) == Eventsystem::action_released)
	{
		if(m_buttons[m_selected]->on_click(layer_manager,soundsystem,window))
		{
			m_selected = -1;
			return;
		}
	}
}

void Menu::render(sf::RenderWindow& window)
{
	for (const auto& button : m_buttons)
	{
		button->render(window);
	}
}

void Menu::on_close()
{
	LOG_INFO("closed menu");
}

LayerID Menu::get_layer_id()
{
	return LayerID::main_menu;
}
