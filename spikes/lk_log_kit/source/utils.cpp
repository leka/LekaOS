#include "include/utils.h"

#include "rtos/ThisThread.h"

#include "LogKit.h"

using namespace std::chrono;

[[noreturn]] void log_thread_debug()
{
	while (true) {
		auto start = rtos::Kernel::Clock::now();
		for (auto i = 1; i <= 10; ++i) {
			auto start_run = rtos::Kernel::Clock::now();
			log_debug("Run debug number: %i", i);
			log_debug("%s debug message", "First:");
			log_info("%s information message", "Second:");
			log_error("%s error message", "Third:");
			auto stop_run = rtos::Kernel::Clock::now();
			log_info("Total time to log the %i messages --> %ims\n", 4, int((stop_run - start_run).count()));
		}
		auto stop = rtos::Kernel::Clock::now();

		log_info("Total time to LOG the for loop --> %ims\n", int((stop - start).count()));

		rtos::ThisThread::sleep_for(3s);
	}
}

[[noreturn]] void log_thread_printf()
{
	auto log_printf = [](const char *str, auto... params) { printf(str, params...); };

	while (true) {
		auto start = rtos::Kernel::Clock::now();
		for (auto i = 1; i <= 10; ++i) {
			auto start_run = rtos::Kernel::Clock::now();
			log_printf("Run printf number: %i\n", i);
			log_printf("%s debug message\n", "First:");
			log_printf("%s information message\n", "Second:");
			log_printf("%s error message\n", "Third:");
			auto stop_run = rtos::Kernel::Clock::now();
			log_printf("Total time to log the %i messages --> %ims\n\n", 4, int((stop_run - start_run).count()));
		}
		auto stop = rtos::Kernel::Clock::now();

		log_printf("Total time to PRINTF the for loop --> %ims\n\n", int((stop - start).count()));

		rtos::ThisThread::sleep_for(3s);
	}
}

void log_from_isr()
{
	log_info("Log from ISR");
	auto start = rtos::Kernel::Clock::now();
	for (auto i = 1; i <= 10; ++i) {
		auto start_run = rtos::Kernel::Clock::now();
		log_debug("Run debug number: %i", i);
		log_debug("%s debug message", "First:");
		log_info("%s information message", "Second:");
		log_error("%s error message", "Third:");
		auto stop_run = rtos::Kernel::Clock::now();
		log_info("Total time to log the %i messages --> %ims\n", 4, int((stop_run - start_run).count()));
	}
	auto stop = rtos::Kernel::Clock::now();

	log_info("Total time to LOG the for loop --> %ims\n", int((stop - start).count()));
}
