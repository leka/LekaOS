// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "mbed.h"

#include "LogKit.h"

DigitalOut audio_enable(SOUND_ENABLE, 1);
#include "AudioUtils.h"
Thread thread_audio;
AnalogOut audio_output(MCU_SOUND_OUT);

#include "FATFileSystem.h"
#include "SDBlockDevice.h"
auto sd_bd = SDBlockDevice {SD_SPI_MOSI, SD_SPI_MISO, SD_SPI_SCK};
auto fatfs = FATFileSystem {"fs"};

using namespace leka;

AnalogIn batteries_level(BATTERY_VOLTAGE);

constexpr auto serial_buffer_size = uint8_t {128};
auto serial_buffer				  = std::array<char, serial_buffer_size> {};

void initializeSD()
{
	constexpr auto default_sd_bd_frequency = uint64_t {25'000'000};

	sd_bd.init();
	sd_bd.frequency(default_sd_bd_frequency);

	fatfs.mount(&sd_bd);
}

auto main() -> int
{
	logger::init();

	log_info("Hello, Investigation Day!\n");

	auto start	  = Kernel::Clock::now();
	auto duration = Kernel::Clock::now() - start;

	initializeSD();

	audio_pause_duration = 20s;
	thread_audio.start(callback(playSoundPeriodically, &audio_output));

	rtos::ThisThread::sleep_for(10s);
	while (true) {
		duration   = Kernel::Clock::now() - start;
		int length = snprintf(serial_buffer.data(), serial_buffer_size, "Leka is still alive after: %2i:%2i:%2i\n",
							  int(std::chrono::duration_cast<std::chrono::hours>(duration).count()),
							  int(std::chrono::duration_cast<std::chrono::minutes>(duration).count()) % 60,
							  int(std::chrono::duration_cast<std::chrono::seconds>(duration).count()) % 60);

		rtos::ThisThread::sleep_for(1s);
	}
}
