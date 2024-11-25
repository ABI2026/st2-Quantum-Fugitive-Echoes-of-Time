#include "OptionsMenu.h"

#include "Buttons/Button.h"
#include "Eventsystem.h"
#include "LayerManager.h"
#include "Utils/Log.h"
#include "Utils/Soundsystem.h"

void OptionsMenu::update(std::shared_ptr<Eventsystem>& eventsystem, std::shared_ptr<LayerManager>& layer_manager,
	std::shared_ptr<Soundsystem>& soundsystem, sf::RenderWindow& window, double deltatime)
{
	sf::Vector2f mouse_pos = eventsystem->get_mouse_position();

	if (eventsystem->get_key_action(sf::Keyboard::Key::Escape) == Eventsystem::action_pressed || m_buttons[0]->is_clicked() == true)
	{
		layer_manager->pop_layer();
		return;
	}
	else if (m_buttons[2]->is_clicked() == true) {
		if (m_windowselect < 1)
			m_windowselect++;
		else
			m_windowselect = 0;
		if (m_windowselect == 0) {
			m_buttons[1]->set_text("windowmode");
			window.create(sf::VideoMode(720, 480), "window", sf::Style::Default);
		}
		else {
			m_buttons[1]->set_text("fullscreen");
			window.create(sf::VideoMode(1920, 1080), "window", sf::Style::Fullscreen);
		}
		
	}
	else if (m_buttons[3]->is_clicked() == true) {
		if (m_windowselect > 0) 
			m_windowselect--;
		else
			m_windowselect = 1;
		if (m_windowselect == 0) {
			m_buttons[1]->set_text("windowmode");
			window.create(sf::VideoMode(720, 480), "window", sf::Style::Default);
		}
		else {
			m_buttons[1]->set_text("fullscreen");
			window.create(sf::VideoMode(1920, 1080), "window", sf::Style::Fullscreen);
		}
		LOG_INFO("windowselect: " + m_windowselect);
	}
	/*else if (m_buttons[5]->is_clicked() == true) {
		for (int i = 0; i < soundsystem->get_group_size(); i++) {
			soundsystem->increament(5, i);
		}
		m_buttons[4]->set_text(soundsystem->get_volumes());
	}*/

	for (int i = 0; i < m_buttons.size(); i++) {
		m_buttons[i]->update(mouse_pos, eventsystem->get_mouse_button_action(sf::Mouse::Button::Left) == Eventsystem::action_released);
	}
}

LayerID OptionsMenu::get_layer_id()
{
	return LayerID::options;
}

OptionsMenu::OptionsMenu() {
	for (int i = 0; i < 7; i++) {
		m_buttons.emplace_back(std::make_shared<Buttons>());
	}

	m_buttons[0]->set_layout(std::make_shared<TextLayout>("back", sf::Vector2f{ 260.f,145.f }, sf::Vector2f{ 200.f,50.f }, sf::Color::White));

	m_buttons[1]->set_layout(std::make_shared<TextLayout>("windowmode", sf::Vector2f{260.f,215.f}, sf::Vector2f{200.f,50.f}, sf::Color::White));
	m_buttons[2]->set_layout(std::make_shared<TextLayout>("<", sf::Vector2f{ 210.f,215.f }, sf::Vector2f{ 50.f,50.f }, sf::Color::White));
	m_buttons[3]->set_layout(std::make_shared<TextLayout>(">", sf::Vector2f{ 460.f,215.f }, sf::Vector2f{ 50.f,50.f }, sf::Color::White));

	m_buttons[4]->set_layout(std::make_shared<TextLayout>("100", sf::Vector2f{ 260.f,285.f }, sf::Vector2f{ 200.f,50.f }, sf::Color::White));
	m_buttons[5]->set_layout(std::make_shared<TextLayout>("<", sf::Vector2f{ 210.f,285.f }, sf::Vector2f{ 50.f,50.f }, sf::Color::White));
	m_buttons[6]->set_layout(std::make_shared<TextLayout>(">", sf::Vector2f{ 460.f,285.f }, sf::Vector2f{ 50.f,50.f }, sf::Color::White));

}

void OptionsMenu::render(sf::RenderWindow& window) {

	for (const auto& button : m_buttons)
	{
		button->render(window);
	}
}

void OptionsMenu::on_close() {
	LOG_INFO("closed OptionsMenu");
}