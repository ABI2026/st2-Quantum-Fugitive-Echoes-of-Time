#pragma once
#include <vector>
#include <string>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>

#include "Eventsystem.h"
#include "LayerManager.h"
#include "Utils/Soundsystem.h"


/*enum stats
{

};*/
struct Stats
{							//standartwerte
	//Leben
	float health = 100.0f; //100% 
	float maxHealth = 100.0f; //maximales Leben bei 100%
	float healthGeneration = 10.0f; //10% jede sek
	float healthCooldown = 5.0f; // 5 sek cooldown bevor regeneration startet
	float lastDamageTaken = 0.0f; //Timer für Lebensgernation

	//Schaden
	float strength = 50.0f; //50
	float attackSpeed = 1.5f; // 1.50 pro sekunde
	float critDamage = 200.0f; //200%
	float critChange = 5.0f; //5% 

	//Movment / Bewegung
	float dashes = 1.0f; //1 pro 6 sek //mit leer taste dashen
	float dashCooldown = 6.0f; //cooldown alle 6 sek
	float lastDashTime = 0.0f; //Zeit vergangen nach dem Dash
	float movmentSpeed = 100.0f; //selsbterklärend 

	//Projectil / Area
	float projectileSpeed = 100.0f; // 100 % 
	float projectilePenetration = 0.0f; // wie oft das projektil durch gegner durch gehen soll
	float aoe = 100.0f; //flächen schaden also wie groß also die reichweite des flächenschadens , einmal schaden und auch die sammelreichweite
	bool rebound = false; //false ist nicht sofort aktiviert (rebound bedeutet wenn der gegner so oft wie möglich durchringen wurde
	// bis das projectil verschwindet. Jetzt verschwindet es nicht sondern prallt am letzten gegner ab. Apraller gehen immer richtung andere Gegner)
	float reboundTimes = 1.0f; //wie oft er aprallt. also wie viele weiter projectile gespawnt werden müssen.

	float shild = 0.0; //wie viele schilder um dir herum schweben.

	//Generelles
	float xpAmount = 0.0f; //wie viel xp / also wie viel xp du hast und die spalte weiter geht 
	float xpIndicator = 0.0f; // geht nur ganze zahlen also welches level du bist
	float cooldownGeneral = 100.0f; //100%

	// ... 
};

class Player {
public:

	Player();
	~Player();



	//enum weaponClass
	//{

	//};
	//struct weaponClass
	//{

	//};



	//getters und setters

	void update(std::shared_ptr<Eventsystem>& eventsystem, std::shared_ptr<Soundsystem>& soundsystem, double deltatime);

	const Stats& getStats() const
	{
		return m_stats;
	}

	void setStats(const Stats& i_stats)
	{
		m_stats = i_stats;
	}

	void increase_health(float increment)
	{
		m_stats.health += increment;
	}

	//position
	void updatePosition(const sf::Vector2f& movement, float deltaTime);

	sf::Vector2f getPosition() const 
	{ 
		return m_position; 
	}

	void setPosition(const sf::Vector2f& pos) //: m_position(pos)
	{ 
		m_position = pos;
		m_shape.setPosition(m_position);

	}

	void draw(sf::RenderWindow& window) const;

private:
	Stats m_stats;
	sf::Texture m_player_texture;
	sf::RectangleShape m_shape;
	sf::Vector2f m_position;
	sf::Vector2f m_velocity;
	float m_speed;


};
// push_back() emplace_back()  []  clear() pop_back()