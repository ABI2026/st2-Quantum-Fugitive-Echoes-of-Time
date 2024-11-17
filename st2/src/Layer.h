#pragma once
#include <memory>
#include <SFML/Graphics/RenderWindow.hpp>

class Soundsystem;
class LayerManager;
class Eventsystem;

enum class LayerID
{
    main_menu,
    game,
    pause_menu,
    level_selector,
    options,
};


class Layer
{
public:
	virtual ~Layer() = default;

	virtual void update(std::shared_ptr<Eventsystem>& eventsystem, std::shared_ptr<LayerManager>& layer_manager,
		std::shared_ptr<Soundsystem>& soundsystem, sf::RenderWindow& window, double deltatime) {}

	virtual void render(sf::RenderWindow& window) {}

	virtual void on_close() {}

	virtual LayerID get_layer_id() = 0;
};

