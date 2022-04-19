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

//
// MARK: - Global definitions
//

namespace {

namespace battery {

	namespace charge {

		auto status_input = mbed::InterruptIn {PinName::BATTERY_CHARGE_STATUS};

	}

	auto cells = CoreBattery {PinName::BATTERY_VOLTAGE, battery::charge::status_input};

}	// namespace battery

namespace sd {

	namespace internal {

		auto bd = SDBlockDevice {SD_SPI_MOSI, SD_SPI_MISO, SD_SPI_SCK};
		auto fs = FATFileSystem {"fs"};

		constexpr auto default_frequency = uint64_t {25'000'000};

	}	// namespace internal

	void init()
	{
		internal::bd.init();
		internal::bd.frequency(internal::default_frequency);
		internal::fs.mount(&internal::bd);
	}

}	// namespace sd

namespace leds {

	namespace internal {

		namespace ears {

			auto spi			= CoreSPI {LED_EARS_SPI_MOSI, NC, LED_EARS_SPI_SCK};
			constexpr auto size = 2;

		}	// namespace ears

		namespace belt {

			auto spi			= CoreSPI {LED_BELT_SPI_MOSI, NC, LED_BELT_SPI_SCK};
			constexpr auto size = 20;

		}	// namespace belt

		namespace animations {

			auto thread		 = rtos::Thread {};
			auto event_flags = CoreEventFlags {};

		}	// namespace animations

	}	// namespace internal

	auto ears = CoreLED<internal::ears::size> {internal::ears::spi};
	auto belt = CoreLED<internal::belt::size> {internal::belt::spi};

	auto kit = LedKit {internal::animations::thread, internal::animations::event_flags, ears, belt};

	void turnOff()
	{
		ears.setColor(RGB::black);
		belt.setColor(RGB::black);
		ears.show();
		belt.show();
	}

}	// namespace leds

namespace motors {

	namespace left {

		namespace internal {

			auto dir_1 = mbed::DigitalOut {MOTOR_LEFT_DIRECTION_1};
			auto dir_2 = mbed::DigitalOut {MOTOR_LEFT_DIRECTION_2};
			auto speed = CorePwm {MOTOR_LEFT_PWM};

		}	// namespace internal

		auto motor = CoreMotor {internal::dir_1, internal::dir_2, internal::speed};

	}	// namespace left

	namespace right {

		namespace internal {

			auto dir_1 = mbed::DigitalOut {MOTOR_RIGHT_DIRECTION_1};
			auto dir_2 = mbed::DigitalOut {MOTOR_RIGHT_DIRECTION_2};
			auto speed = CorePwm {MOTOR_RIGHT_PWM};

		}	// namespace internal

		auto motor = CoreMotor {internal::dir_1, internal::dir_2, internal::speed};

	}	// namespace right

	void turnOff()
	{
		left::motor.stop();
		right::motor.stop();
	}

}	// namespace motors

auto videokit	 = VideoKit {};
auto behaviorkit = BehaviorKit {videokit, leds::kit, motors::left::motor, motors::right::motor};

namespace command {

	namespace internal {

		auto led_single = LedSingleCommand {leds::ears, leds::belt};
		auto led_full	= LedFullCommand {leds::ears, leds::belt};
		auto led_range	= LedRangeCommand {leds::ears, leds::belt};
		auto motors		= MotorsCommand {motors::left::motor, motors::right::motor};
		auto reinforcer = ReinforcerCommand {behaviorkit};

	}	// namespace internal

	auto list = std::to_array<interface::Command *>({
		&internal::led_single,
		&internal::led_full,
		&internal::led_range,
		&internal::motors,
		&internal::reinforcer,
	});

}	// namespace command

auto commandkit = CommandKit {};

namespace firmware {

	namespace internal {

		auto qspi	 = CoreQSPI();
		auto manager = CoreFlashManagerIS25LP016D(qspi);
		auto flash	 = CoreFlashIS25LP016D(qspi, manager);

	}	// namespace internal

	auto kit = FirmwareKit(internal::flash);

	void initializeFlash()
	{
		internal::flash.reset();
		internal::qspi.setDataTransmissionFormat();
		internal::qspi.setFrequency(flash::is25lp016d::max_clock_frequency_in_hz);
	}

	void setPendingUpdate()
	{
		boot_set_pending(1);
	}

}	// namespace firmware

namespace mcuboot {

	// namespace internal {

	auto qspi_bd   = QSPIFBlockDevice {};
	auto sliced_bd = mbed::SlicingBlockDevice {&qspi_bd, 0x0, MCUBOOT_SLOT_SIZE};

	// }	// namespace internal

}	// namespace mcuboot

namespace watchdog {

	namespace internal {

		auto &instance		   = mbed::Watchdog::get_instance();
		constexpr auto timeout = 30000ms;
		auto thread			   = rtos::Thread {osPriorityLow};

	}	// namespace internal

	__attribute__((noreturn)) void kick()
	{
		while (true) {
			internal::instance.kick();
			rtos::ThisThread::sleep_for(5s);
		}
	}

	void start()
	{
		internal::instance.start(internal::timeout.count());
		internal::thread.start(watchdog::kick);
	}

}	// namespace watchdog

namespace robot {

	namespace internal {

		auto sleep_timeout = CoreTimeout {};

		auto mcu			 = CoreMCU {};
		auto serialnumberkit = SerialNumberKit {mcu};

	}	// namespace internal

	auto controller = RobotController {internal::sleep_timeout,
									   battery::cells,
									   internal::serialnumberkit,
									   firmware::kit,
									   motors::left::motor,
									   motors::right::motor,
									   leds::kit,
									   videokit,
									   behaviorkit,
									   commandkit};

}	// namespace robot

}	// namespace

auto get_secondary_bd() -> mbed::BlockDevice *
{
	return &mcuboot::sliced_bd;
}

auto main() -> int
{
	logger::init();
	watchdog::start();

	leds::turnOff();
	motors::turnOff();

	rtos::ThisThread::sleep_for(1s);

	log_info("\n\n");
	log_info("Hello, LekaOS!\n");

	rtos::ThisThread::sleep_for(2s);

	auto hello = HelloWorld();
	hello.start();

	sd::init();
	firmware::initializeFlash();

	commandkit.registerCommand(command::list);

	robot::controller.initializeComponents();
	robot::controller.registerOnUpdateLoadedCallback(firmware::setPendingUpdate);
	robot::controller.registerEvents();

	while (true) {
		log_debug("A message from your board %s --> \"%s\" at %ims", MBED_CONF_APP_TARGET_NAME, hello.world,
				  int(rtos::Kernel::Clock::now().time_since_epoch().count()));
		rtos::ThisThread::sleep_for(1s);
	}
}
