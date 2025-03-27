#include "Healthbar.h"

#include <SFML/Graphics.hpp>
#include "../Resources/Images/Roboto-Regular.embed"
#include "Utils/Random.h"


void Healthbar::update(double i_max_hp, double i_current_hp)
{
	m_max_hp = i_max_hp;
	m_current_hp = i_current_hp;
}

void Healthbar::render(sf::RenderTarget& target)
{
	sf::Sprite full(*m_full_hp_texture);
	sf::Sprite empty(*m_no_hp_texture);

	constexpr sf::Vector2f size = { 500.f, 50.f };
	constexpr sf::Vector2f padding = { 0.f, 0.f };
	const sf::Vector2f pos = {target.getView().getCenter().x - target.getSize().x / 2.0f + 50,
		target.getView().getCenter().y - target.getSize().y / 2.0f + 50};
	const int offset = static_cast<int>((size.x + padding.x) * (glm::clamp(m_current_hp,0.0,m_max_hp) / m_max_hp));

	full.setTextureRect(
		sf::IntRect(
		{ 0,0 }, 
		{ offset,static_cast<int>(size.y+padding.y)}));

	empty.setTextureRect(
		sf::IntRect({ offset,0 },
			{ static_cast<int>((size.x+padding.x) - offset),static_cast<int>(size.y + padding.y) }));

	full.setPosition({ pos.x,pos.y});

	empty.setPosition({ pos.x + offset,pos.y});

	target.draw(empty);
	target.draw(full);

	const std::string health_text = std::format("{}/{}", m_current_hp, m_max_hp);

	m_text.setString(health_text);
	{
		const sf::FloatRect text_bounds = m_text.getLocalBounds();
		m_text.setOrigin({ 0, 0 });
		const float center_x = pos.x + m_full_hp_texture->getSize().x / 2u;
		const float center_y = pos.y + m_full_hp_texture->getSize().y / 2u;

		const float adjusted_x = center_x - (text_bounds.size.x + text_bounds.position.x) / 2.0f;
		const float adjusted_y = center_y - (text_bounds.size.y + text_bounds.position.y) / 2.0f;

		m_text.setPosition({ std::floor(adjusted_x), std::floor(adjusted_y) });
	}
	target.draw(m_text);
}

//sf::Font tmp_font = sf::Font(g_RobotoRegular, sizeof(g_RobotoRegular));
//
//void render_healthbar(sf::RenderTarget& target, float max_hp, float current_hp)
//{
//	constexpr sf::Vector2f size = { 300.f,40.f };
//	constexpr sf::Vector2f padding = { 8.f,8.f };
//
//	sf::RectangleShape background(size);
//	background.setFillColor(sf::Color::White);
//	background.setPosition(
//		sf::Vector2f(target.getView().getCenter().x - target.getSize().x / 2.0f + 50 - 4,
//			target.getView().getCenter().y - target.getSize().y / 2.0f + 50 - 4));
//
//
//	sf::RectangleShape health_background(size-padding);
//	health_background.setFillColor(sf::Color::Black);
//	health_background.setPosition(
//		sf::Vector2f(target.getView().getCenter().x - target.getSize().x / 2.0f + 50,
//			target.getView().getCenter().y - target.getSize().y / 2.0f + 50));
//
//	const float a = (size.x - padding.x) * current_hp / max_hp;
//	sf::RectangleShape health(sf::Vector2f(a, (size.y - padding.y)));
//	health.setFillColor(sf::Color::Red);
//	
//	health.setPosition(
//		sf::Vector2f(target.getView().getCenter().x - target.getSize().x / 2.0f + 50, 
//		target.getView().getCenter().y - target.getSize().y / 2.0f + 50));
//
//	target.draw(background);
//	target.draw(health_background);
//	target.draw(health);
//
//	const std::string health_text = std::format("{}/{}", current_hp, max_hp);
//	sf::Text text(tmp_font, health_text);
//	text.setPosition({ target.getView().getCenter().x - target.getSize().x / 2.0f + 50.f,
//		target.getView().getCenter().y - target.getSize().y / 2.0f + 50.f });
//	target.draw(text);
//}
