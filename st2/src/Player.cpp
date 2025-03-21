#include <iostream>
#include "Player.h"

#include <glm/geometric.hpp>
#include <glm/vec2.hpp>

#include "imgui.h"
#include "Weapon.h"
#include "Utils/Log.h"
#include "Utils/Random.h"

Player::Player() : m_shape({ 100.f,100.f }), m_position(320.f, 320.f), m_velocity(0.0f, 0.0f)
{
	m_speed = 0;
    //konstrukt	 ;
	if (!m_player_texture.loadFromFile("Resources/Images/hintergrund.jpg"))
		LOG_ERROR("failed loading player texture");
	m_shape.setOrigin({50.f, 50.f});
	m_shape.setTexture(&m_player_texture);
	m_weapon = std::make_shared<Weapon>(2.0, 0.25f, 1, 0.5, 50.0, 400.f);
}

Player::~Player()
{
//destrukt
}

bool Player::take_damage(double damage, double crit_chance, double crit_damage)
{
	if (m_invicibility_time > 0.0)
		return false;

	m_stats.health -= damage + Random::foo(crit_chance) * crit_damage; //TODO: ACTUAL DAMAGE CALCULATION
	m_invicibility_time = 0.125;
	return true;
}

void Player::update(std::shared_ptr<Eventsystem>& eventsystem,
	[[maybe_unused]] std::shared_ptr<Soundsystem>& soundsystem,
	[[maybe_unused]] const double deltatime,
	std::shared_ptr<EnemyManager> enemy_manager,
	std::shared_ptr<ProjectileManager> projectile_manager

)
{
	m_invicibility_time -= deltatime;
	m_weapon->update(deltatime, m_position, enemy_manager, projectile_manager);
	glm::vec2 movement = { 0.f,0.f };

	if (eventsystem->get_key_state(sf::Keyboard::Key::W))
		movement.y -= 1;
	if (eventsystem->get_key_state(sf::Keyboard::Key::A))
		movement.x -= 1;
	if (eventsystem->get_key_state(sf::Keyboard::Key::S))
		movement.y += 1;
	if (eventsystem->get_key_state(sf::Keyboard::Key::D))
		movement.x += 1;

	ImGui::Begin("Debug");
	ImGui::Text("fps: %f", 1.0 / deltatime);
	if (movement != glm::vec2{ 0.f,0.f })
	{
		movement = glm::normalize(movement);

		ImGui::Text("direction: x:%f y:%f", movement.x, movement.y);

		movement = movement * 600.f * static_cast<float>(deltatime);
		ImGui::Text("movement: x:%f y:%f", movement.x, movement.y);

		glm::vec2 new_pos = { m_position.x + movement.x, m_position.y + movement.y };
		//new_pos = clamp(new_pos, { 50.f,50.f }, { 590.f,590.f });
		m_position = { new_pos.x, new_pos.y };
	}
	else
	{
		ImGui::Text("direction: x:0.000000 y:0.000000");
		ImGui::Text("movement: x:0.000000 y:0.000000");
	}

	ImGui::Text("position: x:%f y:%f", m_position.x, m_position.y);
	ImGui::End();
	m_shape.setPosition(m_position);


}

void Player::updatePosition([[maybe_unused]] const sf::Vector2f& movement, [[maybe_unused]] float deltaTime)
{
	throw std::runtime_error("bitte nicht nutzen danke");

	//float length = std::sqrt(movement.x * movement.x + movement.y * movement.y);
	//if (length > 0) {
	//	//sf::Vector2f normalizedMovement = movement / length;

	//	m_position.x += movement.x * m_speed * deltaTime;
	//	m_position.y += movement.y * m_speed * deltaTime;

	//	//m_velocity = normalizedMovement * m_stats.movementSpeed;
	//	//m_position += m_velocity * deltaTime;
	//}
}

void Player::draw(sf::RenderWindow& window) const
{
	window.draw(m_shape);
}
