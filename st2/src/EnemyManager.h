#pragma once
#include <memory>
#include <vector>
#include <SFML/Graphics/Texture.hpp>

#include "Player.h"
class Enemy;
class EnemyManager
{
	std::vector<std::shared_ptr<Enemy>> m_enemies;
	std::vector<sf::Texture> m_textures;
public:
	EnemyManager();
	void spawn_enemy(Player* player);
	void update(std::shared_ptr<Eventsystem>& eventsystem, std::shared_ptr<Soundsystem>& soundsystem, double deltatime, Player* player);
	void add_enemy();
	void draw(sf::RenderTarget& target) const;
};

