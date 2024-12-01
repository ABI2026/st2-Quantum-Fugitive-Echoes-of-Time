#include "Game.h"

#include <glm/glm.hpp>

#include "Buttons/Button.h"
#include "Eventsystem.h"
#include "imgui.h"
#include "LayerManager.h"
#include "PauseMenu.h"
#include "Utils/Log.h"

Game::Game(int i_level_id, std::shared_ptr<Soundsystem>& soundsystem)
{
	soundsystem->set_music_indices({ 1,2,3 });
	m_background_texture.loadFromFile("Resources/Images/hintergrund.jpg");
}

void Game::update(std::shared_ptr<Eventsystem>& eventsystem, std::shared_ptr<LayerManager>& layer_manager, std::shared_ptr<Soundsystem>& soundsystem, sf::RenderWindow& window, const double deltatime)
{
	static sf::Vector2f position = sf::Vector2f(eventsystem->get_window_size());

	glm::vec2 movement = { 0.f,0.f };

	if (eventsystem->get_key_state(sf::Keyboard::Key::W))
		movement.y -= 1;
	if (eventsystem->get_key_state(sf::Keyboard::Key::A))
		movement.x -= 1;
	if (eventsystem->get_key_state(sf::Keyboard::Key::S))
		movement.y += 1;
	if (eventsystem->get_key_state(sf::Keyboard::Key::D))
		movement.x += 1;
	ImGui::Begin("Debug");
	ImGui::Text("fps: %f", 1.0 / deltatime);
	if (movement != glm::vec2{ 0.f,0.f })
	{
		movement = glm::normalize(movement);

		ImGui::Text("direction: x:%f y:%f", movement.x, movement.y);
		movement = movement * 600.f * static_cast<float>(deltatime);
		ImGui::Text("movement: x:%f y:%f", movement.x, movement.y);

		glm::vec2 new_pos = { position.x + movement.x, position.y + movement.y };
		new_pos = clamp(new_pos, { 50.f,50.f }, { m_background_texture.getSize().x - 50.f,m_background_texture.getSize().y - 50.f });
		position = { new_pos.x, new_pos.y };

	}
	else
	{
		ImGui::Text("direction: x:0.000000 y:0.000000");
		ImGui::Text("movement: x:0.000000 y:0.000000");
	}

	ImGui::Text("position: x:%f y:%f", position.x, position.y);
	ImGui::End();


	const sf::View backup = window.getView();
	m_view = backup;
	m_view.setCenter(position);
	window.setView(m_view);


	window.setView(backup);

	if (eventsystem->get_key_action(sf::Keyboard::Key::Escape) == Eventsystem::action_pressed || !eventsystem->has_focus())
	{
		layer_manager->push_layer(std::make_shared<PauseMenu>(layer_manager->get_top()));
		return;
	}

	if (eventsystem->get_key_action(sf::Keyboard::Key::Q) == Eventsystem::action_pressed)
	{
		soundsystem->set_music_indices({ 0,1,2 });
		layer_manager->pop_layer();
	}

	//float blend = 1-pow(lerp_speed,deltatime);
}

void Game::render(sf::RenderWindow& window)
{
	const sf::View backup = window.getView();
	m_view.setSize(sf::Vector2f(window.getSize()));
	window.setView(m_view);
	static sf::RectangleShape shape(sf::Vector2f(m_background_texture.getSize()));

	shape.setPosition(0.f,0.f);
	shape.setFillColor(sf::Color::Cyan);
	shape.setTexture(&m_background_texture);
	sf::RectangleShape shape2({ 100.f,100.f });
	shape2.setOrigin(50.f,50.f);
	shape2.setPosition(window.getView().getCenter());
	shape2.setTexture(&m_background_texture);
	//shape2.setFillColor(sf::Color::Yellow);

	window.draw(shape);
	window.draw(shape2);

	window.setView(backup);
}

void Game::on_close()
{
	LOG_INFO("closed game");
}

LayerID Game::get_layer_id()
{
	return LayerID::game;
}
