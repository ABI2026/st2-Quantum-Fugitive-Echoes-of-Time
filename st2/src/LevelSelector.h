#pragma once
#include <SFML/Graphics/RenderWindow.hpp>

#include "Layer.h"

class Buttons;

class LevelSelector : public Layer
{
	int m_selected_x = -1;
	int m_selected_y = -1;

	std::vector<std::shared_ptr<Buttons>> m_buttons;
public:
	LevelSelector();

	void update(std::shared_ptr<Eventsystem>& eventsystem, std::shared_ptr<LayerManager>& layer_manager, 
		std::shared_ptr<Soundsystem>& soundsystem, sf::RenderWindow& window, double deltatime) override;

	void render(sf::RenderWindow& window) override;

	void on_close() override;

	LayerID get_layer_id() override;
};

