#pragma once
#include <random>

#include <glm/glm.hpp>

class Random
{
public:
	static void Init()
	{
		s_random_engine.seed(std::random_device()());
	}

	static uint32_t UInt()
	{
		return s_distribution(s_random_engine);
	}

	static uint32_t UInt(const uint32_t min, const uint32_t max)
	{
		return min + (s_distribution(s_random_engine) % (max - min + 1));
	}

	static float Float()
	{
		return static_cast<float>(s_distribution(s_random_engine)) / static_cast<float>(std::numeric_limits<uint32_t>::max());
	}
	static float Float(const float min, const float max)
	{
		return Float() * (max - min) + min;
	}

	static glm::vec3 Vec3()
	{
		return {Float(), Float(), Float()};
	}

	static glm::vec3 Vec3(const float min, const float max)
	{
		return {Float(min,max), Float(min,max), Float(min,max)};
	}

	static glm::vec3 InUnitSphere()
	{
		return glm::normalize(Vec3(-1.0f, 1.0f));
	}
private:
	static std::mt19937 s_random_engine;
	static std::uniform_int_distribution<std::mt19937::result_type> s_distribution;
};



