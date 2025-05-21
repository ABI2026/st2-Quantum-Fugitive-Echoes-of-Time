#include <iostream>
#include "Player.h"

#include <glm/geometric.hpp>
#include <glm/vec2.hpp>

#include "imgui.h"
#include "Weapon.h"
#include "Utils/Log.h"
#include "Utils/Random.h"

Player::Player(sf::Texture& texture) : m_shape({ 100.f,100.f }), m_position(320.f, 320.f), m_velocity(0.0f, 0.0f)
{
	m_speed = 0;
    //konstrukt	 ;
	//if (!m_player_texture.loadFromFile("Resources/Images/Sheet_Sprites_16Bit.png"))
	//	LOG_ERROR("failed loading player texture");
	m_shape.setTexture(&texture);
	m_shape.setTextureRect({ {0,0},{32,32} });
	m_shape.setSize({ 16*8,16*8 });
	m_shape.setOrigin(m_shape.getSize()/2.f);

	m_weapon = std::make_shared<Weapon>(200.0, 0.1f, INT_MAX, 0.5, 50.0, 400.f);
}

Player::~Player()
{
//destrukt
}

bool Player::take_damage(double damage, double crit_chance, double crit_damage)
{
	if (m_invicibility_time > 0.0)
		return false;

	m_stats.health -= damage + Random::foo(crit_chance) * crit_damage; //TODO: ACTUAL DAMAGE CALCULATION
	m_invicibility_time = 0.125;
	return true;
}

