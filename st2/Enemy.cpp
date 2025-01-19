#include <vector>
#include <string>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>

#include "Eventsystem.h"
#include "LayerManager.h"
#include "Utils/Soundsystem.h"
#include "Utils/Log.h"
#include "Enemy.h"

Enemy::~Enemy() {
	count--;
}
Enemy::Enemy(float i_damage, float i_health, float i_speed, sf::Vector2f i_position) {
	count++;
	m_damage = i_damage;
	m_health = i_health;
	m_speed = i_speed;
	m_position = i_position;
	if (!m_texture.loadFromFile("Resources/Images/hintergrund.jpg")) {
		LOG_ERROR("failed loading player texture");
	}
}
void Enemy::draw(sf::RenderWindow& window) {
	sf::Sprite m_sprite(m_texture);
	window.draw(m_sprite);
}
void Enemy::setDamage(float i_damage) {
	m_damage = i_damage;
}
void Enemy::setHealth(float i_health) {
	m_health = i_health;
}
void Enemy::setSpeed(float i_speed) {
	m_speed = i_speed;
}
void Enemy::setPosition(sf::Vector2f i_position) {
	m_position = i_position;
}
unsigned int Enemy::getCount() {
	return count;
}
float Enemy::getDamage() {
	return m_damage;
}
float Enemy::getHealth() {
	return m_health;
}
float Enemy::getSpeed() {
	return m_speed;
}
sf::Vector2f Enemy::getPosition() {
	return m_position;
}
void Enemy::update(std::shared_ptr<Eventsystem>& eventsystem, std::shared_ptr<Soundsystem>& soundsystem, double deltatime) {
	  
}