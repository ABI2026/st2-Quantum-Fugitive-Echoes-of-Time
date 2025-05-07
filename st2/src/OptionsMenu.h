#pragma once
#include <SFML/Graphics/Texture.hpp>

#include "Layer.h"

class Button;

class OptionsMenu : public Layer
{
public:
	OptionsMenu();
	void update(std::shared_ptr<Eventsystem>& eventsystem, std::shared_ptr<LayerManager>& layer_manager, std::shared_ptr<Soundsystem>& soundsystem, sf::RenderWindow& window, double deltatime) override;
	void render(sf::RenderWindow& window) override;
	[[nodiscard]] LayerID get_layer_id() override;
	void on_close() override;
private:
	int m_selected = -1;
	std::vector<std::shared_ptr<Button>> m_buttons;
	int m_windowselect = 0;
	int m_selected_y{-1};
	int m_selected_x{-1};
	sf::Texture background;

};

