#include "HelloWorld/HelloWorld.h"
#include "mbed.h"

HelloWorld hello;
uint8_t myCount = 0;

int main(void) {
	hello.start();

	while (true) {
		printf("%d - %s\n", myCount, hello.world);
		myCount++;
		rtos::ThisThread::sleep_for(1000);
	}

	return 0;
}
