// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <array>
#include <filesystem>

#include "drivers/AnalogOut.h"
#include "drivers/DigitalOut.h"
#include "platform/mbed_wait_api.h"
#include "rtos/ThisThread.h"
#include "rtos/Thread.h"

#include "FATFileSystem.h"
#include "FileManagerKit.h"
#include "LogKit.h"
#include "SDBlockDevice.h"

using namespace leka;
using namespace std::chrono_literals;

#include "mbed_stats.h"

#include "drivers/Watchdog.h"
namespace {

namespace watchdog {

	namespace internal {

		auto &instance		   = mbed::Watchdog::get_instance();
		constexpr auto timeout = 30000ms;
		auto thread			   = rtos::Thread {osPriorityLow};

		namespace stats {

			auto cpu   = mbed_stats_cpu_t {};
			auto stack = mbed_stats_stack_t {};
			auto heap  = mbed_stats_heap_t {};

		}	// namespace stats

		__attribute__((noreturn)) void watchdog_kick()
		{
			static auto kick_count = uint32_t {0};

			static auto start = rtos::Kernel::Clock::now();
			static auto stop  = rtos::Kernel::Clock::now();
			static auto delta = static_cast<int>((stop - start).count());

			static auto idle_ratio		 = uint8_t {};
			static auto sleep_ratio		 = uint8_t {};
			static auto deep_sleep_ratio = uint8_t {};

			static auto stack_used_delta	= int32_t {};
			static auto stack_used_size		= uint32_t {};
			static auto stack_reserved_size = uint32_t {};
			static auto stack_used_ratio	= uint8_t {};

			static auto heap_used_delta	   = int32_t {};
			static auto heap_used_size	   = uint32_t {};
			static auto heap_reserved_size = uint32_t {};
			static auto heap_used_ratio	   = uint8_t {};

			while (true) {
				internal::instance.kick();
				++kick_count;

				stop  = rtos::Kernel::Clock::now();
				delta = static_cast<int>((stop - start).count());

				mbed_stats_cpu_get(&stats::cpu);

				idle_ratio	= static_cast<uint8_t>(((stats::cpu.idle_time / 1000 * 100) / (stats::cpu.uptime / 1000)));
				sleep_ratio = static_cast<uint8_t>(((stats::cpu.sleep_time / 1000 * 100) / (stats::cpu.uptime / 1000)));
				deep_sleep_ratio =
					static_cast<uint8_t>(((stats::cpu.deep_sleep_time / 1000 * 100) / (stats::cpu.uptime / 1000)));

				mbed_stats_stack_get(&stats::stack);

				stack_used_delta	= static_cast<int32_t>(stats::stack.max_size - stack_used_size);
				stack_used_size		= stats::stack.max_size;
				stack_reserved_size = stats::stack.reserved_size;
				stack_used_ratio	= static_cast<uint8_t>((stack_used_size * 100) / stack_reserved_size);

				mbed_stats_heap_get(&stats::heap);

				heap_used_delta	   = static_cast<int32_t>(stats::heap.current_size - heap_used_size);
				heap_used_size	   = stats::heap.current_size;
				heap_reserved_size = stats::heap.reserved_size;
				heap_used_ratio	   = static_cast<uint8_t>((heap_used_size * 100) / heap_reserved_size);

				log_info(
					"dt: %i, kck: %u, idl: %u%%, slp: %u%%, dsl: %u%%, sur: %u%% (%+i)[%u/"
					"%u], hur: %u%% (%+i)[%u/%u]",
					delta, kick_count, idle_ratio, sleep_ratio, deep_sleep_ratio, stack_used_ratio, stack_used_delta,
					stack_used_size, stack_reserved_size, heap_used_ratio, heap_used_delta, heap_used_size,
					heap_reserved_size);

				start = rtos::Kernel::Clock::now();
				rtos::ThisThread::sleep_for(5s);
			}
		}

	}	// namespace internal

	void start()
	{
		internal::instance.start(internal::timeout.count());
		internal::thread.start(watchdog::internal::watchdog_kick);
	}

}	// namespace watchdog

}	// namespace

auto sd_bd = SDBlockDevice {SD_SPI_MOSI, SD_SPI_MISO, SD_SPI_SCK};
auto fatfs = FATFileSystem {"fs"};

const auto sound_file_path = std::filesystem::path {"/fs/home/wav/fur-elise.wav"};
auto file				   = FileManagerKit::File {sound_file_path};

auto thread_audio = rtos::Thread {};

auto audio_enable = mbed::DigitalOut {SOUND_ENABLE, 1};
auto audio_output = mbed::AnalogOut {MCU_SOUND_OUT};

void initializeSD()
{
	constexpr auto default_sd_bd_frequency = uint64_t {25'000'000};

	sd_bd.init();
	sd_bd.frequency(default_sd_bd_frequency);

	fatfs.mount(&sd_bd);
}

void playSound()
{
	static const auto _n_bytes_to_read = int {512};	  // arbitrary
	auto _buffer					   = std::array<uint8_t, _n_bytes_to_read> {0};

	auto _ns_sample_rate		 = uint32_t {22676};		// 1,000,000,000 / 44,100 (in ns)
	auto _ns_sample_rate_adapted = _ns_sample_rate * 1.7;	// arbitrary, 1s in MCU is not exactly 1s in real life
	auto bytesread				 = uint32_t {_n_bytes_to_read};

	/* START READ WAV */
	while (bytesread == _n_bytes_to_read) {
		// Read "_n_bytes_to_read" from file at each iteration. Real bytes read is given by "bytesread"
		if (bytesread = file.read(_buffer.data(), _n_bytes_to_read); bytesread != 0) {
			// Play every 2-bytes (sound encoded in 16 bits)
			for (uint32_t j = 0; j < bytesread; j += 4) {	// Play one channel, data for stereo are alternate
				audio_output.write_u16((_buffer.at(j + 1) + 0x8000) >>
									   1);	 // offset for int16 data (0x8000) and volume 50% (>>1)

				wait_ns(_ns_sample_rate_adapted);	// adjust play speed
			}
		}
	}
	/* END READ WAV*/
}

auto main() -> int
{
	logger::init();
	watchdog::start();

	log_info("Hello, World!\n\n");

	initializeSD();

	if (FileManagerKit::file_is_missing(sound_file_path)) {
		return 1;
	}

	{
		file.open(sound_file_path);
		// playSound();
		audio_output.write_u16(0xFFFF);
		file.close();

		rtos::ThisThread::sleep_for(1s);
	}

	while (true) {
		rtos::ThisThread::sleep_for(10min);
	}
}
