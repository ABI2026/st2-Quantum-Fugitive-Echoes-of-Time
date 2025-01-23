#include <vector>
#include <string>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>

#include "Eventsystem.h"
#include "LayerManager.h"
#include "Utils/Soundsystem.h"
#include "Utils/Log.h"
#include "Enemy.h"
#include "Player.h"

int unsigned Enemy::count = 0;
Enemy::~Enemy()
{
	count--;
}

Enemy::Enemy(const float i_damage, const float i_health, const float i_speed, const sf::Vector2f i_position, sf::Texture* i_texture, Player* i_player)
	: m_damage(i_damage), m_health(i_health), m_speed(i_speed), m_position(i_position), m_texture(i_texture), m_player(i_player)
{
	count++;
	m_player = i_player;
	m_damage = i_damage;
	m_health = i_health;
	m_speed = i_speed;
	m_position = i_position;

	/*if (!m_texture.loadFromFile("Resources/Images/hintergrund.jpg")) {
		LOG_ERROR("failed loading Enemy texture");
	}*/
}

void Enemy::draw(sf::RenderTarget& target) const
{
	sf::Sprite m_sprite(*m_texture);
	m_sprite.setPosition(m_position);
	m_sprite.setOrigin((sf::Vector2f)m_texture->getSize() / 2.f);
	target.draw(m_sprite);
}

void Enemy::attack_player()
{
	//m_player->getStats().health = -m_damage; der Player hat leider Godmode
}

void Enemy::set_damage(const float i_damage)
{
	m_damage = i_damage;
}

void Enemy::set_health(const float i_health)
{
	if (i_health <= 0) {
		this->~Enemy();
	}
	m_health = i_health;
}

void Enemy::set_speed(const float i_speed)
{
	m_speed = i_speed;
}

void Enemy::set_position(const sf::Vector2f i_position)
{
	m_position = i_position;
}

void Enemy::set_player(Player* i_player)
{
	m_player = i_player;
}

unsigned int Enemy::get_count()
{
	return count;
}

float Enemy::get_damage() const
{
	return m_damage;
}

float Enemy::get_health() const
{
	return m_health;
}

float Enemy::get_speed() const
{
	return m_speed;
}

sf::Vector2f Enemy::get_position() const
{
	return m_position;
}

const Player* Enemy::get_player() const
{
	return m_player;
}

void Enemy::update(std::shared_ptr<Eventsystem>& eventsystem, std::shared_ptr<Soundsystem>& soundsystem, const double deltatime)
{
	const float m = m_speed / (float)sqrt(
		((m_player->getPosition().x - m_position.x) *
		(m_player->getPosition().x - m_position.x)) +
		((m_player->getPosition().y - m_position.y) * 
		(m_player->getPosition().y - m_position.y)));//m = speed/wurzel((a*a)+(b*b))
	m_position = { ((m_player->getPosition().x - m_position.x) * m * (float)deltatime) + m_position.x,((m_player->getPosition().y - m_position.y) * m * (float)deltatime) + m_position.y };

}