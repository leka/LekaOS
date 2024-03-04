// Leka - LekaOS
// Copyright 2024 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <cmath>

#include "rtos/ThisThread.h"

#include "CoreDAC.h"
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

auto sd_blockdevice = SDBlockDevice {SD_SPI_MOSI, SD_SPI_MISO, SD_SPI_SCK};
auto fatfs			= FATFileSystem {"fs"};

auto file = FileManagerKit::File {};

const auto sound_file_path = std::filesystem::path {"/fs/home/wav/fur-elise.wav"};

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
// constexpr auto size = 16'384;
constexpr auto size = 32'768;
// constexpr auto size = 65'536; // NOK
std::array<uint8_t, size> data_file {};
std::array<uint16_t, size> data_play {};

auto main() -> int
{
	logger::init();

	log_info("Hello, World!\n\n");
	rtos::ThisThread::sleep_for(1s);

	initializeSD();
	if (FileManagerKit::file_is_missing(sound_file_path)) {
		return 1;
	}

	// BEGIN -- NEW CODE

	log_info("Initialize");
	rtos::ThisThread::sleep_for(1s);

	auto file = FileManagerKit::File {sound_file_path};

	file.read(data_file);
	for (int index = 0; index < data_file.size(); index += 2) {
		data_play[index / 2] = (data_file[index + 1] << 8) | data_file[index];
		// data_play[index / 2] = (data_file[index] << 8) | data_file[index + 1];
	}

	// for (auto one_data: data_play) {
	// 	printf("%04x", one_data);
	// }
	// printf("\n");

	coredac.registerDMACallbacks(
		[&file] {
			// Fill first half
			std::array<uint8_t, size> data_first_half {};
			file.read(data_first_half);	  // THIS FAIL

			// for (int index = 0; index < data_file.size(); index += 2) {
			// data_play[index / 2] = (data_file[index + 1] << 8) | data_file[index];
			// }
		},
		[&file] {
			// Fill second half

			// file.read(data_file);
			// for (int index = 0; index < data_file.size(); index += 2) {
			// data_play[size / 2 + index / 2] = (data_file[index + 1] << 8) | data_file[index];
			// }
		});

	hal_timer.initialize(sample_rate_hz);
	coredac.initialize();

	coredac.registerDataToPlay(data_play);
	// Play on audio

	log_info("Start sound");
	rtos::ThisThread::sleep_for(1s);

	// END -- NEW CODE
	coredac.start();

	rtos::ThisThread::sleep_for(1s);

	log_info("Stop sound");
	coredac.stop();

	while (true) {
		rtos::ThisThread::sleep_for(1min);
	}
}
