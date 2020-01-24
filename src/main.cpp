#include "mbed.h"

uint8_t myCount = 0;

int main(void) {
	while (true) {
		printf("%d - Hello, World!\n", myCount);
		myCount++;
		rtos::ThisThread::sleep_for(1000);
	}

	return 0;
}
