#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>

namespace sf
{
	class RenderWindow;
}

class ButtonLayout
{
public:
	virtual ~ButtonLayout() = default;
	virtual void update(sf::Vector2f& mouse_position,bool mouse_pressed) = 0;
	virtual void set_position(sf::Vector2f position) = 0;
	virtual bool is_hovered() = 0;
	virtual bool is_clicked() = 0;
	virtual void set_is_hovered(bool hovered) = 0;
	virtual void render(sf::RenderWindow& window) = 0;
};

class TextLayout : public ButtonLayout
{
public:
	TextLayout(
		const std::string& i_text,
		const sf::Vector2f& i_position,
		const sf::Vector2f& i_size,
		const sf::Color& i_default_color = sf::Color::White,
		const sf::Color& i_hovered_color = sf::Color(200, 200, 200),
		const sf::Color& i_pressed_color = sf::Color(150,150,150),
		const sf::Color& i_outline_color = sf::Color::Black,
		float i_outline_thickness = 2.f
	);
	void update(sf::Vector2f& mouse_position, bool mouse_pressed) override;
	void set_position(sf::Vector2f position) override;
	bool is_hovered() override;
	bool is_clicked() override;
	void set_is_hovered(bool hovered) override;
	void render(sf::RenderWindow& window) override;
private:
	sf::Text m_text;
	sf::Font m_font;
	sf::RectangleShape m_shape;
	bool m_hovered{ false };
	bool m_clicked{ false };
	sf::Color m_default_color;
	sf::Color m_hovered_color;
	sf::Color m_pressed_color;
};
