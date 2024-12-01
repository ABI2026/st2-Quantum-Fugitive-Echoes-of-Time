#include <cmath>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <glm/glm.hpp>
#include <imgui-SFML.h>
#include <imgui.h>


#include "Utils/Soundsystem.h"
#include "Utils/Log.h"
#include "Utils/Random.h"
#include "Utils/Timer.h"

#include "Eventsystem.h"
#include "Layer.h"
#include "LayerManager.h"
#include "Menu.h"

static int Main(int argc, char** argv);

static [[nodiscard]] bool init_sfml_imgui(sf::RenderWindow& window);

static [[nodiscard]] std::shared_ptr<Eventsystem> init_eventsystem(sf::RenderWindow& window);

static [[nodiscard]] std::shared_ptr<LayerManager> init_layer_manager();

static [[nodiscard]] std::shared_ptr<Soundsystem> init_soundsystem();

#if (defined(PLATFORM_WINDOWS) || defined (_WIN64)) && defined(DIST)
#include <windows.h>
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
    return Main(__argc, __argv);
}

#else

int main(const int argc, char** argv)
{
    return Main(argc, argv);
}

#endif



int Main(const int argc, char** argv)
{
    Log::init(LOG_LEVEL_INFO,LOG_LEVEL_INFO);
    Random::init();

    sf::RenderWindow window(sf::VideoMode(720, 480), "window", sf::Style::Default);

	if (!init_sfml_imgui(window))
    {
        LOG_ERROR("failed to initialize imgui");
    }

    LOG_INFO("OpenGL Info:");
    LOG_INFO("Vendor: {0}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
    LOG_INFO("Renderer: {0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
    LOG_INFO("Version: {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));

    std::shared_ptr<Eventsystem> eventsystem = init_eventsystem(window);
    std::shared_ptr<Soundsystem> soundsystem = init_soundsystem();
    std::shared_ptr<LayerManager> layer_manager = init_layer_manager();


	window.setFramerateLimit(60);
    sf::Clock delta_clock;
	while (window.isOpen())
    {
        eventsystem->handle_updates(window);
		const double deltatime = static_cast<double>(delta_clock.getElapsedTime().asSeconds());
		ImGui::SFML::Update(window, delta_clock.restart());

    	const std::shared_ptr<Layer> current_layer = layer_manager->get_top();
		current_layer->update(eventsystem,layer_manager,soundsystem,window,deltatime);

		soundsystem->update();

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
    
    ImGui::SFML::ProcessEvent(window, sf::Event{sf::Event::LostFocus,{}});
    ImGui::SFML::ProcessEvent(window, sf::Event{sf::Event::GainedFocus,{} });

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows // funktioniert nicht weil die sfml backends das nicht haben

    ImFontConfig font_config;
    font_config.FontDataOwnedByAtlas = false;
    ImFont* roboto_font = io.Fonts->AddFontFromMemoryTTF(const_cast<uint8_t*>(g_RobotoRegular), sizeof g_RobotoRegular, 20.0f, &font_config);
    io.FontDefault = roboto_font;
    if (!ImGui::SFML::UpdateFontTexture()) 
    {
        LOG_ERROR("failed to update font");
    }
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


std::shared_ptr<Eventsystem> init_eventsystem(sf::RenderWindow& window)
{
    auto eventsystem = std::make_shared<Eventsystem>(window);
    eventsystem->add_key_listener(sf::Keyboard::Key::W);
    eventsystem->add_key_listener(sf::Keyboard::Key::A);
    eventsystem->add_key_listener(sf::Keyboard::Key::S);
    eventsystem->add_key_listener(sf::Keyboard::Key::D);
    eventsystem->add_key_listener(sf::Keyboard::Key::Q);
    eventsystem->add_key_listener(sf::Keyboard::Key::Up);
    eventsystem->add_key_listener(sf::Keyboard::Key::Down);
    eventsystem->add_key_listener(sf::Keyboard::Key::Left);
    eventsystem->add_key_listener(sf::Keyboard::Key::Right);
    eventsystem->add_key_listener(sf::Keyboard::Key::Enter);
    eventsystem->add_key_listener(sf::Keyboard::Key::Escape);

    eventsystem->add_mouse_button_listener(sf::Mouse::Button::Left);

    return eventsystem;
}

std::shared_ptr<LayerManager> init_layer_manager()
{
    auto layer_manager = std::make_shared<LayerManager>();
    layer_manager->push_layer(std::make_shared<Menu>());
    return layer_manager;
}


std::shared_ptr<Soundsystem> init_soundsystem()
{
    auto soundsystem = std::make_shared<Soundsystem>(0.f, false);
    //TODO: ADD ADITIONAL SOUNDS
    soundsystem->add_group("ui_sounds");
    soundsystem->add_group("player_sounds");
    soundsystem->load_buffer("Resources/Sounds/Hitmarker.ogg", true, "ui_sounds");
    soundsystem->load_buffer("Resources/Sounds/background_menu_music_1.mp3",false,"music");
    soundsystem->load_buffer("Resources/Sounds/background_menu_music_2.mp3",false,"music");
    soundsystem->load_buffer("Resources/Sounds/background_menu_music_3.mp3",false,"music");
    soundsystem->set_music_indices({0, 1, 2});
    soundsystem->set_should_play_music(true);
    return soundsystem;
}