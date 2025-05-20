#include "SkillSelector.h"
#include "Eventsystem.h"
#include "LayerManager.h"
SkillSelector::SkillSelector(std::shared_ptr<Layer> i_previous_layer, std::shared_ptr<Player> i_player)
: m_previous_layer(i_previous_layer), m_player(i_player)
{
	if (!m_texture.loadFromFile("Resources/Images/SkillSelector.png"))
		LOG_ERROR("failed loading SkillSelector texture");

	// links 217;261
	// mitte 212;267
	// recht 218;267


}
void SkillSelector::update(std::shared_ptr<Eventsystem>& eventsystem, std::shared_ptr<LayerManager>& layer_manager,
	std::shared_ptr<Soundsystem>& soundsystem, sf::RenderWindow& window, double deltatime)
{
	if (eventsystem->get_key_action(sf::Keyboard::Key::Q) == Eventsystem::action_pressed)
	{
		layer_manager->pop_layer();
	}
}

void SkillSelector::render(sf::RenderWindow& window) 
{
	m_previous_layer->render(window);

	sf::RectangleShape shape(sf::Vector2f(window.getSize()));
	shape.setPosition({ 0, 0 });
	shape.setFillColor(sf::Color(128, 128, 128, 128));
	window.draw(shape);

	sf::Sprite m_sprite(m_texture);

	m_sprite.setOrigin({ m_texture.getSize().x /2.f,m_texture.getSize().y / 2.f });
	m_sprite.setPosition(sf::Vector2f(window.getSize()) / 2.f);
	window.draw(m_sprite);
}
