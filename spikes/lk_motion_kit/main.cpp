// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "rtos/ThisThread.h"

#include "CoreAccelerometer.h"
#include "CoreBufferedSerial.h"
#include "CoreGyroscope.h"
#include "CoreI2C.h"
#include "CoreLSM6DSOX.h"
#include "CoreMotor.h"
#include "CorePwm.h"
#include "CoreRFIDReaderCR95HF.h"
#include "CoreTimeout.h"
#include "EventLoopKit.h"
#include "HelloWorld.h"
#include "IMUKit.h"
#include "LogKit.h"
#include "MotionKit.h"
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

		CoreI2C i2c(PinName::SENSOR_IMU_TH_I2C_SDA, PinName::SENSOR_IMU_TH_I2C_SCL);
		EventLoopKit event_loop {};

	}	// namespace internal

	CoreLSM6DSOX lsm6dsox(internal::i2c);
	CoreAccelerometer accel(lsm6dsox);
	CoreGyroscope gyro(lsm6dsox);

}	// namespace imu

auto imukit = IMUKit {imu::internal::event_loop, imu::accel, imu::gyro};

namespace motion::internal {

	EventLoopKit event_loop {};
	CoreTimeout timeout {};

}	// namespace motion::internal

auto motionkit = MotionKit {motors::left::motor, motors::right::motor, imukit, motion::internal::event_loop,
							motion::internal::timeout};

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
			motionkit.rotate(1, Rotation::counterClockwise, [] { log_debug("Callback end of rotation"); });
			break;
		case (MagicCard::number_2.getId()):
			motionkit.rotate(2, Rotation::clockwise);
			break;
		case (MagicCard::number_3.getId()):
			motionkit.rotate(3, Rotation::counterClockwise);
			break;
		case (MagicCard::number_4.getId()):
			motionkit.rotate(4, Rotation::clockwise);
			break;
		case (MagicCard::number_5.getId()):
			motionkit.rotate(5, Rotation::counterClockwise);
			break;
		case (MagicCard::number_6.getId()):
			motionkit.rotate(6, Rotation::clockwise);
			break;
		case (MagicCard::number_7.getId()):
			motionkit.rotate(7, Rotation::counterClockwise);
			break;
		case (MagicCard::number_8.getId()):
			motionkit.startStabilisation();
			rtos::ThisThread::sleep_for(10s);
			motionkit.stop();
			break;
		case (MagicCard::number_9.getId()):
			motionkit.startStabilisation();
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

	imu::lsm6dsox.init();
	imukit.init();
	motionkit.init();
	rfidkit.init();

	rfidkit.onTagActivated(onMagicCardAvailable);

	while (true) {
		rtos::ThisThread::sleep_for(100ms);
	}
}
