#pragma once
#include "Game.h"
#include "Layer.h"

class Buttons;

class PauseMenu : public Layer
{
	int m_selected = -1;

	std::vector<std::shared_ptr<Buttons>> m_buttons;

	std::shared_ptr<Layer> background_layer;
public:
	PauseMenu(const std::shared_ptr<Layer>& background_layer);

	void update(std::shared_ptr<Eventsystem>& eventsystem, std::shared_ptr<LayerManager>& layer_manager, std::shared_ptr<Soundsystem>& soundsystem, sf::RenderWindow& window, double deltatime) override;

	void render(sf::RenderWindow& window) override;

	void on_close() override;

	LayerID get_layer_id() override;

};
