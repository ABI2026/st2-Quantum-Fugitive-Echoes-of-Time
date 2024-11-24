#include "OptionsMenu.h"

#include "Buttons/Button.h"
#include "Eventsystem.h"
#include "LayerManager.h"
#include "Utils/Log.h"
#include "Utils/Soundsystem.h"

void OptionsMenu::update(std::shared_ptr<Eventsystem>& eventsystem, std::shared_ptr<LayerManager>& layer_manager,
	std::shared_ptr<Soundsystem>& soundsystem, sf::RenderWindow& window, double deltatime)
{
	if (eventsystem->get_key_action(sf::Keyboard::Key::Escape) == Eventsystem::action_pressed)
	{
		layer_manager->pop_layer();
		return;
	}
}

LayerID OptionsMenu::get_layer_id()
{
	return LayerID::options;
}
