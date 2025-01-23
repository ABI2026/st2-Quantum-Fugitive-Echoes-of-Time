#include "ButtonBehaviour.h"

#include <SFML/Graphics.hpp>

#include "Game.h"
#include "LayerManager.h"
#include "LevelSelector.h"
#include "OptionsMenu.h"
#include "Utils/Log.h"
#include "Utils/Soundsystem.h"

bool GoBackTillLayer::on_click(std::shared_ptr<LayerManager>& layer_manager,
	[[maybe_unused]] std::shared_ptr<Soundsystem>& soundsystem,
	[[maybe_unused]] sf::RenderWindow& window)
{
	layer_manager->close_till_layer(m_layer_id);
	return true;
}

bool PopLayer::on_click(std::shared_ptr<LayerManager>& layer_manager,
	[[maybe_unused]] std::shared_ptr<Soundsystem>& soundsystem,
	[[maybe_unused]] sf::RenderWindow& window)
{
	layer_manager->pop_layer();
	return true;
}

bool AddLevelSelectLayer::on_click(std::shared_ptr<LayerManager>& layer_manager,
	[[maybe_unused]] std::shared_ptr<Soundsystem>& soundsystem,
	[[maybe_unused]] sf::RenderWindow& window)
{
	layer_manager->push_layer(std::make_shared<LevelSelector>());
	return true;
}

bool AddGameLayer::on_click(std::shared_ptr<LayerManager>& layer_manager,
	[[maybe_unused]] std::shared_ptr<Soundsystem>& soundsystem,
	[[maybe_unused]] sf::RenderWindow& window)
{
	layer_manager->push_layer(std::make_shared<Game>(m_level_id,soundsystem));
	return true;
}

bool AddOptionsMenu::on_click(std::shared_ptr<LayerManager>& layer_manager,
	[[maybe_unused]] std::shared_ptr<Soundsystem>& soundsystem,
	[[maybe_unused]] sf::RenderWindow& window)
{
	layer_manager->push_layer(std::make_shared<OptionsMenu>());
	return true;
}

bool IncrementVolume::on_click([[maybe_unused]] std::shared_ptr<LayerManager>& layer_manager,
	std::shared_ptr<Soundsystem>& soundsystem,
	[[maybe_unused]] sf::RenderWindow& window)
{
	soundsystem->increment_volume(m_volume, m_group_id);
	return false;
}
