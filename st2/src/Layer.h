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
	skill_selector
};


class Layer
{
public:
	Layer() = default;
	Layer& operator=(const Layer&) = default;
	Layer(const Layer&) = default;
	Layer& operator=(Layer&&) noexcept = default;
	Layer(Layer&&) noexcept = default;

	virtual ~Layer() = default;

	virtual void update([[maybe_unused]]std::shared_ptr<Eventsystem>& eventsystem, [[maybe_unused]] std::shared_ptr<LayerManager>& layer_manager,
		[[maybe_unused]] std::shared_ptr<Soundsystem>& soundsystem, [[maybe_unused]] sf::RenderWindow& window, [[maybe_unused]] double deltatime) {}

	virtual void render([[maybe_unused]] sf::RenderWindow& window) {}

	virtual void on_close() {}

	[[nodiscard]] virtual LayerID get_layer_id() = 0;
};

