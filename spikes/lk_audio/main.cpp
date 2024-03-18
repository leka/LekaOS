// Leka - LekaOS
// Copyright 2024 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "rtos/ThisThread.h"

#include "BLEKit.h"
#include "BLEServiceConfig.h"

#include "AudioKit.h"
#include "CoreDAC.h"
#include "CoreSTM32Hal.h"
#include "CoreSTM32HalBasicTimer.h"
#include "FATFileSystem.h"
#include "LogKit.h"
#include "SDBlockDevice.h"

using namespace leka;
using namespace std::chrono_literals;

auto sd_bd	  = SDBlockDevice {SD_SPI_MOSI, SD_SPI_MISO, SD_SPI_SCK};
auto fatfs	  = FATFileSystem {"fs"};
auto filename = std::filesystem::path {};

auto hal = CoreSTM32Hal {};

namespace audio {

namespace internal {

	extern "C" auto hal_timer = CoreSTM32HalBasicTimer {hal};
	extern "C" auto coredac	  = CoreDAC {hal, hal_timer};

}	// namespace internal

auto kit = AudioKit {internal::hal_timer, internal::coredac};

}	// namespace audio

auto service_config = BLEServiceConfig {};
auto services		= std::to_array<interface::BLEService *>({&service_config});
auto blekit			= BLEKit {};

void initializeSD()
{
	constexpr auto default_sd_bd_frequency = uint64_t {25'000'000};

	sd_bd.init();
	sd_bd.frequency(default_sd_bd_frequency);

	fatfs.mount(&sd_bd);
}

void play(std::string filename)
{
	log_info("Play file: %s", filename.c_str());
	audio::kit.play(filename);
}

auto main() -> int
{
	logger::init();

	log_info("Hello, World!\n\n");
	rtos::ThisThread::sleep_for(1s);

	initializeSD();

	audio::kit.initialize();

	blekit.setServices(services);
	blekit.init();

	service_config.onRobotNameUpdated([](const std::array<uint8_t, BLEServiceConfig::kMaxRobotNameSize> &robot_name) {
		const auto *end_index = std::find(robot_name.begin(), robot_name.end(), '\0');
		filename			  = std::string {robot_name.begin(), end_index};
		play(filename);
	});

	while (true) {
		rtos::ThisThread::sleep_for(1min);
	}
}
