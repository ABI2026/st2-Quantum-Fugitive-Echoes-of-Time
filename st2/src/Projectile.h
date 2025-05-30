﻿#pragma once
#include <SFML/System/Vector2.hpp>


class Projectile
{
public:
	sf::Vector2f current_pos;
	sf::Vector2f dir;
	float speed;
	double damage;
	int penetration;
	double crit_damage;
	double crit_chance;
	bool was_shot_by_player;
	double lifetime{ 20.0 };
	int sprite_id = 0;
	int current_animation = 0;
	double passed_time = 0.0;

	Projectile(const sf::Vector2f& current_pos,
		const sf::Vector2f& dir,
		const float speed,
		const double damage,
		const int penetration,
		const double crit_damage,
		const double crit_chance,
		const bool was_shot_by_player)
		: current_pos(current_pos),
		dir(dir),
		speed(speed),
		damage(damage),
		penetration(penetration),
		crit_damage(crit_damage),
		crit_chance(crit_chance),
		was_shot_by_player(was_shot_by_player)
	{
	}

	void update(const double deltatime)
	{
		lifetime -= deltatime;
		passed_time += deltatime;
		current_pos += dir * speed * (float)deltatime;
		
		if(passed_time > 0.125)
		{
			current_animation++;
			current_animation %= 6;
			passed_time -= 0.125;
		}
	}
};
