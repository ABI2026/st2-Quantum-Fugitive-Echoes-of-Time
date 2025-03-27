#pragma once
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>

class Healthbar
{
	double m_max_hp;
	double m_current_hp;
	const sf::Font& m_font;
	const sf::Texture* m_full_hp_texture;
	const sf::Texture* m_no_hp_texture;
	sf::Text m_text;
public:
	Healthbar(double i_max_hp, double i_current_hp, const sf::Font& i_font,
	const sf::Texture* i_full_hp_texture,
	const sf::Texture* i_no_hp_texture)
	: m_max_hp(i_max_hp),
	m_current_hp(i_current_hp),
	m_font(i_font),
	m_full_hp_texture(i_full_hp_texture),
	m_no_hp_texture(i_no_hp_texture),
	m_text(m_font)
	{}

	void update(double i_max_hp, double i_current_hp);
	void render(sf::RenderTarget& target);
};


//void render_healthbar(sf::RenderTarget& target, float max_hp, float current_hp);