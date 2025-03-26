#include "Expbar.h"

Expbar::Expbar() {
	//if (!m_texture->loadFromFile("....png")) { //keine Texture fuer Expbar :(
	//	this->~Expbar();
	//}
	m_exp = 250;
	m_maxExp = 500;
	m_lvl = 1;
}
Expbar::~Expbar() {
	
}
int Expbar::getExp() {
	return m_exp;
}
int Expbar::getMaxExp() {
	return m_maxExp;
}
int Expbar::getLvl() {
	return m_lvl;
}
void Expbar::setExp(int i_exp) {
	m_exp = i_exp;
}
void Expbar::setMaxExp(int i_maxExp) {
	m_maxExp = i_maxExp;
}
void Expbar::setLvl(int i_lvl) {
	m_lvl = i_lvl;
}
void Expbar::setTexture(sf::Texture* i_texture) {
	m_texture = i_texture;
}
void Expbar::update(std::shared_ptr<Eventsystem>& eventsystem, std::shared_ptr<Soundsystem>& soundsystem, double deltatime) {

}
void Expbar::draw(sf::RenderWindow& window)
{
	sf::RectangleShape background({ 500,50 });
	sf::RectangleShape border({ 520,70 });
	sf::RectangleShape expbar({500 * (float)250 / 500, 50 });

	background.setFillColor(sf::Color::Black);
	border.setFillColor(sf::Color::White);
	expbar.setFillColor(sf::Color::Green);

	background.setOrigin({background.getSize().x / 2,background.getSize().y / 2});
	border.setOrigin({ border.getSize().x / 2,border.getSize().y / 2 });

	background.setPosition({ window.getView().getCenter().x, window.getView().getCenter().y + window.getSize().y/2 - 100 });
	border.setPosition({ window.getView().getCenter().x, window.getView().getCenter().y + window.getSize().y/2 - 100 });
	expbar.setPosition({ window.getView().getCenter().x - 250, window.getView().getCenter().y + window.getSize().y/2 - 125});

	window.draw(border);
	window.draw(background);
	window.draw(expbar);
}
