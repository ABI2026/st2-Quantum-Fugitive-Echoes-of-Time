#include "Expbar.h"
#include <imgui.h>

Expbar::Expbar() {
	if (!m_texture_full.loadFromFile("Resources/Images/Expbar_full.png"))
	{
		LOG_ERROR("failed loading Expbar_Full texture");
	}
	if (!m_texture_empty.loadFromFile("Resources/Images/Expbar_empty.png"))
	{
		LOG_ERROR("failed loading Expbar_Empty texture");
	}
	m_exp = 0;
	m_maxExp = 50;
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
void Expbar::update(std::shared_ptr<Eventsystem>& eventsystem, [[maybe_unused]] std::shared_ptr<Soundsystem>& soundsystem, [[maybe_unused]] double deltatime) {
	if (eventsystem->get_key_action(sf::Keyboard::Key::E)) {
		m_exp += 5;
	}
	if (this->getExp() >= this->getMaxExp()) {
		m_exp -= m_maxExp;
		m_maxExp = static_cast<int>(floor(static_cast<double>(m_maxExp) * 1.5));
		m_lvl += 1;
	}
	ImGui::Begin("Debug");
	ImGui::Text("lvl:%d exp:%d max_exp:%d", m_lvl,m_exp, m_maxExp);
	ImGui::End();
}
void Expbar::draw(sf::RenderWindow& window) {
	/*sf::RectangleShape background({ 500,50 });
	sf::RectangleShape border({ 520,70 });
	sf::RectangleShape expbar({500 * (float)m_exp / m_maxExp, 50 });

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
	window.draw(expbar);*/
	sf::Sprite full(m_texture_full);
	sf::Sprite empty(m_texture_empty);


	full.setTextureRect(sf::IntRect({ 0,0 }, { static_cast<int>(20 + 500 * (float)m_exp / m_maxExp),70 }));
	empty.setTextureRect(sf::IntRect({ static_cast<int>(20 + 500 * (float)m_exp / m_maxExp),0 }, { 520-static_cast<int>(20 + 500 * (float)m_exp / m_maxExp),70 }));

	full.setPosition({ window.getView().getCenter().x - 260, window.getView().getCenter().y + window.getSize().y / 2 - 125 });
	empty.setPosition({ window.getView().getCenter().x - 260 + static_cast<int>(20 + 500 * (float)m_exp / m_maxExp), window.getView().getCenter().y + window.getSize().y / 2 - 125 });
	
	window.draw(empty);
	window.draw(full);
}
