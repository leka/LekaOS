// Leka - LekaOS
// Copyright 2024 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <cmath>

#include "rtos/ThisThread.h"

#include "CoreDAC.h"
#include "CoreEventFlags.h"
#include "CoreEventQueue.h"
#include "CoreSTM32Hal.h"
#include "CoreSTM32HalBasicTimer.h"
#include "DigitalOut.h"
#include "FATFileSystem.h"
#include "FileManagerKit.h"
#include "LogKit.h"
#include "SDBlockDevice.h"

using namespace leka;
using namespace std::chrono_literals;

auto hal	   = CoreSTM32Hal {};
auto hal_timer = CoreSTM32HalBasicTimer {hal};
auto coredac   = CoreDAC {hal, hal_timer};

auto audio_enable = mbed::DigitalOut {SOUND_ENABLE, 1};

constexpr uint32_t sample_rate_hz = 44'100;

auto event_queue		   = CoreEventQueue {};
auto event_queue_converted = CoreEventQueue {};

auto sd_blockdevice = SDBlockDevice {SD_SPI_MOSI, SD_SPI_MISO, SD_SPI_SCK};
auto fatfs			= FATFileSystem {"fs"};

const auto sound_file_path = std::filesystem::path {"/fs/home/wav/440.wav"};
auto file				   = FileManagerKit::File {};

auto event_flags = CoreEventFlags {};

struct flag {
	static constexpr uint32_t START = (1UL << 1);
};

void initializeSD()
{
	constexpr auto default_sd_blockdevice_frequency = uint64_t {25'000'000};

	sd_blockdevice.init();
	sd_blockdevice.frequency(default_sd_blockdevice_frequency);

	fatfs.mount(&sd_blockdevice);
}

// constexpr auto size = 128;
// constexpr auto size = 256;
// constexpr auto size = 512;
constexpr auto size = 5'000;
// constexpr auto size = 1'024;
// constexpr auto size = 2'048;
// constexpr auto size = 4'096;
// constexpr auto size = 8'192;
// constexpr auto size = 16'384;
// constexpr auto size = 32'768;
// constexpr auto size = 65'536; // NOK
constexpr auto coefficient	  = 10;						  // Related to ARR | ARR*coefficient ~= 2448
constexpr auto data_file_size = size / coefficient / 2;	  // /2 for half buffer and *2
std::array<int16_t, data_file_size> data_file {};
// std::array<uint8_t, size> data_file {};
std::array<uint16_t, size> data_converted {};
std::array<uint16_t, size> data_play {};

auto is_eof = false;

void convertData(uint32_t offset)
{
	auto bytes_read = file.read(data_file);

	for (auto i = 0; i < data_file_size; i++) {
		auto normalized_value = static_cast<uint16_t>((data_file.at(i) + 0x8000) >> 4);
		std::fill_n(data_converted.begin() + offset + i * coefficient, coefficient, normalized_value);
	}
	is_eof = bytes_read != data_file_size;

	log_info("bytes_read %d", bytes_read);	 // Better than sleep_for
}

void setData(uint32_t offset)
{
	if (is_eof) {
		coredac.stop();
		return;
	}

	std::copy(data_converted.begin() + offset, data_converted.begin() + offset + size / 2, data_play.begin() + offset);

	event_queue_converted.call([offset] { convertData(offset); });

	// log_info("");	// Better than sleep_for
	rtos::ThisThread::sleep_for(1ms);
}

void onHalfTransfer()
{
	// Fill first half
	setData(0);
	// std::fill_n(data_play.begin(), size / 2, 0x0);
}
void onCompleteTransfer()
{
	// Fill second half
	setData(size / 2);
	// std::fill_n(data_play.begin() + size / 2, size / 2, 0xFFF);
}

auto main() -> int
{
	logger::init();

	log_info("Hello, World!\n\n");
	rtos::ThisThread::sleep_for(1s);

	event_queue_converted.dispatch_forever();
	event_queue.dispatch_forever();

	initializeSD();
	if (FileManagerKit::file_is_missing(sound_file_path)) {
		return 1;
	}

	// BEGIN -- NEW CODE

	log_info("Initialize");
	rtos::ThisThread::sleep_for(1s);

	coredac.registerDMACallbacks([] { event_queue.call(onHalfTransfer); },
								 [] { event_queue.call(onCompleteTransfer); });

	hal_timer.initialize(44'100 * coefficient);
	coredac.initialize();

	event_flags.set(flag::START);

	while (true) {
		event_flags.wait_any(flag::START);
		event_flags.set(flag::START);	// Temporary

		{
			file.open(sound_file_path);
			auto header_array = std::array<uint8_t, 44> {};
			file.read(header_array);   // header

			is_eof = false;

			convertData(0);
			rtos::ThisThread::sleep_for(300ms);
			convertData(size / 2);
			rtos::ThisThread::sleep_for(300ms);

			setData(0);
			rtos::ThisThread::sleep_for(300ms);
			setData(size / 2);
			rtos::ThisThread::sleep_for(300ms);

			coredac.registerDataToPlay(data_play);
			rtos::ThisThread::sleep_for(100ms);
		}	// Setup new file

		{
			coredac.start();
			while (!is_eof) rtos::ThisThread::sleep_for(1s);
		}	// Play on audio

		rtos::ThisThread::sleep_for(1s);
	}
}
