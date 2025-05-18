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
#include "Utils/Random.h"

int unsigned Enemy::count = 0;



Enemy::~Enemy()
{
	count--;
}

Enemy::Enemy(const float i_damage, const double i_health, const float i_speed, const sf::Vector2f i_position, Player* i_player)
	: m_damage(i_damage), m_health(i_health), m_speed(i_speed), m_position(i_position),/* m_texture(i_texture),*/ m_player(i_player)
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

//void Enemy::draw(sf::RenderTarget& target) const
//{
//	sf::RectangleShape rectangle;
//	rectangle.setTexture(m_texture);
//	rectangle.setPosition(m_position);
//	rectangle.setSize({64.f,64.f});
//	rectangle.setOrigin(rectangle.getSize() / 2.f);
//	target.draw(rectangle);
//}

void Enemy::attack_player()
{
	//m_player->getStats().health = -m_damage; der Player hat leider Godmode
}

bool Enemy::take_damage(double damage, double crit_chance, double crit_damage)
{
	if (m_invicibility_time > 0.0)
		return false;

	m_health -= damage + Random::foo(crit_chance) * crit_damage;//TODO: ACTUAL DAMAGE CALCULATION
	m_invicibility_time = 0.125;
	return true;
}

void Enemy::set_damage(const float i_damage)
{
	m_damage = i_damage;
}

void Enemy::set_health(const double i_health)
{
	if (i_health <= 0) 
	{
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

double Enemy::get_health() const
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
	condt += deltatime;
	while (condt >= 0.125f)
	{
		current_animation++;
		condt -= 0.125f;
		current_animation %= 4;
	}
	m_invicibility_time -= deltatime;
	const sf::Vector2f distance_vec = m_player->getPosition() - m_position;
	//const float dist = distance_vec.length();
	const float distance_length = sqrt((distance_vec.x * distance_vec.x) + (distance_vec.y * distance_vec.y));
	if (distance_length < 3)
	{
		m_position = m_player->getPosition();
	}
	else 
	{
		const float m = m_speed / distance_length;//m = speed/wurzel((a*a)+(b*b))
		m_position = { (distance_vec.x * m * (float)deltatime) + m_position.x,(distance_vec.y * m * (float)deltatime) + m_position.y };
	}
}

int Enemy::get_current_animation() const
{
	return current_animation;
}

int Enemy::get_sprite_id() const
{
	return sprite_id;
}
