#pragma once
#include <memory>
#include <SFML/Graphics/RenderWindow.hpp>
#include "Eventsystem.h"

class Layer
{
public:
	virtual ~Layer() = default;

	virtual void update(std::shared_ptr<Eventsystem>&) {}
	virtual void render(sf::RenderWindow&) {}
};

