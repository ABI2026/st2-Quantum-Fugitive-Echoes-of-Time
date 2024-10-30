#pragma once
#include <string>
#include <chrono>
#include "Log.h"


class Timer
{
public:

	Timer()
	{
		Reset();
	}
	//resets the timer to 0
	void Reset()
	{
		m_start = std::chrono::high_resolution_clock::now();
	}

	//returns the amount of time passed since the last time reset has been called in seconds
	float Elapsed()
	{
		return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - m_start).count() * 0.001f * 0.001f * 0.001f;
	}

	//returns the amount of time passed since the last time reset has been called in seconds
	float ElapsedMillis()
	{
		return Elapsed() * 1000.0f;
	}
private:
	//zeitpunkt
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
		float time = m_timer.ElapsedMillis();
		LOG_INFO("[TIMER] {} - {} ms", m_name, time);
	}
private:
	std::string m_name;
	Timer m_timer;
};
