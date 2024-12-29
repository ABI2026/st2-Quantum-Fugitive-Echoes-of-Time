#include <iostream>
#include "Player.h"

#include <glm/geometric.hpp>
#include <glm/vec2.hpp>

#include "imgui.h"
#include "Utils/Log.h"

Player::Player() : m_shape({ 100.f,100.f }), m_position(320.f, 320.f), m_velocity(0.0f, 0.0f)
{
    //konstrukt	 ;
	if (!m_player_texture.loadFromFile("Resources/Images/hintergrund.jpg"))
		LOG_ERROR("failed loading player texture");
	m_shape.setOrigin({50.f, 50.f});
	m_shape.setTexture(&m_player_texture);
}

Player::~Player()
{
//destrukt
}

void Player::update(std::shared_ptr<Eventsystem>& eventsystem, std::shared_ptr<Soundsystem>& soundsystem, const double deltatime)
{
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
		new_pos = clamp(new_pos, { 50.f,50.f }, { 590.f,590.f });
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

void Player::updatePosition(const sf::Vector2f& movement, float deltaTime)
{
	throw std::runtime_error("bitte nicht nutzen danke");

	float length = std::sqrt(movement.x * movement.x + movement.y * movement.y);
	if (length > 0) {
		//sf::Vector2f normalizedMovement = movement / length;

		m_position.x += movement.x * m_speed * deltaTime;
		m_position.y += movement.y * m_speed * deltaTime;

		//m_velocity = normalizedMovement * m_stats.movementSpeed;
		//m_position += m_velocity * deltaTime;
	}
}

void Player::draw(sf::RenderWindow& window) const
{
	window.draw(m_shape);
}
