#pragma once
#include "Layer.h"

class OptionsMenu : public Layer
{
public:
	void update(std::shared_ptr<Eventsystem>& eventsystem, std::shared_ptr<LayerManager>& layer_manager, std::shared_ptr<Soundsystem>& soundsystem, sf::RenderWindow& window, double deltatime) override;
	LayerID get_layer_id() override;
};

