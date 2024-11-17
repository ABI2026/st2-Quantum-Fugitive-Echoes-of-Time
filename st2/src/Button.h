#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <functional>

class Button
{
protected:
    sf::RectangleShape shape;
    sf::Texture texture;
    sf::Text text;
    sf::Font font;
    bool hasTexture;
    bool isHovered;
    bool isPressed;
    sf::Color defaultColor;
    sf::Color hoverColor;
    sf::Color pressedColor;
    std::function<void()> callback;

public:
    Button(const sf::Vector2f& position, const sf::Vector2f& size,
        const std::string& buttonText = "",
        const sf::Color& normal = sf::Color::White,
        const sf::Color& hover = sf::Color(200, 200, 200),
        const sf::Color& pressed = sf::Color(150, 150, 150));

    virtual void setPosition(const sf::Vector2f& position)
    {
        shape.setPosition(position);
        const sf::Vector2f size = shape.getSize();
        const sf::FloatRect textBounds = text.getLocalBounds();
        text.setOrigin(textBounds.left + textBounds.width / 2.0f,
            textBounds.top + textBounds.height / 2.0f);
        text.setPosition(position.x + size.x / 2.0f, position.y + size.y / 2.0f);
    };
    virtual void setTexture(const std::string& texturePath) {};
    void setText(const std::string& buttonText) { text.setString(buttonText); }
    void setTextColor(const sf::Color& color) { text.setFillColor(color); }
    void setFont(const std::string& fontPath) {};
    void setCallback(std::function<void()> func) { callback = func; }
    void setColors(const sf::Color& normal, const sf::Color& hover, const sf::Color& pressed);

    virtual void update(const sf::Vector2i& mousePosition, bool isMousePressed);
    virtual bool isClicked() { return isPressed; }
    virtual bool is_hovered() { return isHovered; }

    virtual void draw(sf::RenderWindow& window) const;
};

class ImageButton : public Button {
private:
    sf::Texture backgroundTexture;
    sf::Texture hoverTexture;
    sf::Texture pressedTexture;
    bool hasBackgroundTexture;
    bool hasHoverTexture;
    bool hasPressedTexture;

public:
    ImageButton(const sf::Vector2f& position, const sf::Vector2f& size,
        const std::string& backgroundPath, const std::string& buttonText = "");
    void setBackgroundTexture(const std::string& normalPath,
        const std::string& hoverPath = "",
        const std::string& pressedPath = "");
    void setTexture(const std::string& texturePath) override {};
    void update(const sf::Vector2i& mousePosition, bool isMousePressed) override;


    void draw(sf::RenderWindow& window) const override;
};

