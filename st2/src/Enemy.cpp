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
	sf::RectangleShape rectangle;
	rectangle.setTexture(m_texture);
	rectangle.setPosition(m_position);
	rectangle.setSize(sf::Vector2f(m_texture->getSize()/2u));
	rectangle.setOrigin(rectangle.getSize() / 2.f);
	target.draw(rectangle);
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
	if (i_health <= 0) 
	{
		this->~Enemy(); // du hurensohn
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

void Enemy::update([[maybe_unused]] std::shared_ptr<Eventsystem>& eventsystem, [[maybe_unused]] std::shared_ptr<Soundsystem>& soundsystem, const double deltatime)
{
	const sf::Vector2f distance_vec = m_player->getPosition() - m_position;
	//const float dist = distance_vec.length();
	const float distance_length = sqrt((distance_vec.x * distance_vec.x) + (distance_vec.y * distance_vec.y));
	if (distance_length < 20)
	{
		m_position = m_player->getPosition();
	}
	else 
	{
		const float m = m_speed / distance_length;//m = speed/wurzel((a*a)+(b*b))
		m_position = { (distance_vec.x * m * (float)deltatime) + m_position.x,(distance_vec.y * m * (float)deltatime) + m_position.y };
	}
}