void Player::update(std::shared_ptr<Eventsystem>& eventsystem,
	[[maybe_unused]] std::shared_ptr<Soundsystem>& soundsystem,
	[[maybe_unused]] const double deltatime,
	std::shared_ptr<EnemyManager> enemy_manager,
	std::shared_ptr<ProjectileManager> projectile_manager

)
{
	static double condt = 0;
	condt += deltatime;

	m_invicibility_time -= deltatime;
	m_weapon->update(deltatime, m_position, enemy_manager, projectile_manager);
	glm::vec2 movement = { 0.f,0.f };
	int x_pos_sprite = 30;

	if (eventsystem->get_key_state(sf::Keyboard::Key::W)) 
	{
		//m_shape.setTextureRect({ {17,m_shape.getTextureRect().position.y},{16,16} });
		x_pos_sprite = 30;
		movement.y -= 1;
	}
	if (eventsystem->get_key_state(sf::Keyboard::Key::A))
	{
		//m_shape.setTextureRect({ {34,m_shape.getTextureRect().position.y},{16,16} });
		x_pos_sprite = 30;
		movement.x -= 1;
	}
	if (eventsystem->get_key_state(sf::Keyboard::Key::S))
	{
		//m_shape.setTextureRect({ {0,m_shape.getTextureRect().position.y},{16,16} });
		x_pos_sprite = 0;
		movement.y += 1;
	}
	if (eventsystem->get_key_state(sf::Keyboard::Key::D))
	{
		x_pos_sprite = 90;
		movement.x += 1;
	}

	ImGui::Begin("Debug");
	ImGui::Text("fps: %f", 1.0 / deltatime);
	if (movement != glm::vec2{ 0.f,0.f })
	{
		constexpr float animation_length = 0.125f;
		if(movement.x == 1.f)
		{
			if (m_animation_id >= 0 && m_animation_id < 10)
			{
				if (condt > animation_length)
				{
					condt -= animation_length;
					m_animation_id++;
					if (m_animation_id >= 10)
						m_animation_id = 0;
				}
			}
			else
				m_animation_id = 0;
		}
		else if(movement.x == -1.f)
		{
			if (m_animation_id >= 10 && m_animation_id < 20)
			{
				if (condt > animation_length)
				{
					condt -= animation_length;
					m_animation_id++;
					if (m_animation_id >= 20)
						m_animation_id = 10;
				}
			}
			else
				m_animation_id = 10;
		}
		else if(movement.y == 1.f)
		{
			if (m_animation_id >= 20 && m_animation_id < 30)
			{
				if (condt > animation_length)
				{
					condt -= animation_length;
					m_animation_id++;
					if (m_animation_id >= 30)
						m_animation_id = 20;
				}
			}
			else
				m_animation_id = 20;
		}
		else if(movement.y == -1.f)
		{
			if (m_animation_id >= 30 && m_animation_id < 40)
			{
				if (condt > animation_length)
				{
					condt -= animation_length;
					m_animation_id++;
					if (m_animation_id >= 40)
						m_animation_id = 30;
				}
			}
			else
				m_animation_id = 30;
		}

		movement = glm::normalize(movement);
		const auto footsteps = soundsystem->get_sound_by_group_and_id("player_sounds", 4);
		if (footsteps->empty())
			soundsystem->play_sound("player_sounds", 4);
		else if(footsteps->front().getStatus() == sf::Sound::Status::Paused)
			footsteps->front().play();

		ImGui::Text("direction: x:%f y:%f", movement.x, movement.y);

		movement = movement * 600.f * static_cast<float>(deltatime);
		ImGui::Text("movement: x:%f y:%f", movement.x, movement.y);

		glm::vec2 new_pos = { m_position.x + movement.x, m_position.y + movement.y };
		//new_pos = clamp(new_pos, { 50.f,50.f }, { 590.f,590.f });
		m_position = { new_pos.x, new_pos.y };
	}
	else
	{
		constexpr float animation_length = 0.25f;

		if(condt > animation_length)
		{
			condt -= animation_length;
			if((m_animation_id >= 0 && m_animation_id < 10) || m_animation_id == 51) //walking right
			{
				m_animation_id = 40; 
			}
			else if(m_animation_id >= 40 && m_animation_id < 44) //idle right
			{
				m_animation_id++;
				if (m_animation_id >= 44)
					m_animation_id = 40;
			}
			else if((m_animation_id >= 10 && m_animation_id < 20) || m_animation_id == 52 || m_animation_id == 53) //walking left
			{
				m_animation_id = 44;
			}
			else if (m_animation_id >= 44 && m_animation_id < 49) //idle left
			{
				m_animation_id++;
				if (m_animation_id >= 49)
					m_animation_id = 44;
			}
			else if((m_animation_id >= 20 && m_animation_id < 30) || m_animation_id == 49) //walking down
			{
				m_animation_id = 54;
			}
			else if (m_animation_id >= 54 && m_animation_id < 57) //idle down
			{
				m_animation_id++;
				if (m_animation_id >= 57)
					m_animation_id = 54;
			}
			else if((m_animation_id >= 30 && m_animation_id < 40) || m_animation_id == 50) //walking up
			{
				m_animation_id = 57;
			}
			else if (m_animation_id >= 57 && m_animation_id < 60) //idle up
			{
				m_animation_id++;
				if (m_animation_id >= 60)
					m_animation_id = 57;
			}

		}

		const auto footsteps = soundsystem->get_sound_by_group_and_id("player_sounds", 4);
		if(!footsteps->empty())
			footsteps->front().pause();

		ImGui::Text("direction: x:0.000000 y:0.000000");
		ImGui::Text("movement: x:0.000000 y:0.000000");
	}

	ImGui::Text("position: x:%f y:%f", m_position.x, m_position.y);

	int y_pos = (m_animation_id / 8) * 30;
	x_pos_sprite = (m_animation_id % 8) * 30;
	ImGui::Text("animation_id: %d x_pos: %d y_pos: %d",m_animation_id, x_pos_sprite,y_pos);
	ImGui::End();
	//if(condt > 0.25)
	//{
	//	condt = 0;
	//	y_pos += 30;
	//	
	//	//y_pos -= y_pos % 17;
	//}
	//if (x_pos_sprite == 170 - 30)
	//{
	//	y_pos = y_pos % (6 * 30);
	//}
	//else
	//{
	//	y_pos = y_pos % (5 * 30);
	//}
	m_shape.setTextureRect({ { x_pos_sprite,y_pos }, { 30,30 } });
	m_shape.setPosition(m_position);


}

void Player::updatePosition([[maybe_unused]] const sf::Vector2f& movement, [[maybe_unused]] float deltaTime)
{
	throw std::runtime_error("bitte nicht nutzen danke");

	//float length = std::sqrt(movement.x * movement.x + movement.y * movement.y);
	//if (length > 0) {
	//	//sf::Vector2f normalizedMovement = movement / length;

	//	m_position.x += movement.x * m_speed * deltaTime;
	//	m_position.y += movement.y * m_speed * deltaTime;

	//	//m_velocity = normalizedMovement * m_stats.movementSpeed;
	//	//m_position += m_velocity * deltaTime;
	//}
}

void Player::draw(sf::RenderWindow& window) const
{
	auto draw_shape = m_shape;
	draw_shape.setOrigin({ 0,0 });
	draw_shape.setPosition(
		{
		m_shape.getPosition().x - m_shape.getSize().x/2.f,
		m_shape.getPosition().y - m_shape.getSize().y/2.f
		});
	window.draw(draw_shape);
}
