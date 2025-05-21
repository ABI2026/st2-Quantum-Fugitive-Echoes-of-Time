#pragma once
#include <SFML/Graphics.hpp>

class Animations
{
	std::vector<int> m_indices;
	std::vector<double> m_intervals;
	int current = 0;
	double m_passed_time = 0;
	bool playing = false;
public:
	Animations(const std::vector<int>& i_indices, const std::vector<double>& i_intervals);
	int update(double deltatime);
	void play();
};
