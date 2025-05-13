#include "Game.h"

#include <glm/glm.hpp>

#include "EnemyManager.h"
#include "Buttons/Button.h"
#include "Eventsystem.h"
#include "Expbar.h"
#include "Healthbar.h"
#include "imgui.h"
#include "LayerManager.h"
#include "PauseMenu.h"
#include "Player.h"
#include "ProjectileManager.h"
#include "Utils/Log.h"
#include <SFML/Graphics.hpp>


#include "../Resources/Images/Roboto-Regular.embed"
Game::Game(int i_level_id, std::shared_ptr<Soundsystem>& soundsystem)
{
	if(!m_sprite_sheet.loadFromFile("Resources/Images/Image (1).png"))
	{
		LOG_ERROR("error loading sprite sheet");
	}
	m_enemy_manager = std::make_shared<EnemyManager>();
	m_player = std::make_shared<Player>(m_sprite_sheet);
	m_projectile_manager = std::make_shared<ProjectileManager>();
	m_level = std::make_shared<Level>(i_level_id, soundsystem);
	m_expbar = std::make_shared<Expbar>();

	if (!m_font.openFromMemory(g_RobotoRegular, sizeof(g_RobotoRegular)))
	{
		LOG_ERROR("failed to load font");
	}

	if (!m_healthbar_full.loadFromFile("Resources/Images/Health_full.png"))
	{
		LOG_ERROR("failed loading healtbar full");
	}

	if (!m_healthbar_empty.loadFromFile("Resources/Images/Health_empty.png"))
	{
		LOG_ERROR("failed loading healtbar empty");
	}


	m_healthbar = std::make_shared<Healthbar>(m_player->getStats().maxHealth, m_player->getStats().health,m_font , &m_healthbar_full, &m_healthbar_empty);
	//soundsystem->set_music_indices({ 1,2,3 });
	
	//struktur
	std::shared_ptr<sf::Texture> texture1 = std::make_shared<sf::Texture>();
	std::shared_ptr<sf::Texture> texture2 = std::make_shared<sf::Texture>();
	//sf::Texture* texture3 = new sf::Texture();

	if (!texture1->loadFromFile("Resources/Images/struktur1.png")){ LOG_ERROR("failed to load structure 1");}
	if (!texture2->loadFromFile("Resources/Images/struktur2.png")){ LOG_ERROR("failed to load structure 2");}
	//if (!texture3->loadFromFile("Resources/Images/")) LOG_ERROR("failed to load structure 3");

	m_structure_textures = { texture1, texture2};
	constexpr sf::Vector2f desired_size = { 128.f,128.f };
	m_structures.emplace_back(texture1, sf::Vector2f(-100.f, 0.f));
	{
		const sf::Vector2f tex_size = sf::Vector2f(m_structures.back().sprite.getTexture().getSize());
		m_structures.back().sprite.setScale({ desired_size.x / tex_size.x, desired_size.y / tex_size.y });
	}
	m_structures.emplace_back(texture2, sf::Vector2f(0.f, 0.f));
	{
		const sf::Vector2f tex_size = sf::Vector2f(m_structures.back().sprite.getTexture().getSize());
		m_structures.back().sprite.setScale({ desired_size.x / tex_size.x, desired_size.y / tex_size.y });
	}
	//m_structures.emplace_back(, sf::Vector2f(100.f, 0.f));

	/*for (int i = 0; i < 2; ++i) 
	{
		sf::Sprite tx;

		tx.setTexture(*m_structure_textures[i]);
		tx.setPosition(structure_positions[i]);
		m_structures.push_back(tx);
	}*/

	//if (!m_background_texture.loadFromFile("Resources/Images/Unbasdasdenannt-1.png"))
	if (!m_background_texture.loadFromFile("Resources/Images/Hintergrund_game.png"))
	{
		LOG_ERROR("failed loading background image");
	}
	m_background_texture.setRepeated(true);
}

