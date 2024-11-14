#include <SFML/Graphics.hpp>

#include "GameWindow.h"
#include "Utils/Soundsystem.h"
#include "Utils/Log.h"
#include "Utils/Random.h"
#include "imgui-SFML.h"
#include "imgui.h"
#include <cmath>

#include "Eventsystem.h"
#include "SFML/OpenGL.hpp"
#include "Utils/Timer.h"
#include "Button.h"

int Main(int argc, char** argv);

bool init_sfml_imgui(sf::RenderWindow& window);

#if (defined(PLATFORM_WINDOWS) || defined (_WIN64)) && defined(DIST)
#include <windows.h>

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
    return Main(__argc, __argv);
}

#else

int main(int argc, char** argv)
{
    return Main(argc, argv);
}

#endif

int Main(int argc, char** argv)
{
    Log::init(LOG_LEVEL_INFO,LOG_LEVEL_INFO);
    Random::init();

    GameWindow::init(720, 480, "window");

    LOG_INFO("  OpenGL Info:");
    LOG_INFO("  Vendor: {0}", (const char*)glGetString(GL_VENDOR));
    LOG_INFO("  Renderer: {0}", (const char*)glGetString(GL_RENDERER));
    LOG_INFO("  Version: {0}", (const char*)glGetString(GL_VERSION));

	sf::RenderWindow& window = SFwindowInstance;
    DBL_MAX;
	std::shared_ptr<Eventsystem> eventsystem = std::make_shared<Eventsystem>();
    init_sfml_imgui(window);
    window.setFramerateLimit(60);
    sf::Clock deltaClock;
    //variablen


        // Normaler Button mit Farbwechsel
    Button button({ 100, 100 }, { 200, 50 }, "Click me",
        sf::Color::White,      // Normal
        sf::Color::Yellow,     // Hover
        sf::Color::Red);       // Pressed

    // Bildbutton mit verschiedenen Zuständen
    ImageButton imgButton({ 100, 200 }, { 200, 50 }, "button_normal.png", "Image Button");
    imgButton.setBackgroundTexture("button_normal.png", "button_hover.png", "button_pressed.png");

    while (window.isOpen())
    {
        sf::Event event{};
        eventsystem->update(); //release events updaten
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(window, event);
            eventsystem->process_events(window, event);
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            default:
                break;
            }
        }

        const double deltatime = static_cast<double>(deltaClock.getElapsedTime().asSeconds());
        ImGui::SFML::Update(window, deltaClock.restart());
        //logik


        window.clear(sf::Color::Cyan);
        //zeichnen

        button.draw(window);
        imgButton.draw(window);

        ImGui::SFML::Render(window);
        window.display();
        
    }

    ImGui::SFML::Shutdown(window);
    GameWindow::delete_instance();
	return 0;
}

#include "../Resources/Images/Roboto-Regular.embed"
bool init_sfml_imgui(sf::RenderWindow& window)
{

    if (!ImGui::SFML::Init(window, false))
        return false;

    ImGui::SFML::ProcessEvent(window, sf::Event{sf::Event::LostFocus});
    ImGui::SFML::ProcessEvent(window, sf::Event{sf::Event::GainedFocus});

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows // funktioniert nicht weil die sfml backends das nicht haben

    ImFontConfig font_config;
    font_config.FontDataOwnedByAtlas = false;
    ImFont* roboto_font = io.Fonts->AddFontFromMemoryTTF((void*)g_RobotoRegular, sizeof(g_RobotoRegular), 20.0f, &font_config);
    io.FontDefault = roboto_font;
    if (!ImGui::SFML::UpdateFontTexture())
        LOG_ERROR("failed to update font");
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }
    return true;
}
