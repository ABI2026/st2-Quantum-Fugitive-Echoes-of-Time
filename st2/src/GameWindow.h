#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <memory>
#include "Utils/Log.h"

class GameWindow
{
    sf::RenderWindow m_window;
    inline static std::shared_ptr<GameWindow> s_instance = nullptr;

    GameWindow(const sf::VideoMode& i_mode, const std::string& i_title, const uint32_t i_style) : m_window(i_mode, i_title, i_style)
    {
    }

public:
    // Get the singleton instance
    static std::shared_ptr<GameWindow> get_instance()
    {
        if (!s_instance)
        {
            LOG_ERROR("GameWindow::init must be called before GameWindow::get_instance()");
        }
        return s_instance;
    }

    // Initialize the singleton instance
    static void init(int width, int height, const std::string& title, const uint32_t i_style = sf::Style::Default)
    {
        if (s_instance)
        {
            LOG_ERROR("Instance already exists! Use delete_instance and then init again to reload.");
            return;
        }
        s_instance = std::shared_ptr<GameWindow>(new GameWindow(sf::VideoMode(width, height), title,i_style));
    }

    // Delete the singleton instance
    static void delete_instance()
    {
        s_instance.reset();
    }

    // Get the window
    sf::RenderWindow& get_window()
    {
        return m_window;
    }
};
#define GameWindowInstance GameWindow::get_instance()
#define SFwindowInstance GameWindowInstance->get_window()