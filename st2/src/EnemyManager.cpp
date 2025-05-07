#include "EnemyManager.h"

#include "Enemy.h"
#include "imgui.h"
#include "Expbar.h"
#include "Utils/Log.h"
#include "Utils/Random.h"

EnemyManager::EnemyManager()
{
	m_vertex_array.setPrimitiveType(sf::PrimitiveType::Triangles);
	if (!m_texture.loadFromFile(std::filesystem::path("Resources/Images/Enemy.png")))
		LOG_ERROR("failed loading enemy texture");
}

std::shared_ptr<Enemy> EnemyManager::get_closest_enemy(sf::Vector2f position)
{
	if (m_enemies.empty())
		return nullptr;
	return 	std::ranges::min(m_enemies, [position](const std::shared_ptr<Enemy>& enemy1, const std::shared_ptr<Enemy>& enemy2) {
		const float dist1 = sf::Vector2f(position - enemy1->get_position()).length();
		const float dist2 = sf::Vector2f(position - enemy2->get_position()).length();
		return dist1 < dist2;
		});

	//return m_enemies.front(); //TODO: IMPLEMENT ACTUAL LOGIC FOR THIS
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
	m_enemies.push_back(std::make_shared<Enemy>(10.f, 10.f, 300.f, sf::Vector2f{ pos.x,pos.y }, player));
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
		for(int i = 0; i < 500;++i)
			spawn_enemy(player);
	}
	ImGui::Begin("Debug");
	{
		int i = 0;

			//m_vertex_array.clear();
		m_vertex_array.resize(m_enemies.size() * 6);

		for (const auto&  enemy : m_enemies)
		{
			const sf::Vector2f enemy_pos = enemy->get_position();
			const sf::Vector2f enemy_size = { 64.f,64.f };

			m_vertex_array[i].position = { enemy_pos - enemy_size/2.f };
			m_vertex_array[i + 1].position = { enemy_pos.x + enemy_size.x / 2.f,enemy_pos.y - enemy_size.y / 2.f };
			m_vertex_array[i + 2].position = { enemy_pos.x - enemy_size.x / 2.f,enemy_pos.y + enemy_size.y / 2.f };

			m_vertex_array[i + 3].position = { enemy_pos.x + enemy_size.x / 2.f,enemy_pos.y - enemy_size.y / 2.f };
			m_vertex_array[i + 4].position = { enemy_pos.x - enemy_size.x / 2.f,enemy_pos.y + enemy_size.y / 2.f };
			m_vertex_array[i + 5].position = { enemy_pos + enemy_size/2.f };

			m_vertex_array[i]    .texCoords = {0,0};
			m_vertex_array[i + 1].texCoords = {16,0};
			m_vertex_array[i + 2].texCoords = {0,16};

			m_vertex_array[i + 3].texCoords = {16,0};
			m_vertex_array[i + 4].texCoords = {0,16};
			m_vertex_array[i + 5].texCoords = {16,16};

			//ImGui::Text("%d", i);
			i += 6;
		}
		m_vertex_array.setPrimitiveType(sf::PrimitiveType::Triangles);

	}

	ImGui::Text("amount of enemies: %d",m_enemies.size());
	ImGui::End();
}

void EnemyManager::add_enemy()
{
}

void EnemyManager::draw(sf::RenderTarget& target) const
{
	sf::RenderStates states = sf::RenderStates::Default;
	states.texture = &m_texture;
	target.draw(m_vertex_array,states);
	//for (const auto& enemy : m_enemies)
	//{
	//	enemy->draw(target);
	//}
}
