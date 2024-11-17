#include <SFML/Graphics.hpp>

#include "Utils/Soundsystem.h"
#include "Utils/Log.h"
#include "Utils/Random.h"
#include "imgui-SFML.h"
#include "imgui.h"
#include <cmath>

#include "Eventsystem.h"
#include "Layer.h"
#include "Menu.h"
#include "SFML/OpenGL.hpp"
#include "Utils/Timer.h"
#include "Button.h"
#include "LayerManager.h"

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

    sf::RenderWindow window(sf::VideoMode(720,480),"window",sf::Style::Default);

	LOG_INFO("  OpenGL Info:");
    LOG_INFO("  Vendor: {0}", (const char*)glGetString(GL_VENDOR));
    LOG_INFO("  Renderer: {0}", (const char*)glGetString(GL_RENDERER));
    LOG_INFO("  Version: {0}", (const char*)glGetString(GL_VERSION));


    std::shared_ptr<Eventsystem> eventsystem = std::make_shared<Eventsystem>(window);

    eventsystem->add_key_listener(sf::Keyboard::Key::W);
    eventsystem->add_key_listener(sf::Keyboard::Key::A);
    eventsystem->add_key_listener(sf::Keyboard::Key::S);
    eventsystem->add_key_listener(sf::Keyboard::Key::D);
    eventsystem->add_key_listener(sf::Keyboard::Key::Up);
    eventsystem->add_key_listener(sf::Keyboard::Key::Down);
    eventsystem->add_key_listener(sf::Keyboard::Key::Left);
    eventsystem->add_key_listener(sf::Keyboard::Key::Right);
    eventsystem->add_key_listener(sf::Keyboard::Key::Enter);

    eventsystem->add_mouse_button_listener(sf::Mouse::Button::Left);
    

	init_sfml_imgui(window);
    window.setFramerateLimit(60);
    sf::Clock deltaClock;

    std::shared_ptr<LayerManager> layer_manager = std::make_shared<LayerManager>();
    layer_manager->push_layer(std::make_shared<Menu>());

	while (window.isOpen())
    {
        eventsystem->handle_updates(window);

		const double deltatime = static_cast<double>(deltaClock.getElapsedTime().asSeconds());

		ImGui::SFML::Update(window, deltaClock.restart());

        const std::shared_ptr<Layer>& current_layer = layer_manager->get_top();
    	current_layer->update(eventsystem,layer_manager,deltatime);

        window.clear();
		current_layer->render(window);
    	ImGui::SFML::Render(window);
		window.display();

        if (layer_manager->is_empty())
        {
            window.close();
        }
    }

    ImGui::SFML::Shutdown(window);
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
