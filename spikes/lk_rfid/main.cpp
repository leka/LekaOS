#include "mbed.h"
#include <cstddef>
#include <cstdint>

#include "drivers/BufferedSerial.h"
#include "rtos/ThisThread.h"
#include "rtos/Thread.h"

#include "CoreBufferedSerial.h"
#include "CoreCR95HF.h"
#include "HelloWorld.h"
#include "LogKit.h"
#include "RFIDKit.h"

using namespace leka;
using namespace std::chrono;

EventQueue eventQueue;
Thread thread;

auto main() -> int
{
	static auto log_serial = mbed::BufferedSerial(USBTX, USBRX, 115200);
	leka::logger::set_print_function([](const char *str, size_t size) { log_serial.write(str, size); });

	auto start = rtos::Kernel::Clock::now();

	log_info("Hello, World!\n\n");

	rtos::ThisThread::sleep_for(2s);

	auto mbed_serial = mbed::BufferedSerial(RFID_UART_TX, RFID_UART_RX, 57600);
	auto rfid_serial = CoreBufferedSerial(mbed_serial);
	auto rfid_reader = CoreCR95HF(rfid_serial, thread, eventQueue);
	auto core_rfid	 = RFIDKit(rfid_reader);

	core_rfid.init();

	HelloWorld hello;
	hello.start();

	while (true) {
		rtos::ThisThread::sleep_for(10ms);
	}
}
