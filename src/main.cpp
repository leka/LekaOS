#include "mbed.h"
#include "HelloWorld.h"

HelloWorld hello;

static BufferedSerial serial(USBTX, USBRX, 9600);

constexpr uint8_t buff_size = 128;
char buff[buff_size] {};

int main(void) {

	auto start = Kernel::Clock::now();

	hello.start();

	while (true) {
		auto t = Kernel::Clock::now() - start;
		int c_size = sprintf(buff, "A message from your board %s --> \"%s\" at %i s\n", MBED_CONF_APP_TARGET_NAME, hello.world, int(t.count()/1000));
		serial.write(buff, c_size);
		rtos::ThisThread::sleep_for(1s);
	}

	return 0;
}
