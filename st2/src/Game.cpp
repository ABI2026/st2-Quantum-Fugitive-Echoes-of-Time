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
	m_player.update(eventsystem, soundsystem, deltatime);

	const sf::View backup = window.getView();
	m_view = backup;
	m_view.setCenter(m_player.getPosition());
	//window.setView(m_view);


	//window.setView(backup);

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

	window.draw(shape);
	m_player.draw(window);

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
