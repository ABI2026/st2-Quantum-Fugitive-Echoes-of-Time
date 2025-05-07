#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "Layer.h"

class Button;

class LevelSelector : public Layer
{
public:
	LevelSelector();

	void update(std::shared_ptr<Eventsystem>& eventsystem, std::shared_ptr<LayerManager>& layer_manager, 
		std::shared_ptr<Soundsystem>& soundsystem, sf::RenderWindow& window, double deltatime) override;

	void render(sf::RenderWindow& window) override;

	void on_close() override;

	[[nodiscard]] LayerID get_layer_id() override;
private:
	int m_selected_x = -1;
	int m_selected_y = -1;
	sf::Texture background;

	std::vector<std::shared_ptr<Button>> m_buttons;
};

