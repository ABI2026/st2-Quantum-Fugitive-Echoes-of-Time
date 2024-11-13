#pragma once
#include <any>
#include <optional>
#include <variant>
#include <SFML/Graphics/RenderWindow.hpp>

#include "Eventsystem.h"

class Menu
{
	int selected = -1;
public:

	void update(std::shared_ptr<Eventsystem>& eventsystem)
	{
		const sf::Vector2f mouse_pos = eventsystem->get_mouse_position();
	}

	void render(sf::RenderWindow& window)
	{

	}
};

