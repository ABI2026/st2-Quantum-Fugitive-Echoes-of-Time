#pragma once
#include <string>
#include <chrono>
#include "Log.h"


class Timer
{
public:

	Timer()
	{
		reset();
	}

	void reset()
	{
		m_start = std::chrono::high_resolution_clock::now();
	}

	[[nodiscard]] double elapsed() const
	{
		return static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - m_start).count()) * 0.001 * 0.001;
	}

	[[nodiscard]] double elapsed_millis() const
	{
		return elapsed() * 1000.0;
	}
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
};

//timer der in einer funktion verwendet werden soll um zu gucken wie lange die funktion dauert 
class ScopedTimer
{
public:
	ScopedTimer(const std::string& name)
		: m_name(name) {}
	~ScopedTimer()
	{
		double time = m_timer.elapsed_millis();
		LOG_INFO("[TIMER] {} - {} ms", m_name, time);
	}
private:
	std::string m_name;
	Timer m_timer;
};
