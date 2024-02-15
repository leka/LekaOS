// Leka - LekaOS
// Copyright 2020-2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "mbed_stats.h"

#include "drivers/Watchdog.h"
#include "rtos/Kernel.h"
#include "rtos/ThisThread.h"
#include "rtos/Thread.h"

#include "ActivityKit.h"
#include "BatteryKit.h"
#include "ChooseReinforcer.h"
#include "CoreBattery.h"
#include "CoreBufferedSerial.h"
#include "CoreDMA2D.hpp"
#include "CoreDSI.hpp"
#include "CoreFlashIS25LP016D.h"
#include "CoreFlashManagerIS25LP016D.h"
#include "CoreFont.hpp"
#include "CoreGraphics.hpp"
#include "CoreI2C.h"
#include "CoreInterruptIn.h"
#include "CoreJPEG.hpp"
#include "CoreJPEGModeDMA.hpp"
#include "CoreLCD.hpp"
#include "CoreLCDDriverOTM8009A.hpp"
#include "CoreLL.h"
#include "CoreLSM6DSOX.hpp"
#include "CoreLTDC.hpp"
#include "CoreMCU.h"
#include "CoreMotor.h"
#include "CorePwm.h"
#include "CoreQSPI.h"
#include "CoreRFIDReaderCR95HF.h"
#include "CoreSDRAM.hpp"
#include "CoreSPI.h"
#include "CoreSTM32Hal.h"
#include "CoreTimeout.h"
#include "CoreVideo.hpp"
#include "DisplayTags.h"
#include "EmotionRecognition.h"
#include "EventLoopKit.h"
#include "FATFileSystem.h"
#include "FirmwareKit.h"
#include "FlashNumberCounting.h"
#include "FoodRecognition.h"
#include "HelloWorld.h"
#include "IMUKit.hpp"
#include "LedColorRecognition.h"
#include "LedKit.h"
#include "LedNumberCounting.h"
#include "LogKit.h"
#include "NumberRecognition.h"
#include "PictoColorRecognition.h"
#include "QSPIFBlockDevice.h"
#include "RFIDKit.h"
#include "ReinforcerKit.h"
#include "RobotController.h"
#include "SDBlockDevice.h"
#include "SerialNumberKit.h"
#include "ShapeRecognition.h"
#include "SlicingBlockDevice.h"
#include "SuperSimon.h"
#include "VideoKit.h"
#include "bootutil/bootutil_public.h"
#include "commands/LedFullCommand.h"
#include "commands/LedRangeCommand.h"
#include "commands/LedSingleCommand.h"
#include "commands/MotorsCommand.h"
#include "commands/ReinforcerCommand.h"
#include "commands/VideoCommand.h"

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
	auto kit   = BatteryKit {battery::cells};

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

namespace factory_reset {

	namespace internal {

		constexpr auto factory_reset_counter_path = "/fs/usr/share/factory_reset_counter";

	}

	void resetCounter()
	{
		FileManagerKit::File file {internal::factory_reset_counter_path, "w+"};

		if (!file.is_open()) {
			return;
		}

		auto output = std::to_array<uint8_t>({0});
		file.write(output);
	}

	void set(bool is_factory_reset_requested)
	{
		FileManagerKit::File file {internal::factory_reset_counter_path, "w+"};

		if (!file.is_open()) {
			return;
		}

		auto value = std::to_array<uint8_t>({0x00});
		if (is_factory_reset_requested) {
			value = std::to_array<uint8_t>({0xFF});
		}
		file.write(value);
	}

}	// namespace factory_reset

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

			auto event_loop = EventLoopKit {};

		}	// namespace animations

	}	// namespace internal

	auto ears = CoreLED<internal::ears::size> {internal::ears::spi};
	auto belt = CoreLED<internal::belt::size> {internal::belt::spi};

	void turnOff()
	{
		ears.setColor(RGB::black);
		belt.setColor(RGB::black);
		ears.show();
		belt.show();
	}

}	// namespace leds

auto ledkit = LedKit {leds::internal::animations::event_loop, leds::ears, leds::belt};

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

namespace display::internal {

	auto event_loop = EventLoopKit {};
	auto backlight	= CorePwm {SCREEN_BACKLIGHT_PWM};

	auto corell		   = CoreLL {};
	auto pixel		   = CGPixel {corell};
	auto hal		   = CoreSTM32Hal {};
	auto coresdram	   = CoreSDRAM {hal};
	auto coredma2d	   = CoreDMA2D {hal};
	auto coredsi	   = CoreDSI {hal};
	auto coreltdc	   = CoreLTDC {hal};
	auto coregraphics  = CoreGraphics {coredma2d};
	auto corefont	   = CoreFont {pixel};
	auto coreotm	   = CoreLCDDriverOTM8009A {coredsi, backlight};
	auto corelcd	   = CoreLCD {coreotm};
	auto _corejpegmode = CoreJPEGModeDMA {hal};
	auto corejpeg	   = CoreJPEG {hal, _corejpegmode};

