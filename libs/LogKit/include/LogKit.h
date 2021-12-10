// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_LOG_KIT_H_
#define _LEKA_OS_LIB_LOG_KIT_H_

#include <array>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <functional>
#include <mutex>
#include <string_view>
#include <unordered_map>

#include "drivers/BufferedSerial.h"
#include "events/EventQueue.h"
#include "platform/FileHandle.h"
#include "rtos/Kernel.h"
#include "rtos/Mutex.h"
#include "rtos/Thread.h"

#include "CircularQueue.h"

namespace leka {

namespace logger {

#if defined(ENABLE_LOG_DEBUG)

	//
	// MARK: - Buffers
	//

	namespace buffer {
		static inline auto timestamp = std::array<char, 32> {};
		static inline auto filename	 = std::array<char, 128> {};
		static inline auto message	 = std::array<char, 128> {};
		static inline auto output	 = std::array<char, 256> {};

		static inline auto fifo = CircularQueue<char, 4096> {};
	};	 // namespace buffer

	//
	// MARK: - Levels
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
	// MARK: - Events, threads & locks
	//

	static inline auto mutex	   = rtos::Mutex {};
	static inline auto thread	   = rtos::Thread {osPriorityLow};
	static inline auto event_queue = events::EventQueue {32 * EVENTS_EVENT_SIZE};

	[[maybe_unused]] static void start_event_queue()
	{
		logger::thread.start(callback(&logger::event_queue, &events::EventQueue::dispatch_forever));
	}

	//
	// MARK: - FIFO processing
	//

	using filehandle_ptr = mbed::FileHandle *;

	static inline filehandle_ptr filehandle = nullptr;

	[[maybe_unused]] static void set_filehandle_pointer(filehandle_ptr fh) { filehandle = fh; }

	static void process_fifo()
	{
		while (!logger::buffer::fifo.empty()) {
			auto c = char {};
			logger::buffer::fifo.pop(c);
			filehandle->write(&c, 1);
		}
	}

	//
	// MARK: - Serial
	//

	static auto default_serial = mbed::BufferedSerial(USBTX, USBRX, 115200);

	//
	// MARK: - Now
	//

	using now_function_t = std::function<int64_t()>;   // LCOV_EXCL_LINE

	static auto default_now_function() -> int64_t { return rtos::Kernel::Clock::now().time_since_epoch().count(); }
	static inline now_function_t now = default_now_function;

	[[maybe_unused]] static void set_now_function(const now_function_t &func) { now = func; }

	//
	// MARK: - Sink
	//

	using sink_function_t = std::function<void(const char *, size_t)>;	 // LCOV_EXCL_LINE

	static void default_sink_function(const char *str, [[maybe_unused]] size_t size)
	{
		logger::buffer::fifo.push(str, size);
		logger::event_queue.call(process_fifo);
	}

	static inline sink_function_t sink = default_sink_function;

	[[maybe_unused]] static void set_sink_function(const sink_function_t &func) { logger::sink = func; }

	//
	// MARK: - Format functions
	//

	[[maybe_unused]] static void format_time_human_readable(int64_t now)
	{
		auto ms	  = now % 1000;
		auto sec  = now / 1000;
		auto min  = sec / 60;
		auto hour = min / 60;

		// ? Format: hhh:mm:ss:μμμ e.g. 008:15:12:345
		snprintf(leka::logger::buffer::timestamp.data(), std::size(leka::logger::buffer::timestamp),
				 "%03lld:%02lld:%02lld:%03lld", hour, min % 60, sec % 60, ms);
	}

	[[maybe_unused]] static void format_filename_line_function(const char *filename, const int line,
															   const char *function)
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

	//
	// MARK: - Public functions
	//

	static void init(const filehandle_ptr fh	 = &logger::default_serial,
					 const sink_function_t &sink = logger::default_sink_function)
	{
		logger::set_filehandle_pointer(fh);
		logger::set_sink_function(sink);
		logger::start_event_queue();
	}

#else

	// ? No op versions when debug is off
	static void init(...) {}					 // NOSONAR
	static void set_now_function(...) {}		 // NOSONAR
	static void set_sink_function(...) {}		 // NOSONAR
	static void set_print_function(...) {}		 // NOSONAR
	static void default_now_function(...) {}	 // NOSONAR
	static void default_sink_function(...) {}	 // NOSONAR
	static void set_filehandle_pointer(...) {}	 // NOSONAR

#endif	 // ENABLE_LOG_DEBUG

};	 // namespace logger

}	// namespace leka

//
// MARK: - Macros
//

#if defined(ENABLE_LOG_DEBUG)	// defined (ENABLE_LOG_DEBUG)

	// NOLINTNEXTLINE
	#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

	// NOLINTNEXTLINE
	#define log_debug(str, ...)                                                                                        \
		do {                                                                                                           \
			const std::lock_guard<rtos::Mutex> lock(leka::logger::mutex);                                              \
			leka::logger::format_time_human_readable(leka::logger::now());                                             \
			leka::logger::format_filename_line_function(__FILENAME__, __LINE__, __FUNCTION__);                         \
			leka::logger::format_message(str, ##__VA_ARGS__);                                                          \
			auto length = leka::logger::format_output("%s %s %s %s\n", leka::logger::buffer::timestamp.data(),         \
													  leka::logger::level_lut.at(leka::logger::level::debug).data(),   \
													  leka::logger::buffer::filename.data(),                           \
													  leka::logger::buffer::message.data());                           \
			leka::logger::sink(leka::logger::buffer::output.data(), length);                                           \
		} while (0)

	// NOLINTNEXTLINE
	#define log_info(str, ...)                                                                                         \
		do {                                                                                                           \
			const std::lock_guard<rtos::Mutex> lock(leka::logger::mutex);                                              \
			leka::logger::format_time_human_readable(leka::logger::now());                                             \
			leka::logger::format_filename_line_function(__FILENAME__, __LINE__, __FUNCTION__);                         \
			leka::logger::format_message(str, ##__VA_ARGS__);                                                          \
			auto length = leka::logger::format_output("%s %s %s %s\n", leka::logger::buffer::timestamp.data(),         \
													  leka::logger::level_lut.at(leka::logger::level::info).data(),    \
													  leka::logger::buffer::filename.data(),                           \
													  leka::logger::buffer::message.data());                           \
			leka::logger::sink(leka::logger::buffer::output.data(), length);                                           \
		} while (0)

	// NOLINTNEXTLINE
	#define log_error(str, ...)                                                                                        \
		do {                                                                                                           \
			const std::lock_guard<rtos::Mutex> lock(leka::logger::mutex);                                              \
			leka::logger::format_time_human_readable(leka::logger::now());                                             \
			leka::logger::format_filename_line_function(__FILENAME__, __LINE__, __FUNCTION__);                         \
			leka::logger::format_message(str, ##__VA_ARGS__);                                                          \
			auto length = leka::logger::format_output("%s %s %s %s\n", leka::logger::buffer::timestamp.data(),         \
													  leka::logger::level_lut.at(leka::logger::level::error).data(),   \
													  leka::logger::buffer::filename.data(),                           \
													  leka::logger::buffer::message.data());                           \
			leka::logger::sink(leka::logger::buffer::output.data(), length);                                           \
		} while (0)

#else	// not defined (ENABLE_LOG_DEBUG)

	#define log_debug(str, ...)
	#define log_info(str, ...)
	#define log_error(str, ...)

#endif	 // not defined (ENABLE_LOG_DEBUG)

#endif	 // _LEKA_OS_LIB_LOG_KIT_H_
