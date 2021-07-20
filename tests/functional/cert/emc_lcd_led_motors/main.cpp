#include "mbed.h"

#include "CoreMotor.h"
#include "CorePwm.h"
#include "LedUtils.h"
#include "LekaScreen.h"
#include "MotorsUtils.h"
#include "WatchdogUtils.h"

using namespace leka;

Thread thread_led;
Thread thread_motors;
Thread thread_watchdog;
Thread thread_lcd;

Screen lcd;

static BufferedSerial serial(USBTX, USBRX, 9600);

constexpr uint8_t buff_size = 128;
char buff[buff_size] {};

Watchdog &watchdog = Watchdog::get_instance();

int main(void)
{
	printf("\nHello, Investigation Day!\n\n");

	auto start = Kernel::Clock::now();

	auto motor_left_dir_1 = mbed::DigitalOut {MOTOR_LEFT_DIRECTION_1};
	auto morot_left_dir_2 = mbed::DigitalOut {MOTOR_LEFT_DIRECTION_2};
	auto morot_left_speed = CorePwm {MOTOR_LEFT_PWM};

	auto motor_right_dir_1 = mbed::DigitalOut {MOTOR_RIGHT_DIRECTION_1};
	auto morot_right_dir_2 = mbed::DigitalOut {MOTOR_RIGHT_DIRECTION_2};
	auto morot_right_speed = CorePwm {MOTOR_RIGHT_PWM};

	auto motor_left	 = CoreMotor {motor_left_dir_1, morot_left_dir_2, morot_left_speed};
	auto motor_right = CoreMotor {motor_right_dir_1, morot_right_dir_2, morot_right_speed};

	Motors motors {.left = motor_left, .right = motor_right};

	thread_motors.start(callback(motor_thread, &motors));

	thread_lcd.start({&lcd, &Screen::start});

	watchdog.start(5000);
	thread_watchdog.start(watchdog_thread);

	initLed();
	rtos::ThisThread::sleep_for(2s);
	thread_led.start(led_thread);

	while (true) {
		auto t	   = Kernel::Clock::now() - start;
		int length = sprintf(buff, "Leka is still alive after: %i s\n", int(t.count() / 1000));
		serial.write(buff, length);
		rtos::ThisThread::sleep_for(1s);
	}
}
