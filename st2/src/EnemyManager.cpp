#include "EnemyManager.h"

#include "Enemy.h"
#include "imgui.h"
#include "Expbar.h"
#include "Utils/Log.h"
#include "Utils/Random.h"
constexpr float chunk_size = 128.f;
std::size_t KeyHasher::operator()(const sf::Vector2f& key) const
{
	const std::size_t h1 = std::hash<float>()(key.x);
	const std::size_t h2 = std::hash<float>()(key.y);

	return h1 ^ (h2 << 1);
}

EnemyManager::EnemyManager()
{
	m_vertex_array.setPrimitiveType(sf::PrimitiveType::Triangles);
	if (!m_texture.loadFromFile(std::filesystem::path("Resources/Images/Enemy.png")))
	{
		LOG_ERROR("failed loading enemy texture");
	}
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
	constexpr sf::Vector2f chunk_size_vec = { chunk_size,chunk_size };
	constexpr sf::Vector2f max_enemy_size = {64.f,64.f };
	constexpr sf::Vector2f min_enemy_size = { 64.f,64.f };
	for (auto& [cell_pos,enemies] : m_cells_enemies)
	{
		sf::FloatRect cell{ cell_pos - max_enemy_size / 2.f,(chunk_size_vec + max_enemy_size)};
		if(bounding_box.findIntersection(cell) == std::nullopt)
		{
			continue;
		}

		for (auto& enemy : enemies)
		{
			if(enemy.expired())
				continue;
			std::shared_ptr<Enemy> locked_enemy = enemy.lock();
			sf::Vector2f enemy_size = { 64,64 };
			sf::FloatRect enemy_rect{ locked_enemy->get_position() - (enemy_size / 2.f),enemy_size };
			if (bounding_box.findIntersection(enemy_rect) != std::nullopt)
			{
				intersected.emplace_back(locked_enemy);
			}
		}
	}

	return intersected;
}

void EnemyManager::spawn_enemy(Player* player)
{
	glm::vec2 pos = normalize(Random::vec2(-1,1));
	constexpr float radius = 1000; //in pixels
	pos *= radius /** Random::floating(1.f,5.f)*/;
	pos += glm::vec2{player->getPosition().x, player->getPosition().y};
	m_enemies.push_back(std::make_shared<Enemy>(10.f, 10.f, 300.f, sf::Vector2f{ pos.x,pos.y }, player));
}

void EnemyManager::update(std::shared_ptr<Eventsystem>& eventsystem, std::shared_ptr<Soundsystem>& soundsystem, double deltatime, Player* player, std::shared_ptr<Expbar>& expbar)
{
	for (const auto& enemy : m_enemies)
	{
		//const sf::Vector2f prev_pos = enemy->get_position();
		enemy->update(eventsystem,soundsystem,deltatime);
		//const sf::Vector2f new_pos = enemy->get_position();
		//const sf::Vector2f prev_chunk_pos = prev_pos / chunk_size;
		//const sf::Vector2f new_chunk_pos = enemy->get_position() / chunk_size;

		//const sf::Vector2f offset = prev_pos - new_pos;
	}

	static int amount_to_spawn = 1;
	if (eventsystem->get_key_action(sf::Keyboard::Key::G) & 1)
	{
		for (int i = 0; i < amount_to_spawn; ++i)
			spawn_enemy(player);
	}

	m_cells_enemies.clear();
	constexpr int first_death_sound_id = 7; // 7
	constexpr int amount_of_death_sounds = 3; 
	static int death_sound = 0;
	int count = 0;
	std::erase_if(m_enemies, [&](const std::shared_ptr<Enemy>& enemy)
	{
		const bool should_get_removed = enemy->get_health() <= 0.0;
		if (should_get_removed)
		{
			if (count < amount_of_death_sounds)
			{
				soundsystem->play_sound("player_sounds", first_death_sound_id + death_sound);
				death_sound++;
				count++;
				death_sound %= amount_of_death_sounds;
			}
			expbar->setExp(5 + expbar->getExp());
		}
		else
		{
			const sf::Vector2f chunk_pos = 
			{
				floor(enemy->get_position().x / chunk_size) * chunk_size,
				floor(enemy->get_position().y / chunk_size) * chunk_size
			};
			
			m_cells_enemies[chunk_pos].emplace_back(enemy);
		}

		return should_get_removed;
	});



	ImGui::Begin("Debug");
	ImGui::DragInt("spawn_amount", &amount_to_spawn,1,0,INT_MAX);
	{
		int i = 0;

		//m_vertex_array.clear();
		m_vertex_array.resize(m_enemies.size() * 6);

		for (const auto&  enemy : m_enemies)
		{
			const sf::Vector2f enemy_pos = enemy->get_position();
			const sf::Vector2f enemy_size = { 64.f,64.f };
			const int sprite_id = enemy->get_sprite_id();
			const int animation_id = enemy->get_current_animation();

			m_vertex_array[i]    .position = { enemy_pos - enemy_size/2.f };
			m_vertex_array[i + 1].position = { enemy_pos.x + enemy_size.x / 2.f,enemy_pos.y - enemy_size.y / 2.f };
			m_vertex_array[i + 2].position = { enemy_pos.x - enemy_size.x / 2.f,enemy_pos.y + enemy_size.y / 2.f };

			m_vertex_array[i + 3].position = { enemy_pos.x + enemy_size.x / 2.f,enemy_pos.y - enemy_size.y / 2.f };
			m_vertex_array[i + 4].position = { enemy_pos.x - enemy_size.x / 2.f,enemy_pos.y + enemy_size.y / 2.f };
			m_vertex_array[i + 5].position = { enemy_pos + enemy_size/2.f };

			const sf::Vector2f single_sprite_size = { 16.f, 16.f };
			const sf::Vector2f top_left_tex_coord = { (float) sprite_id * single_sprite_size.x , (float) animation_id * single_sprite_size.y };
			const sf::Vector2f bottom_right_tex_coord = top_left_tex_coord + single_sprite_size;
			m_vertex_array[i]    .texCoords = top_left_tex_coord;
			m_vertex_array[i + 1].texCoords = { bottom_right_tex_coord.x,top_left_tex_coord.y};
			m_vertex_array[i + 2].texCoords = { top_left_tex_coord.x,bottom_right_tex_coord.y };

			m_vertex_array[i + 3].texCoords = { bottom_right_tex_coord.x,top_left_tex_coord.y};
			m_vertex_array[i + 4].texCoords = { top_left_tex_coord.x,bottom_right_tex_coord.y };
			m_vertex_array[i + 5].texCoords = bottom_right_tex_coord;

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
