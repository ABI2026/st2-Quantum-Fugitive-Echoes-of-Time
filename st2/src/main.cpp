#include <SFML/Graphics.hpp>

#include "GameWindow.h"
#include "Utils/Soundsystem.h"
#include "Utils/Log.h"
#include "Utils/Random.h"
#include "imgui-SFML.h"
#include "imgui.h"
#include <cmath> // For sin() and cos()
#include "SFML/OpenGL.hpp"
#include "Utils/Timer.h"


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
    Log::Init(LOG_LEVEL_INFO,LOG_LEVEL_INFO);
    Random::Init();
    GameWindow::init(720, 480, "window");
    init_sfml_imgui(SFwindowInstance);
    SFwindowInstance.setFramerateLimit(60);
    sf::Clock deltaClock;
	bool left_click = false;
    bool right_click = false;
    LOG_INFO("  OpenGL Info:");
    LOG_INFO("  Vendor: {0}", (const char*)glGetString(GL_VENDOR));
    LOG_INFO("  Renderer: {0}", (const char*)glGetString(GL_RENDERER));
    LOG_INFO("  Version: {0}", (const char*)glGetString(GL_VERSION));
    while (SFwindowInstance.isOpen())
    {
        sf::Event event{};
        while (SFwindowInstance.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(SFwindowInstance, event);

            switch (event.type)
            {
            case sf::Event::Closed:
                SFwindowInstance.close();
                break;
            case sf::Event::MouseButtonReleased://fallthrough
            case sf::Event::MouseButtonPressed:
            {
	            const bool down = event.type == sf::Event::MouseButtonPressed;
                if (event.mouseButton.button == sf::Mouse::Button::Left)
                    left_click = down;
                if (event.mouseButton.button == sf::Mouse::Button::Right)
                    right_click = down;
            }
            break;
                break;
            default:
                break;
            }
        }
        const double deltatime = static_cast<double>(deltaClock.getElapsedTime().asSeconds());
        ImGui::SFML::Update(SFwindowInstance, deltaClock.restart());
        ImGui::DockSpaceOverViewport();
        ImGui::Begin("hallo welt");
        ImGui::Text("hallo welt");
        ImGui::End();

    	SFwindowInstance.clear();
        ImGui::SFML::Render(SFwindowInstance);
        SFwindowInstance.display();
        
    }

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
