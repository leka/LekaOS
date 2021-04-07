#include "mbed.h"

#include "LedUtils.h"
#include "LekaScreen.h"
#include "MotorsUtils.h"
#include "WatchdogUtils.h"

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

	thread_motors.start(motor_thread);
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
