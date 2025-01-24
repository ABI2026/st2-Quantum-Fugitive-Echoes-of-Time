#include "Level.h"

Level::Level(int level_id, std::shared_ptr<Soundsystem>& soundsystem)
	:m_level_id(level_id), m_soundsystem(soundsystem)
{
	soundsystem->set_music_indices({ 3,4,5 });
}

Level::~Level()
{
	m_soundsystem->set_music_indices({ 0,1,2 });
}

void Level::update([[maybe_unused]]std::shared_ptr<Soundsystem>& soundsystem, [[maybe_unused]] double deltatime)
{
}
