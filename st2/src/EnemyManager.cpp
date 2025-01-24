#include "EnemyManager.h"

#include "Enemy.h"
#include "imgui.h"
#include "Utils/Log.h"
#include "Utils/Random.h"

EnemyManager::EnemyManager()
{
	textures.emplace_back(std::filesystem::path("Resources/Images/hintergrund.jpg"));
}

void EnemyManager::spawn_enemy(Player* player)
{
	glm::vec2 pos = normalize(Random::vec2(-1,1));
	constexpr float radius = 1000; //in pixels
	pos *= radius;
	pos += glm::vec2{player->getPosition().x, player->getPosition().y};
	enemies.push_back(std::make_shared<Enemy>(10.f, 10.f, 300.f, sf::Vector2f{ pos.x,pos.y }, &textures.back(), player));
}

void EnemyManager::update(std::shared_ptr<Eventsystem>& eventsystem, std::shared_ptr<Soundsystem>& soundsystem, double deltatime, Player* player)
{
	for (const auto& enemy : enemies)
	{
		enemy->update(eventsystem,soundsystem,deltatime);
	}

	if(eventsystem->get_key_action(sf::Keyboard::Key::G) == Eventsystem::action_pressed)
	{
		spawn_enemy(player);
	}
	ImGui::Begin("Debug");
	ImGui::Text("%d",enemies.size());
	ImGui::End();
}

void EnemyManager::add_enemy()
{
}

void EnemyManager::draw(sf::RenderTarget& target) const
{
	for (const auto& enemy : enemies)
	{
		enemy->draw(target);
	}
}
