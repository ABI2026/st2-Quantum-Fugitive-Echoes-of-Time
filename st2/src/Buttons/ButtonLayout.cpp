#include "ButtonLayout.h"

#include "../../Resources/Images/Roboto-Regular.embed"

TextLayout::TextLayout(const std::string& i_text,
    const sf::Vector2f& i_position,
    const sf::Vector2f& i_size,
    const float i_outline_thickness,
    const sf::Color& i_default_color,
    const sf::Color& i_hovered_color,
    const sf::Color& i_pressed_color,
    const sf::Color& i_outline_color
     )
{

    m_default_color = i_default_color;
    m_hovered_color = i_hovered_color;
    m_pressed_color = i_pressed_color;

    m_shape.setPosition(i_position);
    m_shape.setSize(i_size);
    m_shape.setFillColor(i_default_color);

	m_shape.setOutlineThickness(i_outline_thickness);
    m_shape.setOutlineColor(i_outline_color);

    m_font.loadFromMemory(g_RobotoRegular, sizeof g_RobotoRegular);


    m_text.setFont(m_font);
    m_text.setString(i_text);
    m_text.setCharacterSize(24);
    m_text.setFillColor(sf::Color::Black);

    // Center text
    const sf::FloatRect text_bounds = m_text.getLocalBounds();
    m_text.setOrigin(0, 0);
    const float center_x = i_position.x + i_size.x / 2.0f;
    const float center_y = i_position.y + i_size.y / 2.0f;

    const float adjusted_x = center_x - (text_bounds.width + text_bounds.left) / 2.0f;
    const float adjusted_y = center_y - (text_bounds.height + text_bounds.top) / 2.0f;
    m_text.setPosition(std::floor(adjusted_x), std::floor(adjusted_y));

}

void TextLayout::set_position(const sf::Vector2f& position)
{
    m_shape.setPosition(position);
    const sf::Vector2f size = m_shape.getSize();
    const sf::FloatRect text_bounds = m_text.getLocalBounds();
    const float center_x = position.x + size.x / 2.0f;
    const float center_y = position.y + size.y / 2.0f;

    const float adjusted_x = center_x - (text_bounds.width + text_bounds.left) / 2.0f;
    const float adjusted_y = center_y - (text_bounds.height + text_bounds.top) / 2.0f;
    m_text.setPosition(std::floor(adjusted_x), std::floor(adjusted_y));
}

void TextLayout::render(sf::RenderWindow& window)
{
    window.draw(m_shape);
    window.draw(m_text);
}

bool TextLayout::is_hovered()
{
    return m_hovered;
}

void TextLayout::update(const sf::Vector2f& mouse_position,const bool mouse_pressed)
{
    m_hovered = m_shape.getGlobalBounds().contains(mouse_position);
    m_clicked = false;

    if(!m_hovered)
    {
        m_shape.setFillColor(m_default_color);
        return;
    }

    m_clicked = mouse_pressed;

    if(mouse_pressed)
    {
        m_shape.setFillColor(m_pressed_color);
    }
    else
    {
        m_shape.setFillColor(m_hovered_color);
    }
}

bool TextLayout::is_clicked()
{
	return m_clicked;
}

void TextLayout::set_is_hovered(const bool hovered)
{
    m_hovered = hovered;
    m_shape.setFillColor(hovered ? m_hovered_color : m_default_color);
}

void TextLayout::set_text(const std::string& i_text)
{
    m_text.setString(i_text);
    set_position(m_shape.getPosition());
}