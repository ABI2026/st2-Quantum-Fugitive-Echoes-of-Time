#pragma once
#include <SFML/Graphics/RenderWindow.hpp>

#include "Layer.h"

class Button;

class Menu : public Layer
{
	int m_selected = -1;

	std::vector<std::shared_ptr<Button>> buttons;

	bool button_action(int selected, std::shared_ptr<LayerManager>& layer_manager);
public:
	Menu();

	void update(std::shared_ptr<Eventsystem>& eventsystem, std::shared_ptr<LayerManager>& layer_manager, double deltatime) override;

	void render(sf::RenderWindow& window) override;

	void on_close() override{}

	LayerID get_layer_id() override;
};

