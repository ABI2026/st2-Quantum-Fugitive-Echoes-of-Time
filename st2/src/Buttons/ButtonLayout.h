#pragma once

namespace sf
{
	class RenderWindow;
}

class ButtonLayout
{
public:
	virtual ~ButtonLayout() = default;
	virtual void render(sf::RenderWindow&) = 0;
};

