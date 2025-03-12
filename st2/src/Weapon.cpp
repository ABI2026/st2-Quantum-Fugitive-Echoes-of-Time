#include "Weapon.h"

#include "Enemy.h"
#include "EnemyManager.h"
#include "Player.h"
#include "ProjectileManager.h"


void Weapon::update(double deltatime,
                    const sf::Vector2f pos,
                    const std::shared_ptr<EnemyManager>& enemy_manager,
                    const std::shared_ptr<ProjectileManager>& projectile_manager)
{
	current_cooldown -= deltatime;
	if(current_cooldown <= 0.0)
	{
		const std::shared_ptr<Enemy> enemy = enemy_manager->get_closest_enemy(pos);
		if (!enemy)
			return;

		sf::Vector2f dir = enemy->get_position() - pos;
		if (dir != sf::Vector2f{ 0.f,0.f })
			dir = dir.normalized();
		else
			dir = { 0.f,1.f };

		projectile_manager->add_projectile(pos,
			dir,
			projectile_speed,
			damage, //TODO: MIT SPIELER VERRECHNEN
			penetration,
			crit_damage, //TODO: MIT SPIELER VERRECHNEN
			crit_chance, //TODO: MIT SPIELER VERRECHNEN
			true); //TODO: gucken wie das mit waffen von gegnern ist
		current_cooldown = firerate;
	}
}
