#pragma once
#include <SFML/Graphics.hpp>

#include "Layer.h"

class Game : public Layer
{
public:
	Game() = default;
	Game(int level_id);
	void update(std::shared_ptr<Eventsystem>& eventsystem, std::shared_ptr<LayerManager>& layer_manager, std::shared_ptr<Soundsystem>& soundsystem, sf::RenderWindow& window, double deltatime) override;

	void render(sf::RenderWindow& window) override;

	void on_close() override;

	[[nodiscard]] LayerID get_layer_id() override;
private:
	sf::View m_view;
};

