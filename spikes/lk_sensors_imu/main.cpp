// #include "mbed.h"

#include "PinNames.h"

#include "drivers/BufferedSerial.h"
#include "drivers/DigitalOut.h"
#include "drivers/PwmOut.h"
#include "platform/Callback.h"
#include "rtos/ThisThread.h"
#include "rtos/Thread.h"

#include "CoreMotor.h"
#include "CorePwm.h"
#include "DigitalOut.h"
#include "HelloWorld.h"
#include "LSM6DSOX_CommunicationI2C.h"
#include "LSM6DSOX_ComponentAccelerometer.h"
#include "LSM6DSOX_ComponentGyroscope.h"
#include "LSM6DSOX_DataGatherer.h"
#include "LogKit.h"

//##################################################################################################
// Defines
//##################################################################################################

#define PIN_I2C1_SDA	  (SENSOR_IMU_TH_I2C_SDA)
#define PIN_I2C1_SCL	  (SENSOR_IMU_TH_I2C_SCL)
#define PIN_LSM6DSOX_INT1 (SENSOR_IMU_IRQ)

using namespace leka;
using namespace std::chrono;

void tickerCB();

I2C i2c1(PIN_I2C1_SDA, PIN_I2C1_SCL);
Communication::LSM6DSOX_I2C lsm6dsox_i2c(i2c1);

// Component::LSM6DSOX_DataGatherer dataGatherer(lsm6dsox_i2c, PIN_LSM6DSOX_INT1, 10.0f, USBTX, USBRX,
// MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE, tickerCB);
Component::LSM6DSOX_DataGatherer dataGatherer(lsm6dsox_i2c, PIN_LSM6DSOX_INT1, 10.0f, USBTX, USBRX, 115200, tickerCB);

// DigitalInOut INT_1_LSM6DSOX(PIN_LSM6DSOX_INT1, PIN_OUTPUT, PullNone,
// 							0);	  // This line fix the use of LSM6DSOX on X-NUCLEO_IKS01A2

DigitalIn INT_1_LSM6DSOX(PIN_LSM6DSOX_INT1, PullNone);

// UnbufferedSerial serial(USBTX, USBRX);

void spinLeft(CoreMotorBase &left, CoreMotorBase &right, float speed)
{
	left.spin(Rotation::clockwise, speed);
	right.spin(Rotation::clockwise, speed);
}

void spinRight(CoreMotorBase &left, CoreMotorBase &right, float speed)
{
	left.spin(Rotation::counterClockwise, speed);
	right.spin(Rotation::counterClockwise, speed);
}

void goForward(CoreMotorBase &left, CoreMotorBase &right, float speed)
{
	left.spin(Rotation::counterClockwise, speed);
	right.spin(Rotation::clockwise, speed);
}

void goBackward(CoreMotorBase &left, CoreMotorBase &right, float speed)
{
	left.spin(Rotation::clockwise, speed);
	right.spin(Rotation::counterClockwise, speed);
}

void stop(CoreMotorBase &left, CoreMotorBase &right)
{
	left.stop();
	right.stop();
}

auto main() -> int
{
	static auto serial = mbed::BufferedSerial(USBTX, USBRX, 115200);
	leka::logger::set_print_function([](const char *str, size_t size) { serial.write(str, size); });

	auto motor_left_dir_1 = mbed::DigitalOut {MOTOR_LEFT_DIRECTION_1};
	auto motor_left_dir_2 = mbed::DigitalOut {MOTOR_LEFT_DIRECTION_2};
	auto motor_left_speed = CorePwm {MOTOR_LEFT_PWM};

	auto motor_right_dir_1 = mbed::DigitalOut {MOTOR_RIGHT_DIRECTION_1};
	auto motor_right_dir_2 = mbed::DigitalOut {MOTOR_RIGHT_DIRECTION_2};
	auto motor_right_speed = CorePwm {MOTOR_RIGHT_PWM};

	auto motor_left	 = CoreMotor {motor_left_dir_1, motor_left_dir_2, motor_left_speed};
	auto motor_right = CoreMotor {motor_right_dir_1, motor_right_dir_2, motor_right_speed};

	// Time for sensor init
	ThisThread::sleep_for(10ms);   // waiting for sensor startup

	// Greetings
	printf("  Starting a new run\n\n");

	// Restore default configuration
	dataGatherer.restoreDefaultConfiguration();

	// Disable I3C and setting INT1 pin as input
	// dataGatherer.disableI3C();
	// INT_1_LSM6DSOX.input();	  // setting up INT1 pin as input (for interrupts to work)

	// Components init
	dataGatherer.init();

	// Enable Block Data Update
	lsm6dsox_block_data_update_set(dataGatherer.TMP_getIoFunc(), PROPERTY_ENABLE);

	dataGatherer.setDataRate(26);

	printf("Start by configuring the data gathering. Then call \"start\" to start receiving data\n");
	printf("Write \"help\" for info on the functionnalities\n");
	printf(
		"If you are using mbed sterm don't forget to type \"CTRL + E\" to disable local echo and avoid corrupting the "
		"data\n");
	printf("\n\n");

	std::array<float, 6> data;

	while (true) {
		//	dataGatherer.onTick();
		dataGatherer.getData(data);

		// for (int i = 0; i < 5; i++) {
		// 	printf("%f ", data.at(i));
		// }
		// printf("\n");

		// rtos::ThisThread::sleep_for(50ms);

		while (abs(data[1]) > 100) {
			if (data[1] < 0) {		 // tilted on the right
				if (data[1] < 0) {	 // tilted forward
					motor_right.spin(Rotation::counterClockwise, 0.5);
					motor_left.spin(Rotation::clockwise, 0.5);
				} else {   // tilted backward
					motor_right.spin(Rotation::clockwise, 0.5);
					motor_left.spin(Rotation::counterClockwise, 0.5);
				}
			} else {				 // tilted on the left
				if (data[1] > 0) {	 // tilted forward
					motor_left.spin(Rotation::clockwise, 0.5);
					motor_right.spin(Rotation::counterClockwise, 0.5);
				} else {   // tilted backward
					motor_left.spin(Rotation::counterClockwise, 0.5);
					motor_right.spin(Rotation::clockwise, 0.5);
				}
			}

			dataGatherer.getData(data);
			rtos::ThisThread::sleep_for(50ms);
		}

		if (abs(data[0]) > 100) {
			while (abs(data[0]) > 80) {
				dataGatherer.getData(data);
				// printf("speed : %f\n", abs(data[0]) / 1000.f + 0.2 * (1000 - abs(data[0])) / 1000);
				rtos::ThisThread::sleep_for(50ms);
				float speed = abs(data[0]) / 1000.f;
				if (data[0] < 0) {
					goForward(motor_left, motor_right, abs(data[0]) / 1000.f);
				} else {
					goBackward(motor_left, motor_right, abs(data[0]) / 1000.f);
				}
			}
		}

		stop(motor_left, motor_right);
	}
	return 0;
}

void tickerCB()
{
	// dataGatherer.onTick();
}
