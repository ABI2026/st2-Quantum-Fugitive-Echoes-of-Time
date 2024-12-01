#pragma once
#include <vector>
#include <string>
#include <SFML/System/Vector2.hpp>

class Player {
public:



	/*enum stats
	{

	};*/
	struct stats
	{							//standartwerte
		//Leben
		float health = 100.0; //100% 
		float healthGeneration = 10.0; //10% jede sek
		float healthCooldown = 5.0; // 5 sek cooldown bevor regeneration startet

		//Schaden
		float strength = 50.0; //50
		float attackSpeed = 1.5; // 1.50 pro sekunde
		float critDamage = 200.0; //200%
		float critChange = 5.0; //5% 

		//Movment / Bewegung
		float dashes = 1.0; //1 pro 6 sek //mit leer taste dashen
		float movmentSpeed = 100.0; //selsbterklärend 

		//Projectil / Area
		float projectileSpeed = 100.0; // 100 % 
		float projectilePenetration = 0.0; // wie oft das projektil durch gegner durch gehen soll
		float aoe = 100.0; //flächen schaden also wie groß also die reichweite des flächenschadens , einmal schaden und auch die sammelreichweite
		bool rebound = false; //false ist nicht sofort aktiviert (rebound bedeutet wenn der gegner so oft wie möglich durchringen wurde
			// bis das projectil verschwindet. Jetzt verschwindet es nicht sondern prallt am letzten gegner ab. Apraller gehen immer richtung andere Gegner)
		float reboundTimes = 1.0; //wie oft er aprallt. also wie viele weiter projectile gespawnt werden müssen.

		float shild = 0.0; //wie viele schilder um dir herum schweben.

			//Generelles
			float xpAmount = 0.0; //wie viel xp / also wie viel xp du hast und die spalte weiter geht 
			float xpIndicator = 0.0; // geht nur ganze zahlen also welches level du bist
			float cooldownGeneral = 100.0; //100%

			//












	};

	//enum weaponClass
	//{

	//};
	//struct weaponClass
	//{

	//};

private:

};
// push_back() emplace_back()  []  clear() pop_back()