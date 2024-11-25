#pragma once
#include "Layer.h"

class Buttons;

class OptionsMenu : public Layer
{
public:
	OptionsMenu();
	void update(std::shared_ptr<Eventsystem>& eventsystem, std::shared_ptr<LayerManager>& layer_manager, std::shared_ptr<Soundsystem>& soundsystem, sf::RenderWindow& window, double deltatime) override;
	void render(sf::RenderWindow& window) override;
	LayerID get_layer_id() override;
	void on_close() override;
private:
	int m_selected = -1;
	std::vector<std::shared_ptr<Buttons>> m_buttons;
	int m_windowselect = 0;
};

