// Leka - LekaOS
// Copyright 2024 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <cmath>

#include "rtos/ThisThread.h"

#include "CoreDAC.h"
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

auto event_queue = CoreEventQueue {};

auto sd_blockdevice = SDBlockDevice {SD_SPI_MOSI, SD_SPI_MISO, SD_SPI_SCK};
auto fatfs			= FATFileSystem {"fs"};

const auto sound_file_path = std::filesystem::path {"/fs/home/wav/fur-elise.wav"};
auto file				   = FileManagerKit::File {};

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
// constexpr auto size = 1'024;
// constexpr auto size = 2'048;
// constexpr auto size = 4'096;
// constexpr auto size = 8'192;
constexpr auto size = 16'384;
// constexpr auto size = 32'768;
// constexpr auto size = 65'536; // NOK
std::array<uint8_t, size> data_file {};
std::array<uint16_t, size * 2> data_play {};

void setData(uint16_t offset)
{
	file.read(data_file);
	// for (uint32_t index = 0; index < data_file.size(); index += 2) {
	// 	// data_play.at(offset + index / 2) = (data_file[index + 1] << 8) | data_file[index];
	// 	data_play[offset + index / 2] = (data_file[index] << 8) | data_file[index + 1];

	// 	// data_play.at(offset + index / 2) = (data_file.at(index + 1) + 0x8000) >> 0;
	// }

	for (uint32_t index = 0; index < data_file.size(); index++) {
		// data_play.at(offset + index / 2) = (data_file[index + 1] << 8) | data_file[index];
		// data_play[offset + index / 2] = (data_file[index] << 8) | data_file[index + 1];

		data_play.at(offset + index) = (data_file.at(index) + 0x8000) >> 0;
	}
}

void onHalfTransfer()
{
	// Fill first half
	setData(0);
}
void onCompleteTransfer()
{
	// Fill second half
	setData(size);
}

auto main() -> int
{
	logger::init();

	log_info("Hello, World!\n\n");
	rtos::ThisThread::sleep_for(1s);

	event_queue.dispatch_forever();

	initializeSD();
	if (FileManagerKit::file_is_missing(sound_file_path)) {
		return 1;
	}

	// BEGIN -- NEW CODE

	log_info("Initialize");
	rtos::ThisThread::sleep_for(1s);

	file.open(sound_file_path);

	setData(0);
	setData(size / 2);

	coredac.registerDMACallbacks([] { event_queue.call(onHalfTransfer); },
								 [] { event_queue.call(onCompleteTransfer); });

	hal_timer.initialize(sample_rate_hz);
	coredac.initialize();

	coredac.registerDataToPlay(data_play);
	// Play on audio

	log_info("Start sound");
	rtos::ThisThread::sleep_for(1s);

	// END -- NEW CODE
	coredac.start();

	rtos::ThisThread::sleep_for(10s);

	log_info("Stop sound");
	coredac.stop();

	while (true) {
		rtos::ThisThread::sleep_for(1min);
	}
}
