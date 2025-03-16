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
	static unsigned int count; //?? wofuer wird count gebraucht? das wird spaeter eh in nem std::vector oder aehnlichem gespeichert
	float m_damage{0}; 
	double m_health{0}; 
	float m_speed {0}; 
	sf::Vector2f m_position;
	sf::Texture* m_texture;
	Player* m_player; //??
	double m_invicibility_time{0};
public:
	~Enemy();
	Enemy(float i_damage, float i_health, float i_speed, sf::Vector2f i_position, sf::Texture* i_texture, Player* i_player);

	void update(std::shared_ptr<Eventsystem>& eventsystem, std::shared_ptr<Soundsystem>& soundsystem, double deltatime);
	void draw(sf::RenderTarget& target) const;
	void attack_player(); //?? warum wird das so gemacht ??

	bool take_damage(double damage, double crit_chance, double crit_damage);

	void set_position(sf::Vector2f i_position);
	void set_damage(float i_damage);
	void set_health(double i_health);
	void set_speed(float i_speed);
	void set_player(Player* i_player);

	[[nodiscard]] float get_damage() const;
	[[nodiscard]] double get_health() const;
	[[nodiscard]] float get_speed() const;
	[[nodiscard]] sf::Vector2f get_position() const;
	[[nodiscard]] const Player* get_player() const;
	static unsigned int get_count();
};