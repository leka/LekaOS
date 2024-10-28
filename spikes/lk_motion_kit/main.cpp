// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "rtos/ThisThread.h"

#include "CoreBufferedSerial.h"
#include "CoreI2C.h"
#include "CoreIMU.hpp"
#include "CoreMotor.h"
#include "CorePwm.h"
#include "CoreRFIDReaderCR95HF.h"
#include "CoreTimeout.h"
#include "EventLoopKit.h"
#include "HelloWorld.h"
#include "IMUKit.hpp"
#include "LogKit.h"
#include "MotionKit.hpp"
#include "RFIDKit.h"

using namespace std::chrono;
using namespace leka;

namespace {

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

		auto irq = CoreInterruptIn {PinName::SENSOR_IMU_IRQ};
		CoreI2C i2c(PinName::SENSOR_IMU_TH_I2C_SDA, PinName::SENSOR_IMU_TH_I2C_SCL);
		auto event_queue = CoreEventQueue();

	}	// namespace internal

	CoreIMU coreimu(internal::i2c, internal::irq);

}	// namespace imu

auto imukit = IMUKit {imu::coreimu};

namespace motion::internal {

	CoreTimeout timeout {};

}	// namespace motion::internal

auto motionkit = MotionKit {motors::left::motor, motors::right::motor, imukit, motion::internal::timeout};

namespace rfid {

	auto serial = CoreBufferedSerial(RFID_UART_TX, RFID_UART_RX, 57600);
	auto reader = CoreRFIDReaderCR95HF(serial);

}	// namespace rfid

auto rfidkit = RFIDKit(rfid::reader);

}	// namespace

void onMagicCardAvailable(const MagicCard &card)
{
	switch (card.getId()) {
		case (MagicCard::number_1.getId()):
			motionkit.startYawRotation(90, Rotation::counterClockwise, [] { log_debug("Callback end of rotation"); });
			break;
		case (MagicCard::number_2.getId()):
			motionkit.startYawRotation(180, Rotation::clockwise);
			break;
		case (MagicCard::number_3.getId()):
			motionkit.startYawRotation(360, Rotation::counterClockwise);
			break;
		case (MagicCard::number_4.getId()):
			motionkit.startYawRotation(540, Rotation::clockwise);
			break;
		case (MagicCard::number_5.getId()):
			motionkit.startYawRotation(720, Rotation::counterClockwise);
			break;
		case (MagicCard::number_6.getId()):
			motionkit.startYawRotation(1080, Rotation::clockwise);
			break;
		case (MagicCard::number_7.getId()):
			motionkit.startYawRotation(1080, Rotation::counterClockwise);
			break;
		case (MagicCard::number_10.getId()):
			motionkit.stop();
			break;
		default:
			motionkit.stop();
			log_debug("Stop");
			break;
	}
}

auto main() -> int
{
	logger::init();

	HelloWorld hello;
	hello.start();

	imu::coreimu.init();
	imukit.init();
	rfidkit.init();

	rfidkit.onTagActivated(onMagicCardAvailable);

	while (true) {
		rtos::ThisThread::sleep_for(100ms);
	}
}
