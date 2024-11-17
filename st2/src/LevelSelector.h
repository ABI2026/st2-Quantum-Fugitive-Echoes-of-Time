#pragma once
#include <SFML/Graphics/RenderWindow.hpp>

#include "Layer.h"

class Button;

class LevelSelector : public Layer
{
	int m_selected = -1;

	std::vector<std::shared_ptr<Button>> m_buttons;

	bool button_action(int selected, std::shared_ptr<LayerManager>& layer_manager);
public:
	LevelSelector();

	void update(std::shared_ptr<Eventsystem>& eventsystem, std::shared_ptr<LayerManager>& layer_manager, 
		std::shared_ptr<Soundsystem>& soundsystem, sf::RenderWindow& window, double deltatime) override;

	void render(sf::RenderWindow& window) override;

	void on_close() override;

	LayerID get_layer_id() override;
};

