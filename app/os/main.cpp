// Leka - LekaOS
// Copyright 2020-2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "drivers/Watchdog.h"
#include "rtos/Kernel.h"
#include "rtos/ThisThread.h"
#include "rtos/Thread.h"

#include "CoreBattery.h"
#include "CoreEventFlags.h"
#include "CoreFlashIS25LP016D.h"
#include "CoreFlashManagerIS25LP016D.h"
#include "CoreMCU.h"
#include "CorePwm.h"
#include "CoreQSPI.h"
#include "CoreSPI.h"
#include "CoreTimeout.h"
#include "FATFileSystem.h"
#include "FirmwareKit.h"
#include "HelloWorld.h"
#include "LogKit.h"
#include "QSPIFBlockDevice.h"
#include "RobotController.h"
#include "SDBlockDevice.h"
#include "SerialNumberKit.h"
#include "SlicingBlockDevice.h"
#include "VideoKit.h"
#include "bootutil/bootutil.h"

using namespace leka;
using namespace std::chrono;

namespace watchdog {

auto &instance		   = mbed::Watchdog::get_instance();
constexpr auto timeout = 30000ms;
auto thread			   = rtos::Thread {osPriorityLow};

__attribute__((noreturn)) void kick()
{
	while (true) {
		watchdog::instance.kick();
		rtos::ThisThread::sleep_for(5s);
	}
}

void start()
{
	watchdog::instance.start(watchdog::timeout.count());
	watchdog::thread.start(watchdog::kick);
}

}	// namespace watchdog

auto sleep_timeout = CoreTimeout {};

auto mcu			 = CoreMCU {};
auto serialnumberkit = SerialNumberKit {mcu};

namespace leds {

namespace spi {

	auto belt = CoreSPI {LED_BELT_SPI_MOSI, NC, LED_BELT_SPI_SCK};
	auto ears = CoreSPI {LED_EARS_SPI_MOSI, NC, LED_EARS_SPI_SCK};

}	// namespace spi

namespace animations {

	auto thread		 = rtos::Thread {};
	auto event_flags = CoreEventFlags {};

}	// namespace animations

auto ears = CoreLED<LedKit::kNumberOfLedsEars> {spi::ears};
auto belt = CoreLED<LedKit::kNumberOfLedsBelt> {spi::belt};

}	// namespace leds

auto ledkit = LedKit {leds::animations::thread, leds::animations::event_flags, leds::ears, leds::belt};

namespace motor {

namespace internal {

	namespace left {

		auto dir_1 = mbed::DigitalOut {MOTOR_LEFT_DIRECTION_1};
		auto dir_2 = mbed::DigitalOut {MOTOR_LEFT_DIRECTION_2};
		auto speed = CorePwm {MOTOR_LEFT_PWM};

	}	// namespace left
	namespace right {

		auto dir_1 = mbed::DigitalOut {MOTOR_RIGHT_DIRECTION_1};
		auto dir_2 = mbed::DigitalOut {MOTOR_RIGHT_DIRECTION_2};
		auto speed = CorePwm {MOTOR_RIGHT_PWM};

	}	// namespace right
}	// namespace internal

auto left  = CoreMotor {internal::left::dir_1, internal::left::dir_2, internal::left::speed};
auto right = CoreMotor {internal::right::dir_1, internal::right::dir_2, internal::right::speed};

}	// namespace motor

auto sd_blockdevice = SDBlockDevice {SD_SPI_MOSI, SD_SPI_MISO, SD_SPI_SCK};
auto fatfs			= FATFileSystem {"fs"};

auto videokit = VideoKit {};

auto behaviorkit = BehaviorKit {videokit, ledkit, motor::left, motor::right};

auto charge_input = mbed::InterruptIn {PinName::BATTERY_CHARGE_STATUS};
auto battery	  = leka::CoreBattery {PinName::BATTERY_VOLTAGE, charge_input};

auto coreqspi		  = CoreQSPI();
auto coreflashmanager = CoreFlashManagerIS25LP016D(coreqspi);
auto coreflash		  = CoreFlashIS25LP016D(coreqspi, coreflashmanager);
auto firmwarekit	  = FirmwareKit(coreflash);

namespace command {

namespace internal {

	auto test		= TestCommand {};
	auto led		= LedSingleCommand {leds::ears, leds::belt};
	auto led_full	= LedFullCommand {leds::ears, leds::belt};
	auto led_range	= LedRangeCommand {leds::ears, leds::belt};
	auto motors		= MotorsCommand {motor::left, motor::right};
	auto reinforcer = ReinforcerCommand {behaviorkit};

}	// namespace internal

auto list = std::to_array<interface::Command *>({
	&internal::test,
	&internal::led,
	&internal::led_full,
	&internal::led_range,
	&internal::motors,
	&internal::reinforcer,
});

}	// namespace command

auto commandkit = CommandKit {};

auto rc = RobotController {sleep_timeout, battery, serialnumberkit, firmwarekit, motor::left,
						   motor::right,  ledkit,  videokit,		behaviorkit, commandkit};

void initializeSD()
{
	constexpr auto default_sd_blockdevice_frequency = uint64_t {25'000'000};

	sd_blockdevice.init();
	sd_blockdevice.frequency(default_sd_blockdevice_frequency);

	fatfs.mount(&sd_blockdevice);
}

void initializeUpdateFlash()
{
	coreflash.reset();
	coreqspi.setDataTransmissionFormat();
	coreqspi.setFrequency(flash::is25lp016d::max_clock_frequency_in_hz);
}

auto get_secondary_bd() -> mbed::BlockDevice *
{
	static auto _bd = QSPIFBlockDevice {};

	static auto sliced_bd = mbed::SlicingBlockDevice {&_bd, 0x0, MCUBOOT_SLOT_SIZE};

	return &sliced_bd;
}

void setPendingUpdate()
{
	boot_set_pending(1);
}

auto main() -> int
{
	watchdog::start();

	logger::init();

	rtos::ThisThread::sleep_for(1s);

	log_info("\n\n");
	log_info("Hello, LekaOS!\n");

	rtos::ThisThread::sleep_for(2s);

	auto hello = HelloWorld();
	hello.start();

	initializeSD();
	initializeUpdateFlash();

	commandkit.registerCommand(command::list);

	rc.initializeComponents();
	rc.registerOnUpdateLoadedCallback(setPendingUpdate);
	rc.registerEvents();

	while (true) {
		log_debug("A message from your board %s --> \"%s\" at %ims", MBED_CONF_APP_TARGET_NAME, hello.world,
				  int(rtos::Kernel::Clock::now().time_since_epoch().count()));
		rtos::ThisThread::sleep_for(1s);
	}
}
