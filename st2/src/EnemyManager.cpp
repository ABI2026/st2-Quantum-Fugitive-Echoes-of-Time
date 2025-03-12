#include "EnemyManager.h"

#include "Enemy.h"
#include "imgui.h"
#include "Utils/Log.h"
#include "Utils/Random.h"

EnemyManager::EnemyManager()
{
	m_textures.emplace_back(std::filesystem::path("Resources/Images/hintergrund.jpg"));
}

void EnemyManager::spawn_enemy(Player* player)
{
	glm::vec2 pos = normalize(Random::vec2(-1,1));
	constexpr float radius = 1000; //in pixels
	pos *= radius;
	pos += glm::vec2{player->getPosition().x, player->getPosition().y};
	m_enemies.push_back(std::make_shared<Enemy>(10.f, 10.f, 300.f, sf::Vector2f{ pos.x,pos.y }, &m_textures.back(), player));
}

void EnemyManager::update(std::shared_ptr<Eventsystem>& eventsystem, std::shared_ptr<Soundsystem>& soundsystem, double deltatime, Player* player)
{
	for (const auto& enemy : m_enemies)
	{
//		const sf::Vector2f prev_pos = enemy->get_position();
		enemy->update(eventsystem,soundsystem,deltatime);
//		const sf::Vector2f new_pos = enemy->get_position();
//		const sf::Vector2f offset = prev_pos - new_pos;
	}

	if(eventsystem->get_key_action(sf::Keyboard::Key::G) == Eventsystem::action_pressed)
	{
		for(int i = 0; i < 50;++i)
			spawn_enemy(player);
	}
	ImGui::Begin("Debug");
	ImGui::Text("%d",m_enemies.size());
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
