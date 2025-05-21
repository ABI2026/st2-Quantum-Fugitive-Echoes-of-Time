#pragma once
#include <vector>
#include <string>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

//#include "Eventsystem.h"
//#include "LayerManager.h"
#include "Utils/Soundsystem.h"
#include "Utils/Log.h"
#include "Layer.h"

class Button;
class Eventsystem;
class LayerManager;
class Player;

class SkillSelector : public Layer
{
private:
	std::vector<std::shared_ptr<Button>> m_buttons;
	sf::Texture m_texture;
	int m_selected = -1;
	std::shared_ptr<Layer> m_previous_layer;
	std::shared_ptr<Player> m_player;
public:
	SkillSelector(std::shared_ptr<Layer> i_previous_layer, std::shared_ptr<Player> i_player);

	void update(std::shared_ptr<Eventsystem>& eventsystem, std::shared_ptr<LayerManager>& layer_manager,
		std::shared_ptr<Soundsystem>& soundsystem, sf::RenderWindow& window, double deltatime) override;

	void render(sf::RenderWindow& window) override;

	//void on_close() override;

	[[nodiscard]] LayerID get_layer_id() override {
		return LayerID::skill_selector;
	}
	//void update(std::shared_ptr<Eventsystem>& eventsystem, std::shared_ptr<Soundsystem>& soundsystem, double deltatime);
	//void draw(sf::RenderWindow& window);
};

