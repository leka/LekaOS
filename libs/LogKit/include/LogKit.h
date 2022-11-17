// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

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

namespace leka::logger {

#if defined(ENABLE_LOG_DEBUG)

//
// MARK: - Buffers
//

namespace buffer {

	inline auto timestamp = std::array<char, 32> {};
	inline auto filename  = std::array<char, 128> {};
	inline auto message	  = std::array<char, 256> {};
	inline auto output	  = std::array<char, 512> {};

	inline auto fifo		   = CircularQueue<char, 4096> {};
	inline auto process_buffer = std::array<char, 64> {};

};	 // namespace buffer

//
// MARK: - Levels
//

namespace level {

	enum class name
	{
		debug,
		info,
		error,
	};

	inline const std::unordered_map<level::name, std::string_view> lut = {
		{level::name::debug, "[DBUG]"},
		{level::name::info, "[INFO]"},
		{level::name::error, "[ERR ]"},
	};

}	// namespace level

//
// MARK: - Events, threads & locks
//

namespace internal {

	inline auto mutex		= rtos::Mutex {};
	inline auto thread		= rtos::Thread {osPriorityLow};
	inline auto event_queue = events::EventQueue {32 * EVENTS_EVENT_SIZE};

	[[maybe_unused]] inline void start_event_queue()
	{
		internal::thread.start(callback(&internal::event_queue, &events::EventQueue::dispatch_forever));
	}

}	// namespace internal

//
// MARK: - FIFO processing
//

using filehandle_ptr = mbed::FileHandle *;

namespace internal {

	inline filehandle_ptr filehandle = nullptr;

	inline void filehandle_low_level_write(const char *data, const std::size_t size)
	{
		internal::filehandle->write(data, size);
	}

	inline void disable_filehandle_input()

	{
		if (filehandle == nullptr) {
			return;
		}

		internal::filehandle->enable_input(false);
	}

	inline void enable_filehandle_input()
	{
		if (filehandle == nullptr) {
			return;
		}

		internal::filehandle->enable_input(true);
	}

}	// namespace internal

[[maybe_unused]] inline void set_filehandle_pointer(filehandle_ptr fh)
{
	internal::filehandle = fh;

	internal::disable_filehandle_input();
}

inline void process_fifo()
{
	while (!buffer::fifo.empty()) {
		auto length = buffer::fifo.pop(buffer::process_buffer.data(), std::size(buffer::process_buffer));
		internal::filehandle->write(buffer::process_buffer.data(), length);
	}
}

//
// MARK: - Serial
//

namespace internal {

	inline auto default_serial = mbed::BufferedSerial(USBTX, USBRX, 115200);

}	// namespace internal

//
// MARK: - Now
//

using now_function_t = std::function<int64_t()>;   // LCOV_EXCL_LINE

namespace internal {

	inline auto default_now_function() -> int64_t
	{
		return rtos::Kernel::Clock::now().time_since_epoch().count();
	}
	inline now_function_t now = default_now_function;

}	// namespace internal

[[maybe_unused]] inline void set_now_function(const now_function_t &func)
{
	internal::now = func;
}

//
// MARK: - Sink
//

using sink_function_t = std::function<void(const char *, std::size_t)>;	  // LCOV_EXCL_LINE

namespace internal {
	inline void default_sink_function(const char *str, [[maybe_unused]] std::size_t size)
	{
		buffer::fifo.push(std::span {str, size});
		internal::event_queue.call(process_fifo);
	}

	inline sink_function_t sink = default_sink_function;
}	// namespace internal

[[maybe_unused]] inline void set_sink_function(const sink_function_t &func)
{
	internal::sink = func;
}

//
// MARK: - Format functions
//

[[maybe_unused]] inline void format_time_human_readable(int64_t now)
{
	auto ms	  = now % 1000;
	auto sec  = now / 1000;
	auto min  = sec / 60;
	auto hour = min / 60;

	// ? Format: hhh:mm:ss.μμμ e.g. 008:15:12.345
	snprintf(buffer::timestamp.data(), std::size(buffer::timestamp), "%03lld:%02lld:%02lld.%03lld", hour, min % 60,
			 sec % 60, ms);
}

[[maybe_unused]] inline void format_filename_line_function(const char *filename, const int line, const char *function)
{
	snprintf(buffer::filename.data(), std::size(buffer::filename), "[%s:%i] %s", filename, line, function);
}

template <typename... Args>
void format_message(const char *message = nullptr, Args... args)
{
	static auto format = std::array<char, 128> {};

	if (sizeof...(args) == 0) {
		if (message == nullptr || *message == '\0') {
			buffer::message.at(0) = '\0';
			return;
		}

		snprintf(buffer::message.data(), std::size(buffer::message), "> %s", message);
		return;
	}

	snprintf(format.data(), std::size(format), "> %s", message);
	snprintf(buffer::message.data(), std::size(buffer::message), format.data(), args...);
}

template <typename... Args>
auto format_output(const char *message = nullptr, Args... args) -> int
{
	return snprintf(buffer::output.data(), std::size(buffer::output), message, args...);
}

//
// MARK: - Public functions
//

inline void init(filehandle_ptr fh			 = &internal::default_serial,
				 const sink_function_t &sink = internal::default_sink_function)
{
	set_filehandle_pointer(fh);
	set_sink_function(sink);
	internal::start_event_queue();
}

#else

// ? No op versions when debug is off
inline void init(...) {}					 // NOSONAR
inline void set_now_function(...) {}		 // NOSONAR
inline void set_sink_function(...) {}		 // NOSONAR
inline void set_print_function(...) {}		 // NOSONAR
inline void set_filehandle_pointer(...) {}	 // NOSONAR

namespace internal {

