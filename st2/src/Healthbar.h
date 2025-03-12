#pragma once
#include <SFML/Graphics/RenderTarget.hpp>

class Healthbar
{
};


void render_healthbar(sf::RenderTarget& target, float max_hp, float current_hp);