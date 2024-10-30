#pragma once
#include <memory>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"


class Log
{
public:
	static void Init(spdlog::level::level_enum client_level,spdlog::level::level_enum system_level)
	{
		spdlog::set_pattern("%^[%T] [thread %t] %n: %v%$");
		s_system_logger = spdlog::stdout_color_mt("soundsystem");
		s_system_logger->set_level(system_level);
		s_client_logger = spdlog::stdout_color_mt("client");
		s_system_logger->set_level(client_level);
	}
	static std::shared_ptr<spdlog::logger>& GetLogger() { return s_system_logger; }
private:
	inline static std::shared_ptr<spdlog::logger> s_system_logger;
	inline static std::shared_ptr<spdlog::logger> s_client_logger;
};

#ifdef DIST
#define LOG_TRACE(...)      
#define LOG_DEBUG(...)      
#define LOG_INFO(...)       
#define LOG_WARN(...)       
#define LOG_ERROR(...)      
#define LOG_CRITICAL(...)
#define GET_LOG_LEVEL -1
#define SET_LOG_LEVEL(...)
#else
#define LOG_TRACE(...)         ::Log::GetLogger()->trace(__VA_ARGS__)
#define LOG_DEBUG(...)         ::Log::GetLogger()->debug(__VA_ARGS__)
#define LOG_INFO(...)          ::Log::GetLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)          ::Log::GetLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)         ::Log::GetLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...)      ::Log::GetLogger()->critical(__VA_ARGS__)
#define GET_LOG_LEVEL		   ::Log::GetLogger()->level()
#define SET_LOG_LEVEL(...)	   ::Log::GetLogger()->set_level(__VA_ARGS__)
#endif

#define LOG_LEVEL_TRACE spdlog::level::trace
#define LOG_LEVEL_DEBUG spdlog::level::debug
#define LOG_LEVEL_INFO spdlog::level::info
#define LOG_LEVEL_WARN spdlog::level::warn
#define LOG_LEVEL_ERROR spdlog::level::err
#define LOG_LEVEL_CRITICAL spdlog::level::critical