	extern "C" auto corevideo =
		CoreVideo {hal, coresdram, coredma2d, coredsi, coreltdc, corelcd, coregraphics, corefont, corejpeg};

}	// namespace display::internal

auto videokit = VideoKit {display::internal::event_loop, display::internal::corevideo};

namespace imu {

	namespace internal {

		auto drdy_irq = CoreInterruptIn {PinName::SENSOR_IMU_IRQ};
		auto i2c	  = CoreI2C(PinName::SENSOR_IMU_TH_I2C_SDA, PinName::SENSOR_IMU_TH_I2C_SCL);

	}	// namespace internal

	auto lsm6dsox = CoreLSM6DSOX(internal::i2c, internal::drdy_irq);

}	// namespace imu

auto imukit = IMUKit {imu::lsm6dsox};

namespace motion::internal {

	CoreTimeout timeout {};

}	// namespace motion::internal

auto motionkit = MotionKit {motors::left::motor, motors::right::motor, imukit, motion::internal::timeout};

auto behaviorkit   = BehaviorKit {videokit, ledkit, motors::left::motor, motors::right::motor};
auto reinforcerkit = ReinforcerKit {videokit, ledkit, motionkit};

namespace command {

	namespace internal {

		auto event_loop = EventLoopKit {};

		auto led_single = LedSingleCommand {leds::ears, leds::belt};
		auto led_full	= LedFullCommand {leds::ears, leds::belt};
		auto led_range	= LedRangeCommand {leds::ears, leds::belt};
		auto motors		= MotorsCommand {motors::left::motor, motors::right::motor};
		auto reinforcer = ReinforcerCommand {reinforcerkit};
		auto video		= VideoCommand {videokit};

	}	// namespace internal

	auto list = std::to_array<interface::Command *>({
		&internal::led_single,
		&internal::led_full,
		&internal::led_range,
		&internal::motors,
		&internal::reinforcer,
		&internal::video,
	});

}	// namespace command

auto commandkit = CommandKit {command::internal::event_loop};

namespace firmware {

	namespace internal {

		auto qspi	 = CoreQSPI();
		auto manager = CoreFlashManagerIS25LP016D(qspi);
		auto flash	 = CoreFlashIS25LP016D(qspi, manager);

	}	// namespace internal

	auto kit = FirmwareKit(internal::flash, FirmwareKit::DEFAULT_CONFIG);

	void initializeFlash()
	{
		internal::flash.reset();
		internal::qspi.setDataTransmissionFormat();
		internal::qspi.setFrequency(flash::is25lp016d::max_clock_frequency_in_hz);
	}

	auto version() -> Version
	{
		return kit.getCurrentVersion();
	}

	void setPendingUpdate()
	{
		boot_set_pending(0);
	}

	void confirmFirmware()
	{
		factory_reset::resetCounter();
		boot_set_confirmed();
	}

}	// namespace firmware

namespace rfid {

	auto serial = CoreBufferedSerial(RFID_UART_TX, RFID_UART_RX, 57600);
	auto reader = CoreRFIDReaderCR95HF(serial);

}	// namespace rfid

auto rfidkit = RFIDKit(rfid::reader);

namespace mcuboot {

	// namespace internal {

	auto qspi_bd   = QSPIFBlockDevice {};
	auto sliced_bd = mbed::SlicingBlockDevice {&qspi_bd, 0x0, MCUBOOT_SLOT_SIZE};

	// }	// namespace internal

}	// namespace mcuboot

namespace activities {

	namespace internal {

		auto display_tag			 = leka::activity::DisplayTags(rfidkit, videokit);
		auto choose_reinforcer		 = leka::activity::ChooseReinforcer(rfidkit, videokit, reinforcerkit);
		auto number_recognition		 = leka::activity::NumberRecognition(rfidkit, videokit, reinforcerkit);
		auto picto_color_recognition = leka::activity::PictoColorRecognition(rfidkit, videokit, reinforcerkit);
		auto led_color_recognition = leka::activity::LedColorRecognition(rfidkit, videokit, reinforcerkit, leds::belt);
		auto emotion_recognition   = leka::activity::EmotionRecognition(rfidkit, videokit, reinforcerkit);
		auto food_recognition	   = leka::activity::FoodRecognition(rfidkit, videokit, reinforcerkit);
		auto led_number_counting   = leka::activity::LedNumberCounting(rfidkit, videokit, reinforcerkit, leds::belt);
		auto flash_number_counting = leka::activity::FlashNumberCounting(rfidkit, videokit, reinforcerkit, leds::belt);
		auto super_simon		   = leka::activity::SuperSimon(rfidkit, videokit, reinforcerkit, leds::belt);
		auto shape_recognition	   = leka::activity::ShapeRecognition(rfidkit, videokit, reinforcerkit);

	}	// namespace internal

