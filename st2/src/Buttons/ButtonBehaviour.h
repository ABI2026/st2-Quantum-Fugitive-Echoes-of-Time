#pragma once
#include <memory>
namespace sf
{
	class RenderWindow;
}
class LayerManager;
class Soundsystem;

class ButtonBehaviour
{
public:
	virtual ~ButtonBehaviour() = default;
	virtual void on_click(std::shared_ptr<LayerManager>& layer_manager,
		std::shared_ptr<Soundsystem>& soundsystem,
		sf::RenderWindow& window
		) = 0;
};

