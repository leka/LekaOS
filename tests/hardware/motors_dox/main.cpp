#include "mbed.h"

#include "MotorsUtils.h"
#include "SDUtils.h"
#include "WatchdogUtils.h"

Thread thread_motors;
Thread thread_watchdog;

static BufferedSerial serial(USBTX, USBRX, 9600);

constexpr uint8_t buff_size = 128;
char buff[buff_size] {};

Watchdog &watchdog = Watchdog::get_instance();

int main(void)
{
	printf("\nHello, DOX of motors!\n\n");

	auto start = Kernel::Clock::now();

	thread_motors.start(motor_thread);

	watchdog.start(5000);
	thread_watchdog.start(watchdog_thread);

	while (true) {
		auto t	   = Kernel::Clock::now() - start;
		int length = sprintf(buff, "Leka is still alive after: %i s\n", int(t.count() / 1000));
		serial.write(buff, length);
		rtos::ThisThread::sleep_for(1s);
	}

	return 0;
}