	inline const std::unordered_map<MagicCard, interface::Activity *> activities {
		{MagicCard::number_10, &internal::display_tag},
		{MagicCard::number_0, &internal::choose_reinforcer},
		{MagicCard::number_1, &internal::number_recognition},
		{MagicCard::number_2, &internal::picto_color_recognition},
		{MagicCard::number_3, &internal::led_color_recognition},
		{MagicCard::number_4, &internal::emotion_recognition},
		{MagicCard::number_5, &internal::food_recognition},
		{MagicCard::number_6, &internal::led_number_counting},
		{MagicCard::number_7, &internal::flash_number_counting},
		{MagicCard::number_8, &internal::super_simon},
		{MagicCard::number_9, &internal::shape_recognition},
	};

}	// namespace activities

auto activitykit = ActivityKit {videokit};

namespace robot {

	namespace internal {

		auto timeout_state_internal		   = CoreTimeout {};
		auto timeout_state_transition	   = CoreTimeout {};
		auto timeout_autonomous_activities = CoreTimeout {};

		auto mcu			 = CoreMCU {};
		auto serialnumberkit = SerialNumberKit {mcu, SerialNumberKit::DEFAULT_CONFIG};

	}	// namespace internal

	auto controller = RobotController {
		internal::timeout_state_internal,
		internal::timeout_state_transition,
		internal::timeout_autonomous_activities,
		battery::kit,
		internal::serialnumberkit,
		firmware::kit,
		motors::left::motor,
		motors::right::motor,
		leds::ears,
		leds::belt,
		ledkit,
		display::internal::corelcd,
		videokit,
		behaviorkit,
		commandkit,
		rfidkit,
		activitykit,
	};

}	// namespace robot

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

			static auto ble_connected	= uint8_t {};
			static auto battery_level	= uint8_t {};
			static auto charging_status = uint8_t {};

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

				ble_connected	= robot::controller.isBleConnected() ? 1 : 0;
				battery_level	= battery::cells.level();
				charging_status = battery::cells.isCharging() ? 1 : 0;

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
					"dt: %i, kck: %u, ble: %u, lvl: %u%%, chr: %u, idl: %u%%, slp: %u%%, dsl: %u%%, sur: %u%% (%+i)[%u/"
					"%u], hur: %u%% (%+i)[%u/%u]",
					delta, kick_count, ble_connected, battery_level, charging_status, idle_ratio, sleep_ratio,
					deep_sleep_ratio, stack_used_ratio, stack_used_delta, stack_used_size, stack_reserved_size,
					heap_used_ratio, heap_used_delta, heap_used_size, heap_reserved_size);

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

namespace deep_sleep {
	auto components = std::to_array<interface::DeepSleepEnabled *>({
		&motors::left::motor,
		&motors::right::motor,
		&display::internal::corelcd,
		&rfid::reader,
		&imu::lsm6dsox,
	});
}

}	// namespace

auto get_secondary_bd() -> mbed::BlockDevice *
{
	return &mcuboot::sliced_bd;
}

auto main() -> int
{
	leka::logger::init();
	watchdog::start();

	leds::turnOff();
	motors::turnOff();

	rtos::ThisThread::sleep_for(1s);

	auto version = firmware::version();

	log_info("\n\n");
	log_info("Hello, LekaOS v%i.%i.%i!", version.major, version.minor, version.revision);
	log_info("Arm GNU Toolchain v%s\n\n", __VERSION__);

	rtos::ThisThread::sleep_for(2s);

	auto hello = HelloWorld();
	hello.start();

	sd::init();
	firmware::initializeFlash();

	commandkit.registerCommand(command::list);
	activitykit.registerActivities(activities::activities);

	imu::lsm6dsox.init();
	imukit.init();

	robot::controller.initializeComponents();
	robot::controller.registerOnUpdateLoadedCallback(firmware::setPendingUpdate);
	robot::controller.registerOnFactoryResetNotificationCallback(factory_reset::set);
	robot::controller.registerEvents();
	robot::controller.registerDeepSleepEnabledComponents(deep_sleep::components);

	// TODO(@team): Add functional test prior confirming the firmware
	firmware::confirmFirmware();

	while (true) {
		rtos::ThisThread::sleep_for(1s);
	}
}
