#include "OptionsMenu.h"

#include "Buttons/Button.h"
#include "Eventsystem.h"
#include "LayerManager.h"
#include "Utils/Log.h"
#include "Utils/Soundsystem.h"

OptionsMenu::OptionsMenu()
{
	for (int i = 0; i < 16; i++)
	{
		m_buttons.emplace_back(std::make_shared<Button>());
	}

	m_buttons[0]->set_layout(std::make_shared<TextLayout>("back", sf::Vector2f{ 260.f,145.f }, sf::Vector2f{ 200.f,50.f }, 0.f));
	m_buttons[0]->set_behaviour(std::make_shared<PopLayer>());


	m_buttons[1]->set_layout(std::make_shared<TextLayout>("<", sf::Vector2f{ 210.f,215.f }, sf::Vector2f{ 50.f,50.f }, 0.f));
	m_buttons[1]->set_behaviour(std::make_shared<EmptyBehaviour>());

	m_buttons[2]->set_layout(std::make_shared<TextLayout>("WindowMode", sf::Vector2f{ 260.f,215.f }, sf::Vector2f{ 200.f,50.f }, 0.f));
	m_buttons[2]->set_behaviour(std::make_shared<EmptyBehaviour>());

	m_buttons[3]->set_layout(std::make_shared<TextLayout>(">", sf::Vector2f{ 460.f,215.f }, sf::Vector2f{ 50.f,50.f }, 0.f));
	m_buttons[3]->set_behaviour(std::make_shared<EmptyBehaviour>());


	m_buttons[4]->set_layout(std::make_shared<TextLayout>("<", sf::Vector2f{ 210.f,285.f }, sf::Vector2f{ 50.f,50.f }, 0.f));
	m_buttons[4]->set_behaviour(std::make_shared<IncrementVolume>("global", -5.f));

	m_buttons[5]->set_layout(std::make_shared<TextLayout>("global: 100", sf::Vector2f{ 260.f,285.f }, sf::Vector2f{ 200.f,50.f }, 0.f));
	m_buttons[5]->set_behaviour(std::make_shared<EmptyBehaviour>());

	m_buttons[6]->set_layout(std::make_shared<TextLayout>(">", sf::Vector2f{ 460.f,285.f }, sf::Vector2f{ 50.f,50.f }, 0.f));
	m_buttons[6]->set_behaviour(std::make_shared<IncrementVolume>("global", 5.f));


	m_buttons[7]->set_layout(std::make_shared<TextLayout>("<", sf::Vector2f{ 210.f,285.f + 50.f }, sf::Vector2f{ 50.f,50.f }, 0.f));
	m_buttons[7]->set_behaviour(std::make_shared<IncrementVolume>("music", -5.f));

	m_buttons[8]->set_layout(std::make_shared<TextLayout>("music: 100", sf::Vector2f{ 260.f,285.f + 50.f }, sf::Vector2f{ 200.f,50.f }, 0.f));
	m_buttons[8]->set_behaviour(std::make_shared<EmptyBehaviour>());

	m_buttons[9]->set_layout(std::make_shared<TextLayout>(">", sf::Vector2f{ 460.f,285.f + 50.f }, sf::Vector2f{ 50.f,50.f }, 0.f));
	m_buttons[9]->set_behaviour(std::make_shared<IncrementVolume>("music", 5.f));


	m_buttons[10]->set_layout(std::make_shared<TextLayout>("<", sf::Vector2f{ 210.f,285.f + 100.f }, sf::Vector2f{ 50.f,50.f }, 0.f));
	m_buttons[10]->set_behaviour(std::make_shared<IncrementVolume>("ui_sounds", -5.f));

	m_buttons[11]->set_layout(std::make_shared<TextLayout>("ui: 100", sf::Vector2f{ 260.f,285.f + 100.f }, sf::Vector2f{ 200.f,50.f }, 0.f));
	m_buttons[11]->set_behaviour(std::make_shared<EmptyBehaviour>());

	m_buttons[12]->set_layout(std::make_shared<TextLayout>(">", sf::Vector2f{ 460.f,285.f + 100.f }, sf::Vector2f{ 50.f,50.f }, 0.f));
	m_buttons[12]->set_behaviour(std::make_shared<IncrementVolume>("ui_sounds", 5.f));


	m_buttons[13]->set_layout(std::make_shared<TextLayout>("<", sf::Vector2f{ 210.f,285.f + 150.f }, sf::Vector2f{ 50.f,50.f }, 0.f));
	m_buttons[13]->set_behaviour(std::make_shared<IncrementVolume>("player_sounds", -5.f));

	m_buttons[14]->set_layout(std::make_shared<TextLayout>("player: 100", sf::Vector2f{ 260.f,285.f + 150.f }, sf::Vector2f{ 200.f,50.f }, 0.f));
	m_buttons[14]->set_behaviour(std::make_shared<EmptyBehaviour>());

	m_buttons[15]->set_layout(std::make_shared<TextLayout>(">", sf::Vector2f{ 460.f,285.f + 150.f }, sf::Vector2f{ 50.f,50.f }, 0.f));
	m_buttons[15]->set_behaviour(std::make_shared<IncrementVolume>("player_sounds", 5.f));

	if (!background.loadFromFile(std::filesystem::path("Resources/Images/Hintergrund.png")))
		LOG_ERROR("Failed to load image");

}

