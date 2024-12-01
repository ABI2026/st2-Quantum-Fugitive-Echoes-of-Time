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
	ButtonBehaviour() = default;
	ButtonBehaviour& operator=(const ButtonBehaviour&) = default;
	ButtonBehaviour(const ButtonBehaviour&) = default;
	ButtonBehaviour& operator=(ButtonBehaviour&&) noexcept = default;
	ButtonBehaviour(ButtonBehaviour&&) noexcept = default;
	virtual ~ButtonBehaviour() = default;
	[[nodiscard]] virtual bool on_click(std::shared_ptr<LayerManager>& layer_manager,
		std::shared_ptr<Soundsystem>& soundsystem,
		sf::RenderWindow& window
		) = 0;
};

class EmptyBehaviour : public ButtonBehaviour
{
public:
	[[nodiscard]] bool on_click(std::shared_ptr<LayerManager>& layer_manager,
		std::shared_ptr<Soundsystem>& soundsystem,
		sf::RenderWindow& window) override
	{
		return false;
	}
};

class GoBackTillLayer : public ButtonBehaviour
{
public:
	GoBackTillLayer(const LayerID id): m_layer_id(id){}
	[[nodiscard]] bool on_click(std::shared_ptr<LayerManager>& layer_manager,
		std::shared_ptr<Soundsystem>& soundsystem, 
		sf::RenderWindow& window) override;
private:
	LayerID m_layer_id;
};

class PopLayer : public ButtonBehaviour
{
public:
	[[nodiscard]] bool on_click(std::shared_ptr<LayerManager>& layer_manager,
		std::shared_ptr<Soundsystem>& soundsystem, 
		sf::RenderWindow& window) override;
};

class AddLevelSelectLayer : public ButtonBehaviour
{
public:
	[[nodiscard]] bool on_click(std::shared_ptr<LayerManager>& layer_manager,
		std::shared_ptr<Soundsystem>& soundsystem,
		sf::RenderWindow& window) override;
};

class AddGameLayer : public ButtonBehaviour
{
public:
	[[nodiscard]] AddGameLayer(const int i_level_id):m_level_id(i_level_id){}
	bool on_click(std::shared_ptr<LayerManager>& layer_manager, 
		std::shared_ptr<Soundsystem>& soundsystem,
		sf::RenderWindow& window) override;
private:
	int m_level_id{ 0 };
};

class AddOptionsMenu : public ButtonBehaviour
{
public:
	[[nodiscard]] bool on_click(std::shared_ptr<LayerManager>& layer_manager, std::shared_ptr<Soundsystem>& soundsystem, sf::RenderWindow& window) override;
};

class IncrementVolume : public ButtonBehaviour
{
public:
	IncrementVolume(const std::string& i_group_id,float i_volume): m_group_id(i_group_id), m_volume(i_volume){}
	[[nodiscard]] bool on_click(std::shared_ptr<LayerManager>& layer_manager, std::shared_ptr<Soundsystem>& soundsystem, sf::RenderWindow& window) override;
private:
	std::string m_group_id;
	float m_volume{5.f};
};