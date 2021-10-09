// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_LOG_KIT_H_
#define _LEKA_OS_LIB_LOG_KIT_H_

#include <array>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <mutex>
#include <string_view>
#include <unordered_map>

#include "rtos/Kernel.h"
#include "rtos/Mutex.h"

namespace leka {

struct logger {
	//
	// MARK: - Buffers
	//

	struct buffer {
		static inline auto timestamp = std::array<char, 32> {};
		static inline auto filename	 = std::array<char, 128> {};
		static inline auto message	 = std::array<char, 128> {};
		static inline auto output	 = std::array<char, 256> {};
	};

	//
	// MARK: - Structs
	//

	enum class level
	{
		debug,
		info,
		error,
	};

	static inline const std::unordered_map<logger::level, std::string_view> level_lut = {
		{logger::level::debug, "[DBUG]"},
		{logger::level::info, "[INFO]"},
		{logger::level::error, "[ERR ]"},
	};

	//
	// MARK: - Variables
	//

	static inline auto mutex = rtos::Mutex {};

	//
	// MARK: - Now
	//

	using now_function_t = int64_t (*)();
	static auto default_now() -> int64_t { return rtos::Kernel::Clock::now().time_since_epoch().count(); }
	static inline now_function_t now = default_now;

	template <typename now_function_t>
	static void set_now_function(now_function_t func)
	{
		now = func;
	}

	//
	// MARK: - Print
	//

	using print_function_t = void (*)(const char *, size_t);
	static void default_printf(const char *str, [[maybe_unused]] size_t size) { ::printf("%s", str); }
	static inline print_function_t print = default_printf;

	template <typename print_function_t>
	static void set_print_function(print_function_t func)
	{
		print = func;
	}

	//
	// MARK: - Format functions
	//

	static void format_time_human_readable(int64_t now)
	{
		auto ms	  = now % 1000;
		auto sec  = now / 1000;
		auto min  = sec / 60;
		auto hour = min / 60;

		// ? Format: hhh:mm:ss:μμμ e.g. 008:15:12:345
		snprintf(leka::logger::buffer::timestamp.data(), std::size(leka::logger::buffer::timestamp),
				 "%03lld:%02lld:%02lld:%03lld", hour, min % 60, sec % 60, ms);
	}

	static void format_filename_line_function(const char *filename, const int line, const char *function)
	{
		snprintf(leka::logger::buffer::filename.data(), std::size(leka::logger::buffer::filename), "[%s:%i] %s",
				 filename, line, function);
	}

	template <typename... Args>
	static void format_message(const char *message = nullptr, Args... args)
	{
		static auto format = std::array<char, 64> {};

		if (sizeof...(args) == 0) {
			if (message == nullptr || *message == '\0') {
				leka::logger::buffer::message.at(0) = '\0';
				return;
			}

			snprintf(leka::logger::buffer::message.data(), std::size(leka::logger::buffer::message), "> %s", message);
			return;
		}

		snprintf(format.data(), std::size(format), "> %s", message);
		snprintf(leka::logger::buffer::message.data(), std::size(leka::logger::buffer::message), format.data(),
				 args...);
	}

	template <typename... Args>
	static auto format_output(const char *message = nullptr, Args... args) -> int
	{
		return snprintf(leka::logger::buffer::output.data(), std::size(leka::logger::buffer::output), message, args...);
	}
};

}	// namespace leka

//
// MARK: - Macros
//

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define log_debug(str, ...)                                                                                            \
	do {                                                                                                               \
		const std::lock_guard<rtos::Mutex> lock(leka::logger::mutex);                                                  \
		leka::logger::format_time_human_readable(leka::logger::now());                                                 \
		leka::logger::format_filename_line_function(__FILENAME__, __LINE__, __FUNCTION__);                             \
		leka::logger::format_message(str, ##__VA_ARGS__);                                                              \
		auto length =                                                                                                  \
			leka::logger::format_output("%s %s %s %s\n", leka::logger::buffer::timestamp.data(),                       \
										leka::logger::level_lut.at(leka::logger::level::debug).data(),                 \
										leka::logger::buffer::filename.data(), leka::logger::buffer::message.data());  \
		leka::logger::print(leka::logger::buffer::output.data(), length);                                              \
	} while (0)

#define log_info(str, ...)                                                                                             \
	do {                                                                                                               \
		const std::lock_guard<rtos::Mutex> lock(leka::logger::mutex);                                                  \
		leka::logger::format_time_human_readable(leka::logger::now());                                                 \
		leka::logger::format_filename_line_function(__FILENAME__, __LINE__, __FUNCTION__);                             \
		leka::logger::format_message(str, ##__VA_ARGS__);                                                              \
		auto length =                                                                                                  \
			leka::logger::format_output("%s %s %s %s\n", leka::logger::buffer::timestamp.data(),                       \
										leka::logger::level_lut.at(leka::logger::level::info).data(),                  \
										leka::logger::buffer::filename.data(), leka::logger::buffer::message.data());  \
		leka::logger::print(leka::logger::buffer::output.data(), length);                                              \
	} while (0)

#define log_error(str, ...)                                                                                            \
	do {                                                                                                               \
		const std::lock_guard<rtos::Mutex> lock(leka::logger::mutex);                                                  \
		leka::logger::format_time_human_readable(leka::logger::now());                                                 \
		leka::logger::format_filename_line_function(__FILENAME__, __LINE__, __FUNCTION__);                             \
		leka::logger::format_message(str, ##__VA_ARGS__);                                                              \
		auto length =                                                                                                  \
			leka::logger::format_output("%s %s %s %s\n", leka::logger::buffer::timestamp.data(),                       \
										leka::logger::level_lut.at(leka::logger::level::error).data(),                 \
										leka::logger::buffer::filename.data(), leka::logger::buffer::message.data());  \
		leka::logger::print(leka::logger::buffer::output.data(), length);                                              \
	} while (0)

#endif	 // _LEKA_OS_LIB_LOG_KIT_H_
