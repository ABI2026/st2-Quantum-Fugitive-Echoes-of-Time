#include <iostream>
#include "Player.h"

Player::Player() : m_position(0.0f, 0.0f), m_velocity(0.0f, 0.0f)
{
    //konstrukt
}

Player::~Player()
{
//destrukt
}

void Player::updatePosition(const sf::Vector2f& movement, float deltaTime)
{
    float length = std::sqrt(movement.x * movement.x + movement.y * movement.y);
    if (length > 0) {
        //sf::Vector2f normalizedMovement = movement / length;

        m_position.x += movement.x * m_speed * deltaTime;
        m_position.y += movement.y * m_speed * deltaTime;

        //m_velocity = normalizedMovement * m_stats.movementSpeed;
        //m_position += m_velocity * deltaTime;
    }
}
