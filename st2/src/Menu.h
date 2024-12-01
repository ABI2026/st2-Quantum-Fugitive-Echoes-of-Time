#pragma once
#include <SFML/Graphics/RenderWindow.hpp>

#include "Layer.h"

class Button;

class Menu : public Layer
{
public:
	Menu();

	void update(std::shared_ptr<Eventsystem>& eventsystem, std::shared_ptr<LayerManager>& layer_manager, std::shared_ptr<Soundsystem>& soundsystem, sf::RenderWindow& window, double deltatime) override;

	void render(sf::RenderWindow& window) override;

	void on_close() override;

	[[nodiscard]] LayerID get_layer_id() override;
private:
	int m_selected = -1;

	std::vector<std::shared_ptr<Button>> m_buttons;
};

