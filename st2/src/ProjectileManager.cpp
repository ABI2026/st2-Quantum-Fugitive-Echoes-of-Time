﻿#include "ProjectileManager.h"

#include <SFML/Graphics/Rect.hpp>

#include "Enemy.h"
#include "EnemyManager.h"
#include "imgui.h"
#include "Projectile.h"

void ProjectileManager::update(const double deltatime, std::shared_ptr<EnemyManager> enemy_manager,
	std::shared_ptr<Player> player)
{
	for (const auto& projectile : m_projectiles)
	{
		projectile->update(deltatime);
		const sf::Vector2f projectile_size = { 32,32 };
		const sf::FloatRect proj_rect = { projectile->current_pos - (projectile_size / 2.f),projectile_size };
		if (projectile->was_shot_by_player)
		{
			for (auto enemy : enemy_manager->all_intersections(proj_rect))
			{
				if (enemy->take_damage(projectile->damage, projectile->crit_chance, projectile->crit_damage))
				{
					projectile->penetration--;
				}

				if (projectile->penetration <= 0)
				{
					break;
				}
			}
		}
		else
		{
			if (player->intersects(proj_rect))
			{
				if (player->take_damage(projectile->damage, projectile->crit_chance, projectile->crit_damage))
				{
					projectile->penetration--;
				}
			}
		}
	}
	std::erase_if(m_projectiles, [](const std::unique_ptr<Projectile>& proj)
		{
			return proj->penetration < 1 || proj->lifetime <0.0;
		});

	//if(auto enemy =	enemy_manager->get_closest_enemy({ 0.f,0.f }))
	//{
	//	sf::Vector2f dir = player->getPosition() - enemy->get_position();
	//	if (dir != sf::Vector2f{ 0.f,0.f })
	//		dir = dir.normalized();
	//	else
	//		dir = { 0.f,1.f };
	//
	//	add_projectile(enemy->get_position(),
	//		dir,
	//		4000.f,
	//		1.0, 
	//		1,
	//		2.0, 
	//		0.5, 
	//		false);
	//}

	ImGui::Begin("Debug");
	ImGui::Text("amount of projectiles: %d", m_projectiles.size());
	ImGui::End();
}

void ProjectileManager::draw(sf::RenderWindow& window)
{
	sf::RectangleShape proj{ {32,32} };
	proj.setFillColor(sf::Color::Red);
	proj.setOrigin(proj.getSize() / 2.f);
	for(const auto& projectile:m_projectiles)
	{
		proj.setPosition(projectile->current_pos);
		proj.setFillColor(projectile->was_shot_by_player ? sf::Color::Red : sf::Color::White);
		window.draw(proj);
	}
}
