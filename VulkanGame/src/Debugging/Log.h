#pragma once
#include "Logger.h"

#ifndef DISTRIBUTION_CONFIG
	#ifdef PLATFORM_WINDOWS
		#define DEBUGBREAK() __debugbreak();
	#endif

	#define TRACE(...)	Logger::Trace(__VA_ARGS__)
	#define DEBUG(...)	Logger::Debug(__VA_ARGS__)
	#define INFO(...)	Logger::Info(__VA_ARGS__)
	#define WARN(...)	Logger::Warn(__VA_ARGS__)
	#define ERROR(...)	Logger::Error(__VA_ARGS__)

	#define ASSERT(con, msg, ...) { if (!(con)) { ERROR(msg, __VA_ARGS__); DEBUGBREAK(); } }
	#define ASSERT(con, msg) { if (!(con)) { ERROR(msg); DEBUGBREAK(); } }
#else
	#define TRACE(...)
	#define DEBUG(...)
	#define INFO(...)
	#define WARN(...)
	#define ERROR(...)

	#define ASSERT(con, msg, ...) con
	#define ASSERT(con, msg) con
	#define DEBUGBREAK()
#endif