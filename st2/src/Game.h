#pragma once
#include <SFML/Graphics.hpp>

#include "Layer.h"
#include "Level.h"

class Expbar;
class Player;
class Enemy;
class EnemyManager;
class ProjectileManager;

class Game : public Layer
{
public:
	Game(int level_id, std::shared_ptr<Soundsystem>& soundsystem);

	void update(std::shared_ptr<Eventsystem>& eventsystem, std::shared_ptr<LayerManager>& layer_manager, std::shared_ptr<Soundsystem>& soundsystem, sf::RenderWindow& window, double deltatime) override;

	void render(sf::RenderWindow& window) override;

	void on_close() override;

	[[nodiscard]] LayerID get_layer_id() override;
private:
	sf::View m_view;
	sf::Texture m_background_texture;
	std::shared_ptr<Level> m_level;


	std::shared_ptr<Player> m_player;
	std::shared_ptr<EnemyManager> m_enemy_manager;
	std::shared_ptr<ProjectileManager> m_projectile_manager;

	std::shared_ptr<Expbar> m_expbar;
	sf::Texture m_sprite_sheet;
};


