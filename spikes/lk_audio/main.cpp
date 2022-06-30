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

	log_info("Hello, World!\n\n");

	initializeSD();

	while (true) {
		if (auto is_open = file.open(sound_file_path); is_open) {
			playSound();
			file.close();
		}

		rtos::ThisThread::sleep_for(1s);
	}
}
