#pragma once
#include <SFML/Graphics.hpp>

#include "Layer.h"

class Game : public Layer
{
public:
	void update(std::shared_ptr<Eventsystem>& eventsystem, std::shared_ptr<LayerManager>& layer_manager, std::shared_ptr<Soundsystem>& soundsystem, sf::RenderWindow& window, double deltatime) override;

	void render(sf::RenderWindow& window) override;

	void on_close() override;

	LayerID get_layer_id() override;
};

