#include "ProjectileManager.h"

#include <execution>
#include <SFML/Graphics/Rect.hpp>

#include "Enemy.h"
#include "EnemyManager.h"
#include "imgui.h"
#include "Projectile.h"
#include "Utils/Log.h"

ProjectileManager::ProjectileManager()
{
	if (!m_texture.loadFromFile(std::filesystem::path("Resources") / "Images" / "ball animation.png"))
	{
		LOG_ERROR("projectile texture couldn't load");
	}
}

void ProjectileManager::update(const double deltatime, std::shared_ptr<EnemyManager> enemy_manager,
                               std::shared_ptr<Player> player)
{
	std::for_each(std::execution::par, m_projectiles.begin(), m_projectiles.end(), 
		[&](const std::unique_ptr<Projectile>& projectile)
	{
		projectile->update(deltatime);
		const sf::Vector2f projectile_size = { 32,32 };
		const sf::FloatRect proj_rect = { projectile->current_pos - (projectile_size / 2.f),projectile_size };
		if (projectile->was_shot_by_player)
		{
			for (const auto& enemy : enemy_manager->all_intersections(proj_rect))
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
	});
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
	{
		int i = 0;

		//m_vertex_array.clear();
		m_vertex_array.resize(m_projectiles.size() * 6);

		for (const auto& projectile : m_projectiles)
		{
			const sf::Vector2f projectile_pos = projectile->current_pos;
			const sf::Vector2f projectile_size = { 32.f,32.f };
			const int sprite_id = projectile->sprite_id;
			const int animation_id = projectile->current_animation;

			m_vertex_array[i].position = { projectile_pos - projectile_size / 2.f };
			m_vertex_array[i + 1].position = { projectile_pos.x + projectile_size.x / 2.f,projectile_pos.y - projectile_size.y / 2.f };
			m_vertex_array[i + 2].position = { projectile_pos.x - projectile_size.x / 2.f,projectile_pos.y + projectile_size.y / 2.f };

			m_vertex_array[i + 3].position = { projectile_pos.x + projectile_size.x / 2.f,projectile_pos.y - projectile_size.y / 2.f };
			m_vertex_array[i + 4].position = { projectile_pos.x - projectile_size.x / 2.f,projectile_pos.y + projectile_size.y / 2.f };
			m_vertex_array[i + 5].position = { projectile_pos + projectile_size / 2.f };

			const sf::Vector2f single_sprite_size = { 30.f, 30.f };
			const sf::Vector2f top_left_tex_coord = { (float)(animation_id%2) * single_sprite_size.x , (float)(animation_id / 3) * single_sprite_size.y };
			const sf::Vector2f bottom_right_tex_coord = top_left_tex_coord + single_sprite_size;
			m_vertex_array[i].texCoords = top_left_tex_coord;
			m_vertex_array[i + 1].texCoords = { bottom_right_tex_coord.x,top_left_tex_coord.y };
			m_vertex_array[i + 2].texCoords = { top_left_tex_coord.x,bottom_right_tex_coord.y };

			m_vertex_array[i + 3].texCoords = { bottom_right_tex_coord.x,top_left_tex_coord.y };
			m_vertex_array[i + 4].texCoords = { top_left_tex_coord.x,bottom_right_tex_coord.y };
			m_vertex_array[i + 5].texCoords = bottom_right_tex_coord;

			/*sf::Color color = projectile->was_shot_by_player ? sf::Color::Red : sf::Color::White;
			m_vertex_array[i].color   = sf::Color(255,255,0);
			m_vertex_array[i+1].color = sf::Color();
			m_vertex_array[i+2].color = sf::Color();
			m_vertex_array[i+3].color = sf::Color();
			m_vertex_array[i+4].color = sf::Color();
			m_vertex_array[i+5].color = sf::Color();*/

			//ImGui::Text("%d", i);
			i += 6;
		}
		m_vertex_array.setPrimitiveType(sf::PrimitiveType::Triangles);

	}
	ImGui::Begin("Debug");
	ImGui::Text("amount of projectiles: %llu", m_projectiles.size());
	ImGui::End();
}

void ProjectileManager::draw(sf::RenderWindow& window)
{
	sf::RenderStates states = sf::RenderStates::Default;
	states.texture = &m_texture;
	window.draw(m_vertex_array, states);
	//sf::RectangleShape proj{ {32,32} };
	//proj.setFillColor(sf::Color::Red);
	//proj.setOrigin(proj.getSize() / 2.f);
	//for(const auto& projectile:m_projectiles)
	//{
	//	proj.setPosition(projectile->current_pos);
	//	proj.setFillColor(projectile->was_shot_by_player ? sf::Color::Red : sf::Color::White);
	//	window.draw(proj);
	//}
}
