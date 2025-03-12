#include "Healthbar.h"

#include <SFML/Graphics.hpp>
#include "../Resources/Images/Roboto-Regular.embed"
#include "Utils/Random.h"

sf::Font tmp_font = sf::Font(g_RobotoRegular, sizeof(g_RobotoRegular));

void render_healthbar(sf::RenderTarget& target, float max_hp, float current_hp)
{
	constexpr sf::Vector2f size = { 300.f,40.f };
	constexpr sf::Vector2f padding = { 8.f,8.f };

	sf::RectangleShape background(size);
	background.setFillColor(sf::Color::White);
	background.setPosition(
		sf::Vector2f(target.getView().getCenter().x - target.getSize().x / 2.0f + 50 - 4,
			target.getView().getCenter().y - target.getSize().y / 2.0f + 50 - 4));


	sf::RectangleShape health_background(size-padding);
	health_background.setFillColor(sf::Color::Black);
	health_background.setPosition(
		sf::Vector2f(target.getView().getCenter().x - target.getSize().x / 2.0f + 50,
			target.getView().getCenter().y - target.getSize().y / 2.0f + 50));

	const float a = (size.x - padding.x) * current_hp / max_hp;
	sf::RectangleShape health(sf::Vector2f(a, (size.y - padding.y)));
	health.setFillColor(sf::Color::Red);
	
	health.setPosition(
		sf::Vector2f(target.getView().getCenter().x - target.getSize().x / 2.0f + 50, 
		target.getView().getCenter().y - target.getSize().y / 2.0f + 50));

	target.draw(background);
	target.draw(health_background);
	target.draw(health);

	const std::string health_text = std::format("{}/{}", current_hp, max_hp);
	sf::Text text(tmp_font, health_text);
	text.setPosition({ target.getView().getCenter().x - target.getSize().x / 2.0f + 50.f,
		target.getView().getCenter().y - target.getSize().y / 2.0f + 50.f });
	target.draw(text);
}
