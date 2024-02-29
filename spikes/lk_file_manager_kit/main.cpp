// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <cstddef>

#include "drivers/BufferedSerial.h"
#include "rtos/ThisThread.h"

#include "FATFileSystem.h"
#include "FileManagerKit.h"
#include "HelloWorld.h"
#include "LogKit.h"
#include "SDBlockDevice.h"
#include "filesystem"

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

auto file									  = FileManagerKit::File {};
const auto seek_temp						  = uint8_t {7};
const std::filesystem::path tmp_dir			  = "/fs/tmp/";
const std::filesystem::path test_dir		  = tmp_dir / "test/";
const std::filesystem::path sub_test_dir	  = test_dir / "sub/";
const std::filesystem::path test_filename	  = test_dir / "test_spike_lk_filesystem_kit";
const std::filesystem::path sub_test_filename = sub_test_dir / "test_spike_lk_filesystem_kit";

const auto file_for_sha256_path = std::filesystem::path {"fs/usr/os/LekaOS-1.0.0.bin"};

SDBlockDevice sd_blockdevice(SD_SPI_MOSI, SD_SPI_MISO, SD_SPI_SCK);
FATFileSystem fatfs("fs");

void initializeSD()
{
	constexpr auto default_sd_blockdevice_frequency = uint64_t {25'000'000};

	sd_blockdevice.init();
	sd_blockdevice.frequency(default_sd_blockdevice_frequency);

	fatfs.mount(&sd_blockdevice);
}

void printSHA256(std::span<uint8_t> array)
{
	for (size_t i = 0; i < std::size(array); i++) {
		if (array[i] < 0xF) {
			printf("0%x", array[i]);
		} else {
			printf("%x", array[i]);
		}
	}
	printf("\n");
}

auto main() -> int
{
	logger::init();
	watchdog::start();

	log_info("Hello, World!\n\n");

	auto start = rtos::Kernel::Clock::now();

	auto hello = HelloWorld();

	rtos::ThisThread::sleep_for(1s);

	hello.start();

	initializeSD();

	rtos::ThisThread::sleep_for(1s);

	if (FileManagerKit::file_exists(file_for_sha256_path)) {
		file.open(file_for_sha256_path);
		auto sha256 = file.getSHA256();
		printSHA256(sha256);
	}

	{
		auto t = rtos::Kernel::Clock::now() - start;
		log_info("A message from your board %s --> \"%s\" at %i s", MBED_CONF_APP_TARGET_NAME, hello.world,
				 int(t.count() / 1000));
		rtos::ThisThread::sleep_for(1s);

		if (auto removed = std::filesystem::remove(test_filename); !removed) {
			log_error("The file or directory '%s' doesn't exist or fails to be removed", test_filename.c_str());
		} else {
			log_info("File or directory '%s' removed", test_filename.c_str());
		}
		rtos::ThisThread::sleep_for(1s);

		if (auto removed = std::filesystem::remove(sub_test_dir); !removed) {
			log_error("The file or directory '%s' doesn't exist or fails to be removed", sub_test_dir.c_str());
		} else {
			log_info("File or directory '%s' removed", sub_test_dir.c_str());
		}
		rtos::ThisThread::sleep_for(1s);

		if (auto removed = std::filesystem::remove(test_dir); !removed) {
			log_error("The file or directory '%s' doesn't exist or fails to be removed", test_dir.c_str());
		} else {
			log_info("File or directory '%s' removed", test_dir.c_str());
		}
		rtos::ThisThread::sleep_for(1s);

		if (auto created = FileManagerKit::create_directory(test_dir); !created) {
			log_error("The directory '%s' already exists or fails to be created ", test_dir.c_str());
		} else {
			log_info("Directory '%s' created", test_dir.c_str());
		}
		rtos::ThisThread::sleep_for(1s);

		if (auto created = FileManagerKit::create_directory(sub_test_dir); !created) {
			log_error("The directory '%s' already exists or fails to be created ", sub_test_dir.c_str());
		} else {
			log_info("Directory '%s' created", sub_test_dir.c_str());
		}
		rtos::ThisThread::sleep_for(1s);

		if (auto open = file.open(test_filename, "w"); !open) {
			log_error("Fail to open file");
			return EXIT_FAILURE;
		}
		rtos::ThisThread::sleep_for(1s);

		log_info("File opened in mode 'w'");

		auto buffer = std::array<char, BUFSIZ> {};

		if (auto buffering = file.setBuffer(buffer); !buffering) {
			log_error("Fail to set buffer");
			return EXIT_FAILURE;
		}
		rtos::ThisThread::sleep_for(1s);

		log_info("Setting buffer");

		auto input_data = std::to_array("Hello, Leka!");

		if (auto bytes = file.write(input_data); bytes != std::size(input_data)) {
			log_error("Fail to edit file");
			return EXIT_FAILURE;
		}
		log_info("File edited");
		rtos::ThisThread::sleep_for(1s);

		auto size = file.size();
		if (size != std::size(input_data)) {
			log_error("Size error");
			return EXIT_FAILURE;
		}
		log_info("Size : %d", size);
		rtos::ThisThread::sleep_for(1s);

		auto output_data = std::array<char, 1024> {};

		if (auto open = file.reopen(test_filename, "r"); !open) {
			log_error("Fail to re-open file");
			return EXIT_FAILURE;
		}
		log_info("File re-opened in mode 'r'");
		rtos::ThisThread::sleep_for(1s);

		file.rewind();
		log_info("Reading...");
		if (auto bytes = file.read(output_data); bytes != size) {
			log_error("Fail to read file");
			return EXIT_FAILURE;
		}
		log_info("File read");
		log_info("Data : %s", output_data.data());
		rtos::ThisThread::sleep_for(1s);

		file.seek(seek_temp);
		log_info("Position indicator set to %d", file.tell());
		log_info("Reading...");
		if (auto bytes = file.read(output_data); bytes != (size - seek_temp)) {
			log_error("Fail to read file");
			return EXIT_FAILURE;
		}
		log_info("File read");
		log_info("Data : %s", output_data.data());
		rtos::ThisThread::sleep_for(1s);

		file.close();
		log_info("File closed");
		log_info("\n\n");
		rtos::ThisThread::sleep_for(10s);
	}

	while (true) {
		rtos::ThisThread::sleep_for(10min);
	}
}
