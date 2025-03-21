#pragma once
#include <vector>
#include <string>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

#include "Eventsystem.h"
#include "LayerManager.h"
#include "Utils/Soundsystem.h"
#include "Utils/Log.h"
class Expbar
{
private:
	int m_exp;
	int m_maxExp;
	int m_lvl;
	sf::Texture m_texture_full;
	sf::Texture m_texture_empty;
public:
	Expbar();
	~Expbar();

	int getExp();
	int getMaxExp();
	int getLvl();

	void setExp(int i_exp);
	void setMaxExp(int i_maxExp);
	void setLvl(int i_lvl);

	void update(std::shared_ptr<Eventsystem>& eventsystem, std::shared_ptr<Soundsystem>& soundsystem, double deltatime);
	void draw(sf::RenderWindow& window);
};

