#pragma once
#include <vector>
#include <string>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>

#include "Eventsystem.h"
#include "LayerManager.h"
#include "Utils/Soundsystem.h"
class Player;
class Enemy
{
protected:
	static unsigned int count;
	float m_damage;
	float m_health;
	float m_speed;
	sf::Texture m_texture;
	sf::Vector2f m_position;
	Player* m_player;
public:
	~Enemy();
	Enemy(float i_damage, float i_health, float i_speed, sf::Vector2f i_position, Player* i_player);

	void update(std::shared_ptr<Eventsystem>& eventsystem, std::shared_ptr<Soundsystem>& soundsystem, double deltatime);
	void draw(sf::RenderWindow &window);
	void attackPlayer();

	void setPosition(sf::Vector2f i_position);
	void setDamage(float i_damage);
	void setHealth(float i_health);
	void setSpeed(float i_speed);
	void setPlayer(Player* i_player);

	float getDamage();
	float getHealth();
	float getSpeed();
	sf::Vector2f getPosition();
	const Player* getPlayer();
	static unsigned int getCount();
};