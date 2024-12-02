#include "Random.h"


thread_local std::mt19937 Random::s_random_engine;  // NOLINT(cert-msc51-cpp)
thread_local std::uniform_int_distribution<std::mt19937::result_type> Random::s_distribution;
