#pragma once
#include <SFML/Graphics/Texture.hpp>

class EnemyManager;
class Player;
class ProjectileManager;

class Weapon
// schaden (float 0 und hoeher), feuerrate in sec (intervall),
// durchschlag (int 1 heisst trifft 1 gegner und verschwindet dann),
// crit damage, crit chance (float 0-1), proj speed,
// projektil kann gegner nur einmal treffen
// 
// gibt stats mit spieler/gegner verrechnet an nen projektil mit start und richtung
// automatisch schiessen,
// projektil manager
{
	//sf::Texture* m_texture;
	double damage;
	double firerate;
	double current_cooldown{0};
	int penetration;
	double crit_chance;
	double crit_damage;
	float projectile_speed;
public:
	Weapon(const double damage, const double firerate, const int penetration, const double crit_chance,
		const double crit_damage, const float projectile_speed)
		: damage(damage),
		  firerate(firerate),
		  penetration(penetration),
		  crit_chance(crit_chance),
		  crit_damage(crit_damage),
		  projectile_speed(projectile_speed)
	{
	}

	void update(double deltatime,
	            const sf::Vector2f pos,
	            const std::shared_ptr<EnemyManager>& enemy_manager, const std::shared_ptr<ProjectileManager>& projectile_manager
	);
};

