#include "Menu.h"

#include "Buttons/Button.h"
#include "Eventsystem.h"
#include "LayerManager.h"
#include "Utils/Log.h"
#include "Utils/Soundsystem.h"

Menu::Menu()
{

	if (!button_1.loadFromFile(std::filesystem::path("Resources/Images/Button_1.png")))
	{
		LOG_ERROR("error loading image");
	}
	if (!button_2.loadFromFile(std::filesystem::path("Resources/Images/Button_2.png")))
	{
		LOG_ERROR("error loading image");
	}
	m_buttons.emplace_back(std::make_shared<Button>());
	m_buttons.emplace_back(std::make_shared<Button>());
	m_buttons.emplace_back(std::make_shared<Button>());

	m_buttons[0]->set_layout(std::make_shared<TextImageLayout>("start", sf::Vector2f{ 260.f,145.f }, sf::Vector2f{ 400.f,100.f },&button_1,&button_2, &button_2, 0.f));
	m_buttons[0]->set_behaviour(std::make_shared<AddLevelSelectLayer>());

	m_buttons[1]->set_layout(std::make_shared<TextImageLayout>("optionen", sf::Vector2f{ 260.f,215.f }, sf::Vector2f{ 400.f,100.f }, &button_1, &button_2, &button_2, 0.f));
	m_buttons[1]->set_behaviour(std::make_shared<AddOptionsMenu>());


	m_buttons[2]->set_layout(std::make_shared<TextImageLayout>("schließen", sf::Vector2f{ 260.f,285.f }, sf::Vector2f{ 400.f,100.f }, &button_1, &button_2, &button_2, 0.f));
	m_buttons[2]->set_behaviour(std::make_shared<PopLayer>());

	if (!background.loadFromFile(std::filesystem::path("Resources/Images/Hintergrund.png")))
	{
		LOG_ERROR("Failed to load image");
	}

}

void Menu::update(std::shared_ptr<Eventsystem>& eventsystem, std::shared_ptr<LayerManager>& layer_manager, 
	std::shared_ptr<Soundsystem>& soundsystem, sf::RenderWindow& window, [[maybe_unused]] double deltatime)
{
	if (eventsystem->get_key_action(sf::Keyboard::Key::Escape) == Eventsystem::action_pressed)
	{
		layer_manager->pop_layer();
		soundsystem->play_sound("ui_sounds", 2);
		return;
	}

	const sf::Vector2f mouse_pos = eventsystem->get_mouse_position();

	constexpr float padding = 20.0f;
	constexpr float button_size = 100.f;
	const float total_height = static_cast<float>(m_buttons.size()) * button_size + (static_cast<float>(m_buttons.size()) - 1.f) * padding;
	const float start_y = (static_cast<float>(eventsystem->get_window_size().y) - total_height) / 2.f;
	
	for (uint8_t i = 0; i < m_buttons.size(); ++i)
	{
		m_buttons[i]->set_position({ static_cast<float>(eventsystem->get_window_size().x) / 2.f - 200.f,start_y + static_cast<float>(i) * (button_size + padding) });
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
	sf::RectangleShape s((sf::Vector2f)window.getSize());
	s.setTexture(&background);
	window.draw(s);

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
