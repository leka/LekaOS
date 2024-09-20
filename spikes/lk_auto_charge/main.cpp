// Leka - LekaOS
// Copyright 2023 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "drivers/DigitalOut.h"
#include "drivers/InterruptIn.h"
#include "rtos/ThisThread.h"

#include "BLEKit.h"
#include "BLEServiceCommands.h"

#include "BatteryKit.h"
#include "BehaviorKit.h"
#include "CoreBattery.h"
#include "CoreI2C.h"
#include "CoreLSM6DSOX.hpp"
#include "CoreMotor.h"
#include "CorePwm.h"
#include "CoreTimeout.h"
#include "EventLoopKit.h"
#include "HappyFishy.h"
#include "HappyToupie.h"
#include "HelloWorld.h"
#include "IMUKit.hpp"
#include "LogKit.h"
#include "MathUtils.h"
#include "SealStrategy.h"
#include "behaviors/AutochargeSeal.h"

using namespace std::chrono;
using namespace leka;

namespace {

auto event_loop = EventLoopKit {};
auto timeout	= CoreTimeout {};

namespace battery {

	namespace charge {

		auto status_input = mbed::InterruptIn {PinName::BATTERY_CHARGE_STATUS};

	}

	auto cells = CoreBattery {PinName::BATTERY_VOLTAGE, battery::charge::status_input};

}	// namespace battery

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

}	// namespace motors

namespace imu {

	namespace internal {

		auto drdy_irq = CoreInterruptIn {PinName::SENSOR_IMU_IRQ};
		auto i2c	  = CoreI2C(PinName::SENSOR_IMU_TH_I2C_SDA, PinName::SENSOR_IMU_TH_I2C_SCL);

	}	// namespace internal

	CoreLSM6DSOX lsm6dsox(internal::i2c, internal::drdy_irq);

}	// namespace imu

IMUKit imukit(imu::lsm6dsox);

}	// namespace

auto mainboard_led = mbed::DigitalOut {LED1};

auto service_commands = BLEServiceCommands {};
auto services		  = std::to_array<interface::BLEService *>({&service_commands});
auto blekit			  = BLEKit {};

auto behavior_autocharge_seal = behavior::AutochargeSeal {motors::left::motor, motors::right::motor, imukit};
auto behaviors				  = std::to_array<interface::Behavior *>({&behavior_autocharge_seal});
auto behavior_kit			  = BehaviorKit {event_loop};

auto last_strategy = BehaviorID {0x00};

auto main() -> int
{
	logger::init();

	HelloWorld hello;
	hello.start();

	log_info("Hello world!");

	imu::lsm6dsox.init();

	imukit.stop();
	imukit.init();
	imukit.start();

	behavior_kit.registerBehaviors(behaviors);

	battery::cells.onChargeDidStart([] { behavior_kit.stop(); });
	battery::cells.onChargeDidStop([] { behavior_kit.start(last_strategy); });

	blekit.setServices(services);
	service_commands.onCommandsReceived([](std::span<uint8_t> _buffer) {
		behavior_kit.start(_buffer[0]);
		last_strategy = _buffer[0];
	});
	blekit.init();

	while (true) {
		rtos::ThisThread::sleep_for(10min);
	}
}
