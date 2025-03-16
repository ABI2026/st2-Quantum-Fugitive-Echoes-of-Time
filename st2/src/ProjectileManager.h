#pragma once
#include <memory>
#include <vector>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>

#include "Projectile.h"


class Projectile;
class EnemyManager;
class Player;

class ProjectileManager
{
	std::vector<std::unique_ptr<Projectile>> m_projectiles;
public:

	void add_projectile(sf::Vector2f current_pos,
	sf::Vector2f dir,
	float speed,
	double damage,
	int penetration,
	double crit_damage,
	double crit_chance,
	bool was_shot_by_player)
	{
		m_projectiles.emplace_back(std::make_unique<Projectile>(
			current_pos,
			dir,
			speed,
			damage,
			penetration,
			crit_damage,
			crit_chance,
			was_shot_by_player));
	}

	void update(const double deltatime, std::shared_ptr<EnemyManager> enemy_manager, std::shared_ptr<Player> player);
	void draw(sf::RenderWindow& window);
};
