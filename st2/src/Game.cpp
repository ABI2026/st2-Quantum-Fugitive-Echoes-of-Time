#include "Game.h"

#include "Button.h"
#include "Eventsystem.h"
#include "LayerManager.h"
#include "Utils/Log.h"

void Game::update(std::shared_ptr<Eventsystem>& eventsystem, std::shared_ptr<LayerManager>& layer_manager, std::shared_ptr<Soundsystem>& soundsystem, sf::RenderWindow& window, double deltatime)
{
	if(eventsystem->get_key_action(sf::Keyboard::Key::Escape) == Eventsystem::action_pressed)
	{
		layer_manager->pop_layer();
		//layer_manager->push_layer(std::make_shared<PausenMenu>()); 
		return;
	}
}

void Game::render(sf::RenderWindow& window)
{

}

void Game::on_close()
{
	
}

LayerID Game::get_layer_id()
{
	return LayerID::game;
}
