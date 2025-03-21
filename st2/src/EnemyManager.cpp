#include "EnemyManager.h"

#include "Enemy.h"
#include "imgui.h"
#include "Expbar.h"
#include "Utils/Log.h"
#include "Utils/Random.h"

EnemyManager::EnemyManager()
{
	m_textures.emplace_back(std::filesystem::path("Resources/Images/hintergrund.jpg"));
}

std::shared_ptr<Enemy> EnemyManager::get_closest_enemy(sf::Vector2f position)
{
	if (m_enemies.empty())
		return nullptr;
	return m_enemies.front(); //TODO: IMPLEMENT ACTUAL LOGIC FOR THIS
}

std::vector<std::shared_ptr<Enemy>> EnemyManager::all_intersections(sf::FloatRect bounding_box)
{
	std::vector<std::shared_ptr<Enemy>> intersected;
	for (auto enemy: m_enemies)
	{
		sf::Vector2f enemy_size = { 64,64 };
		sf::FloatRect enemy_rect{ enemy->get_position() - (enemy_size / 2.f),enemy_size};
		if(bounding_box.findIntersection(enemy_rect) != std::nullopt)
		{
			intersected.emplace_back(enemy);
		}
	}
	return intersected;
}

void EnemyManager::spawn_enemy(Player* player)
{
	glm::vec2 pos = normalize(Random::vec2(-1,1));
	constexpr float radius = 1000; //in pixels
	pos *= radius;
	pos += glm::vec2{player->getPosition().x, player->getPosition().y};
	m_enemies.push_back(std::make_shared<Enemy>(10.f, 10.f, 300.f, sf::Vector2f{ pos.x,pos.y }, &m_textures.back(), player));
}

void EnemyManager::update(std::shared_ptr<Eventsystem>& eventsystem, std::shared_ptr<Soundsystem>& soundsystem, double deltatime, Player* player, std::shared_ptr<Expbar>& expbar)
{
	for (const auto& enemy : m_enemies)
	{
//		const sf::Vector2f prev_pos = enemy->get_position();
		enemy->update(eventsystem,soundsystem,deltatime);
//		const sf::Vector2f new_pos = enemy->get_position();
//		const sf::Vector2f offset = prev_pos - new_pos;
	}

	std::erase_if(m_enemies, [&](const std::shared_ptr<Enemy>& enemy)
		{
			bool should_get_removed = enemy->get_health() <= 0.0;
			if (should_get_removed)
			{
				expbar->setExp(5 + expbar->getExp());
			}

			return should_get_removed;
		});

	if(eventsystem->get_key_action(sf::Keyboard::Key::G)&1)
	{
		for(int i = 0; i < 50;++i)
			spawn_enemy(player);
	}
	ImGui::Begin("Debug");
	ImGui::Text("amount of enemies: %d",m_enemies.size());
	ImGui::End();
}

void EnemyManager::add_enemy()
{
}

void EnemyManager::draw(sf::RenderTarget& target) const
{
	for (const auto& enemy : m_enemies)
	{
		enemy->draw(target);
	}
}
