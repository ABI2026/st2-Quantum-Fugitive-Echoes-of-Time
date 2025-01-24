#pragma once
#include "Utils/Soundsystem.h"

class Level
{
public:
	Level(int level_id, std::shared_ptr<Soundsystem>& soundsystem);
	~Level();
	void update(std::shared_ptr<Soundsystem>& soundsystem, double deltatime);
private:
	int m_level_id = 0;
	std::shared_ptr<Soundsystem> m_soundsystem;

};

