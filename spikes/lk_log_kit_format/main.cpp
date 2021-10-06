// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <experimental/source_location>

#include "drivers/BufferedSerial.h"
#include "rtos/Kernel.h"
#include "rtos/ThisThread.h"

#include "LogKit.h"

using namespace leka;
using namespace mbed;
using namespace std::chrono;

namespace lstd {
using source_location = std::experimental::source_location;
}

inline consteval auto src()
{
	return lstd::source_location::current();
}

void some_code()
{
	//   std::cout << src_line() << '\n';  // This line number is output.
	// printf("line: %i\n", src());
}

template <typename source>
inline constexpr void log_format(const char *message, source s = src())
{
	// auto get_file_name = [location]() constexpr
	// {
	// 	constexpr const char *path = location.file_name();
	// 	constexpr const char *file = path;

	// 	constexpr std::string_view sv = location.file_name();
	// 	while (*path != 0U) {
	// 		if (*path++ == '/') {
	// 			file = path;
	// 		}
	// 	}
	// 	return file;
	// };

	// constexpr auto *filename = get_file_name();
	// // const auto *filename = std::filesystem::path(location.function_name()).filename().c_str();

	// printf("[%s:%u] %s --> %s\n", filename, location.line(), location.function_name(), message);

	printf("line: %i\n", s.line());
}

void func(const char *message, int number = 0)
{
	log_format(message);
}

auto main() -> int
{
	static auto serial = BufferedSerial(USBTX, USBRX, 115200);
	leka::logger::set_print_function([](const char *str, size_t size) { serial.write(str, size); });

	rtos::ThisThread::sleep_for(1s);

	while (true) {
		// auto start = rtos::Kernel::Clock::now();

		// log_debug("Message from %s --> \"%s\" at %i ms", MBED_CONF_APP_TARGET_NAME, "0123456789",
		// 		  int(rtos::Kernel::Clock::now().time_since_epoch().count()));

		// auto stop = rtos::Kernel::Clock::now();

		// log_info("log_debug took %i ms\n", int((stop - start).count()));

		auto lambda = []() { log_format("Log from lambada"); };

		log_format("Log from main");
		func("Log from func");
		lambda();
		log_debug("Log from LogKit\n");

		rtos::ThisThread::sleep_for(1000ms);
	}
}
