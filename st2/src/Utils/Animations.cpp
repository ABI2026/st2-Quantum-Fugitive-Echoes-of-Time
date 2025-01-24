#include "Animations.h"

Animations::Animations(const std::vector<int>& i_indices, const std::vector<double>& i_intervals)
	: m_indices(i_indices),m_intervals(i_intervals)
{
}

int Animations::update(double deltatime)
{

	if (!playing)
		return -1;
	m_passed_time += deltatime;
	return 1;

}

void Animations::play()
{
	playing = true;
	m_passed_time = 0;
}