void OptionsMenu::update(std::shared_ptr<Eventsystem>& eventsystem, std::shared_ptr<LayerManager>& layer_manager,
	std::shared_ptr<Soundsystem>& soundsystem, sf::RenderWindow& window, [[maybe_unused]] double deltatime)
{
	const sf::Vector2f mouse_pos = eventsystem->get_mouse_position();

	constexpr float padding_y = 20.0f;
	constexpr float button_size_y = 50.f;
	constexpr float buttons_in_y = 6.f;
	constexpr float total_height = buttons_in_y * button_size_y + (buttons_in_y - 1.f) * padding_y;
	const float start_y = (static_cast<float>(eventsystem->get_window_size().y) - total_height) / 2.f;

	constexpr float padding_x = 10.f;
	constexpr float total_width = 50.f * 2 + 200.f + 2 * padding_x;
	constexpr std::array button_offsets_x{0.f,50.f + padding_x, 50.f + 200.f + padding_x *2};
	const float start_x = (static_cast<float>(eventsystem->get_window_size().x) - total_width) / 2.f;

	m_buttons[5]->set_text(std::format("global: {}", soundsystem->get_volumes().at("global")));
	m_buttons[8]->set_text(std::format("music: {}", soundsystem->get_volumes().at("music")));
	m_buttons[11]->set_text(std::format("ui: {}", soundsystem->get_volumes().at("ui_sounds")));
	m_buttons[14]->set_text(std::format("player: {}", soundsystem->get_volumes().at("player_sounds")));

	for (uint8_t i = 0; i < m_buttons.size(); i++) 
	{
		if (i == 0)
			m_buttons[i]->set_position(sf::Vector2f{ static_cast<float>(eventsystem->get_window_size().x) / 2.f - 100.f,start_y});
		else 
			m_buttons[i]->set_position(sf::Vector2f{ start_x + (button_offsets_x[(i + 2) % 3]),start_y + std::floor((static_cast<float>(i) + 2.f) / 3.f) * (button_size_y + padding_y)});
		m_buttons[i]->update(mouse_pos,eventsystem->get_mouse_button_action(sf::Mouse::Button::Left) == Eventsystem::action_released);
		if (!m_buttons[i]->is_clicked())
			continue;
		if (m_buttons[i]->on_click(layer_manager, soundsystem, window))
		{
			m_selected = -1;
			return;
		}
	}

	if (eventsystem->get_key_action(sf::Keyboard::Key::Escape) == Eventsystem::action_pressed)
	{
		layer_manager->pop_layer();
		return;
	}


	if (m_buttons[1]->is_clicked() == true)
	{
		if (m_windowselect < 1)
			m_windowselect++;
		else
			m_windowselect = 0;
	}
	else if (m_buttons[3]->is_clicked() == true)
	{
		if (m_windowselect > 0)
			m_windowselect--;
		else
			m_windowselect = 1;
	}


	if (m_buttons[1]->is_clicked() || m_buttons[3]->is_clicked())
	{
		if (m_windowselect == 0)
		{
			m_buttons[2]->set_text("WindowMode");
			window.create(sf::VideoMode({ 720, 480 }), "window", sf::Style::Default);
		}
		else
		{
			m_buttons[2]->set_text("Fullscreen");
			window.create(sf::VideoMode({1920, 1080}), "window", sf::State::Fullscreen);
		}
		window.setFramerateLimit(60);
		LOG_INFO("windowselect: {}", m_windowselect);
	}


	if (eventsystem->get_key_action(sf::Keyboard::Key::Down) == Eventsystem::action_pressed)
		m_selected_y = m_selected_y + 1 == static_cast<int>(buttons_in_y) ? 0 : m_selected_y + 1;
	if (eventsystem->get_key_action(sf::Keyboard::Key::Up) == Eventsystem::action_pressed)
		m_selected_y = m_selected_y - 1 < 0 ? static_cast<int>(buttons_in_y)-1 : m_selected_y - 1;

	if (eventsystem->get_key_action(sf::Keyboard::Key::Right) == Eventsystem::action_pressed)
		m_selected_x = m_selected_x + 1 == 3 ? 0 : m_selected_x + 1;
	if (eventsystem->get_key_action(sf::Keyboard::Key::Left) == Eventsystem::action_pressed)
		m_selected_x = m_selected_x - 1 < 0 ? 2 : m_selected_x - 1;

	if ((m_selected_x == -1) ^ (m_selected_y == -1))
	{
		m_selected_x = m_selected_x == -1 ? 1 : m_selected_x;
		m_selected_y = m_selected_y == -1 ? 0 : m_selected_y;
	}

	if (eventsystem->get_mouse_button_action(sf::Mouse::Button::Left) == Eventsystem::action_pressed)
	{
		m_selected_x = -1;
		m_selected_y = -1;
	}

	if (m_selected_x == -1 || m_selected_y == -1)
	{
		return;
	}

	const int selected = m_selected_y == 0 ? 0 : m_selected_x + 1 + (m_selected_y-1) * 3;

	m_buttons[selected]->set_is_hovered(true);

	if (eventsystem->get_key_action(sf::Keyboard::Key::Enter) == Eventsystem::action_released)
	{
		if (selected == 1 || selected == 3) 
		{
			if (selected == 1)
			{
				if (m_windowselect < 1)
					m_windowselect++;
				else
					m_windowselect = 0;
			}
			else
			{
				if (m_windowselect > 0)
					m_windowselect--;
				else
					m_windowselect = 1;
			}

			if (m_windowselect == 0)
			{
				m_buttons[2]->set_text("WindowMode");
				window.create(sf::VideoMode({720, 480}), "window", sf::Style::Default);
			}
			else
			{
				m_buttons[2]->set_text("Fullscreen");
				window.create(sf::VideoMode({1920, 1080}), "window", sf::State::Fullscreen);
			}
			window.setFramerateLimit(60);
			LOG_INFO("windowselect: {}", m_windowselect);
			return;
		}

		if (m_buttons[selected]->on_click(layer_manager, soundsystem, window))
		{
			m_selected_x = -1;
			m_selected_y = -1;
			return;
		}
	}

}

void OptionsMenu::render(sf::RenderWindow& window)
{
	sf::RectangleShape s((sf::Vector2f)window.getSize());
	s.setTexture(&background);
	window.draw(s);

	for (const auto& button : m_buttons)
	{
		button->render(window);
	}
}

void OptionsMenu::on_close()
{
	LOG_INFO("closed OptionsMenu");
}

LayerID OptionsMenu::get_layer_id()
{
	return LayerID::options;
}
