#include "Button.h"

//#include "../Resources/Images/Roboto-Regular.embed"
//
//Button::Button(const sf::Vector2f& position, const sf::Vector2f& size,
//    const std::string& buttonText, const sf::Color& normal,
//    const sf::Color& hover, const sf::Color& pressed)
//    : hasTexture(false), isHovered(false), isPressed(false),
//    defaultColor(normal), hoverColor(hover), pressedColor(pressed) {
//
//    shape.setPosition(position);
//    shape.setSize(size);
//    shape.setFillColor(defaultColor);
//    shape.setOutlineThickness(2);
//    shape.setOutlineColor(sf::Color::Black);
//    
//    font.loadFromMemory(g_RobotoRegular,sizeof(g_RobotoRegular));
//
//
//    text.setFont(font);
//    text.setString(buttonText);
//    text.setCharacterSize(24);
//    text.setFillColor(sf::Color::Black);
//
//    // Center text
//    const sf::FloatRect textBounds = text.getLocalBounds();
//    text.setOrigin(0, 0);
//	const float center_x = position.x + size.x / 2.0f;
//    const float center_y = position.y + size.y / 2.0f;
//
//    const float adjusted_x = center_x - (textBounds.width + textBounds.left) / 2.0f;
//    const float adjusted_y = center_y - (textBounds.height + textBounds.top) / 2.0f;
//    text.setPosition(std::floor(adjusted_x), std::floor(adjusted_y));
//}
//
//void Button::setPosition(const sf::Vector2f& position)
//{
//	shape.setPosition(position);
//	const sf::Vector2f size = shape.getSize();
//	const sf::FloatRect textBounds = text.getLocalBounds();
//    const float center_x = position.x + size.x / 2.0f;
//    const float center_y = position.y + size.y / 2.0f;
//
//    const float adjusted_x = center_x - (textBounds.width + textBounds.left) / 2.0f;
//    const float adjusted_y = center_y - (textBounds.height + textBounds.top) / 2.0f;
//    text.setPosition(std::floor(adjusted_x), std::floor(adjusted_y));
//}
//
//void Button::setColors(const sf::Color& normal, const sf::Color& hover, const sf::Color& pressed) {
//    defaultColor = normal;
//    hoverColor = hover;
//    pressedColor = pressed;
//}
//
//void Button::update(const sf::Vector2i& mousePosition, bool isMousePressed) {
//    isHovered = shape.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y));
//
//    if (isHovered) {
//        if (isMousePressed) {
//            shape.setFillColor(pressedColor);
//            isPressed = true;
//        }
//        else {
//            shape.setFillColor(hoverColor);
//            isPressed = false;
//        }
//    }
//    else {
//        shape.setFillColor(defaultColor);
//        isPressed = false;
//    }
//}
//
//
//// ImageButton Implementation
//ImageButton::ImageButton(const sf::Vector2f& position, const sf::Vector2f& size,
//    const std::string& backgroundPath, const std::string& buttonText)
//    : Button(position, size, buttonText),
//    hasBackgroundTexture(false), hasHoverTexture(false), hasPressedTexture(false) {
//
//    setBackgroundTexture(backgroundPath);
//}
//
//void ImageButton::setBackgroundTexture(const std::string& normalPath,
//    const std::string& hoverPath,
//    const std::string& pressedPath) {
//    if (backgroundTexture.loadFromFile(normalPath)) {
//        shape.setTexture(&backgroundTexture);
//        hasBackgroundTexture = true;
//    }
//
//    if (!hoverPath.empty() && hoverTexture.loadFromFile(hoverPath)) {
//        hasHoverTexture = true;
//    }
//
//    if (!pressedPath.empty() && pressedTexture.loadFromFile(pressedPath)) {
//        hasPressedTexture = true;
//    }
//}
//
//void ImageButton::update(const sf::Vector2i& mousePosition, bool isMousePressed) {
//    isHovered = shape.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y));
//
//    if (isHovered) {
//        if (isMousePressed && hasPressedTexture) {
//            shape.setTexture(&pressedTexture);
//            isPressed = true;
//        }
//        else if (hasHoverTexture) {
//            shape.setTexture(&hoverTexture);
//            isPressed = false;
//        }
//    }
//    else {
//        shape.setTexture(&backgroundTexture);
//        isPressed = false;
//    }
//}
//
//
//void Button::draw(sf::RenderWindow& window) const {
//    window.draw(shape);
//    window.draw(text);
//}
//
//void ImageButton::draw(sf::RenderWindow& window) const {
//    window.draw(shape);
//    window.draw(text);
//}