void Game::update(std::shared_ptr<Eventsystem>& eventsystem, std::shared_ptr<LayerManager>& layer_manager, std::shared_ptr<Soundsystem>& soundsystem, sf::RenderWindow& window, const double deltatime)
{
	m_player->update(eventsystem, soundsystem, deltatime, m_enemy_manager, m_projectile_manager);
	m_enemy_manager->update(eventsystem, soundsystem, deltatime, m_player.get(),m_expbar);
	m_projectile_manager->update(deltatime, m_enemy_manager, m_player);
	const sf::View backup = window.getView();
	m_view = backup;
	m_view.setCenter(m_player->getPosition());
	//window.setView(m_view);


	//window.setView(backup);
	m_expbar->update(eventsystem, soundsystem, deltatime);
	m_healthbar->update(m_player->getStats().maxHealth, m_player->getStats().health);
	if (eventsystem->get_key_action(sf::Keyboard::Key::R))
	{
		m_player->increase_health(-1.0);
	}

	if (eventsystem->get_key_action(sf::Keyboard::Key::T))
	{
		m_player->increase_health(1.0);
	}


	if (eventsystem->get_key_action(sf::Keyboard::Key::Escape) == Eventsystem::action_pressed || !eventsystem->has_focus())
	{
		layer_manager->push_layer(std::make_shared<PauseMenu>(layer_manager->get_top()));
		return;
	}

	if (eventsystem->get_key_action(sf::Keyboard::Key::Q) == Eventsystem::action_pressed)
	{
		//soundsystem->set_music_indices({ 0,1,2 });
		layer_manager->pop_layer();
	}

	//float blend = 1-pow(lerp_speed,deltatime);
}

void Game::render(sf::RenderWindow& window)
{
	const sf::View backup = window.getView();
	m_view.setSize(sf::Vector2f(window.getSize()));
	window.setView(m_view);

	//static sf::RectangleShape shape(sf::Vector2f(m_background_texture.getSize()));
	//shape.setPosition({ 0.f,0.f });
	//shape.setFillColor(sf::Color::Cyan);
	//shape.setTexture(&m_background_texture);
	sf::VertexArray background(sf::PrimitiveType::TriangleStrip,4);

	const sf::Vector2f view_center = m_view.getCenter();
	const sf::Vector2f view_size = m_view.getSize();

	background[0].position = sf::Vector2f(view_center.x - view_size.x / 2.0f, view_center.y - view_size.y / 2.0f); //Oben Links
	background[1].position = sf::Vector2f(view_center.x + view_size.x / 2.0f, view_center.y - view_size.y / 2.0f); //Oben Rechts
	background[2].position = sf::Vector2f(view_center.x - view_size.x / 2.0f, view_center.y + view_size.y / 2.0f); //Unten Links
	background[3].position = sf::Vector2f(view_center.x + view_size.x / 2.0f, view_center.y + view_size.y / 2.0f); //Unten Rechts

	const float texture_offset_x = (float)fmod(view_center.x, m_background_texture.getSize().x);
	const float texture_offset_y = (float)fmod(view_center.y, m_background_texture.getSize().y);
	constexpr float tiling_scale = 1.f;
	
	background[0].texCoords = sf::Vector2f(texture_offset_x, texture_offset_y);
	background[1].texCoords = sf::Vector2f(texture_offset_x + view_size.x * tiling_scale,texture_offset_y);  
	background[2].texCoords = sf::Vector2f(texture_offset_x, texture_offset_y + view_size.y * tiling_scale);  
	background[3].texCoords = sf::Vector2f(texture_offset_x + view_size.x * tiling_scale,texture_offset_y + view_size.y * tiling_scale);  

	sf::RenderStates states = sf::RenderStates::Default;
	
	states.texture = &m_background_texture;
	window.draw(background, states);

	m_enemy_manager->draw(window);

	for (const auto& structure : m_structures)
	{
		window.draw(structure.sprite);
	}
	m_player->draw(window);
	m_projectile_manager->draw(window);

	//render_healthbar(window, m_player->getStats().maxHealth, m_player->getStats().health);
	m_healthbar->render(window);
	m_expbar->draw(window);
	window.setView(backup);



}

void Game::on_close()
{
	LOG_INFO("closed game");
}

LayerID Game::get_layer_id()
{
	return LayerID::game;
}