	inline void default_sink_function(...) {}	   // NOSONAR
	inline void disable_filehandle_input(...) {}   // NOSONAR
	inline void enable_filehandle_input(...) {}	   // NOSONAR

}	// namespace internal

#endif	 // ENABLE_LOG_DEBUG

}	// namespace leka::logger

//
// MARK: - Macros
//

#if defined(ENABLE_LOG_DEBUG)	// defined (ENABLE_LOG_DEBUG)

	// NOLINTNEXTLINE
	#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

	// NOLINTNEXTLINE
	#define log_debug(str, ...)                                                                                        \
		do {                                                                                                           \
			using namespace leka::logger;                                                                              \
			const std::scoped_lock lock(leka::logger::internal::mutex);                                                \
			format_time_human_readable(leka::logger::internal::now());                                                 \
			format_filename_line_function(__FILENAME__, __LINE__, __FUNCTION__);                                       \
			format_message(str, ##__VA_ARGS__);                                                                        \
			auto length = format_output("%s %s %s %s\n", leka::logger::buffer::timestamp.data(),                       \
										leka::logger::level::lut.at(leka::logger::level::name::debug).data(),          \
										leka::logger::buffer::filename.data(), leka::logger::buffer::message.data());  \
			leka::logger::internal::sink(leka::logger::buffer::output.data(), length);                                 \
		} while (0)

	// NOLINTNEXTLINE
	#define log_info(str, ...)                                                                                         \
		do {                                                                                                           \
			using namespace leka::logger;                                                                              \
			const std::scoped_lock lock(leka::logger::internal::mutex);                                                \
			format_time_human_readable(leka::logger::internal::now());                                                 \
			format_filename_line_function(__FILENAME__, __LINE__, __FUNCTION__);                                       \
			format_message(str, ##__VA_ARGS__);                                                                        \
			auto length = format_output("%s %s %s %s\n", leka::logger::buffer::timestamp.data(),                       \
										leka::logger::level::lut.at(leka::logger::level::name::info).data(),           \
										leka::logger::buffer::filename.data(), leka::logger::buffer::message.data());  \
			leka::logger::internal::sink(leka::logger::buffer::output.data(), length);                                 \
		} while (0)

	// NOLINTNEXTLINE
	#define log_error(str, ...)                                                                                        \
		do {                                                                                                           \
			using namespace leka::logger;                                                                              \
			const std::scoped_lock lock(leka::logger::internal::mutex);                                                \
			format_time_human_readable(leka::logger::internal::now());                                                 \
			format_filename_line_function(__FILENAME__, __LINE__, __FUNCTION__);                                       \
			format_message(str, ##__VA_ARGS__);                                                                        \
			auto length = format_output("%s %s %s %s\n", leka::logger::buffer::timestamp.data(),                       \
										leka::logger::level::lut.at(leka::logger::level::name::error).data(),          \
										leka::logger::buffer::filename.data(), leka::logger::buffer::message.data());  \
			leka::logger::internal::sink(leka::logger::buffer::output.data(), length);                                 \
		} while (0)

// NOLINTNEXTLINE
	#define log_free(str, ...)                                                                                         \
		do {                                                                                                           \
			using namespace leka::logger;                                                                              \
			const std::scoped_lock lock(leka::logger::internal::mutex);                                                \
			auto length = format_output(str, ##__VA_ARGS__);                                                           \
			leka::logger::internal::sink(leka::logger::buffer::output.data(), length);                                 \
		} while (0)

// NOLINTNEXTLINE
	#define log_ll(p_data, size)                                                                                       \
		do {                                                                                                           \
			using namespace leka::logger;                                                                              \
			const std::scoped_lock lock(leka::logger::internal::mutex);                                                \
			leka::logger::buffer::fifo.push(std::span {p_data, static_cast<std::size_t>(size)});                       \
			leka::logger::internal::event_queue.call(process_fifo);                                                    \
		} while (0)

#else	// not defined (ENABLE_LOG_DEBUG)

	#define log_debug(str, ...)
	#define log_info(str, ...)
	#define log_error(str, ...)
	#define log_free(str, ...)
	#define log_ll(p_data, size)

#endif	 // not defined (ENABLE_LOG_DEBUG)
