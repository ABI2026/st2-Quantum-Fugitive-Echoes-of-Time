#pragma once
#include <memory>

#include "Layer.h"
#include "Utils/Soundsystem.h"


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
	virtual bool on_click(std::shared_ptr<LayerManager>& layer_manager,
		std::shared_ptr<Soundsystem>& soundsystem,
		sf::RenderWindow& window
		) = 0;
};

class EmptyLayer : public ButtonBehaviour
{
public:
	bool on_click(std::shared_ptr<LayerManager>& layer_manager,
		std::shared_ptr<Soundsystem>& soundsystem,
		sf::RenderWindow& window) override
	{
		return false;
	}
};

class GoBackTillLayer : public ButtonBehaviour
{
public:
	GoBackTillLayer(LayerID id): m_layer_id(id){}
	bool on_click(std::shared_ptr<LayerManager>& layer_manager,
		std::shared_ptr<Soundsystem>& soundsystem, 
		sf::RenderWindow& window) override;
private:
	LayerID m_layer_id;
};

class PopLayer : public ButtonBehaviour
{
public:
	bool on_click(std::shared_ptr<LayerManager>& layer_manager,
		std::shared_ptr<Soundsystem>& soundsystem, 
		sf::RenderWindow& window) override;
};

class AddLevelSelectLayer : public ButtonBehaviour
{
public:
	bool on_click(std::shared_ptr<LayerManager>& layer_manager, 
		std::shared_ptr<Soundsystem>& soundsystem,
		sf::RenderWindow& window) override;
};

class AddGameLayer : public ButtonBehaviour
{
public:
	AddGameLayer(int i_level_id):m_level_id(i_level_id){}
	bool on_click(std::shared_ptr<LayerManager>& layer_manager, 
		std::shared_ptr<Soundsystem>& soundsystem,
		sf::RenderWindow& window) override;
private:
	int m_level_id{ 0 };
};

