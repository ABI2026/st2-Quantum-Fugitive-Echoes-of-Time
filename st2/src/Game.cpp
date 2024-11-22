#include "Game.h"

#include <glm/glm.hpp>

#include "Button.h"
#include "Eventsystem.h"
#include "LayerManager.h"
#include "PauseMenu.h"
#include "Utils/Log.h"

Game::Game(int level_id)
{
}

void Game::update(std::shared_ptr<Eventsystem>& eventsystem, std::shared_ptr<LayerManager>& layer_manager, std::shared_ptr<Soundsystem>& soundsystem, sf::RenderWindow& window, double deltatime)
{
	if(eventsystem->get_key_action(sf::Keyboard::Key::Escape) == Eventsystem::action_pressed || !eventsystem->has_focus())
	{
		layer_manager->push_layer(std::make_shared<PauseMenu>(layer_manager->get_top())); 
		return;
	}

	if(eventsystem->get_key_action(sf::Keyboard::Key::Q) == Eventsystem::action_pressed)
	{
		layer_manager->pop_layer(); 
		return;
	}
	//float blend = 1-pow(lerp_speed,deltatime);
}

void Game::render(sf::RenderWindow& window)
{
	sf::RectangleShape shape(sf::Vector2f(window.getSize()));
	shape.setPosition(0, 0);
	shape.setFillColor(sf::Color::Cyan);
	window.draw(shape);
}

void Game::on_close()
{
	LOG_INFO("closed game");
}

LayerID Game::get_layer_id()
{
	return LayerID::game;
}
