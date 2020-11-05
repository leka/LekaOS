#include "mbed.h"

#include "BLEUtils.h"

#include "MotorsUtils.h"
#include "WatchdogUtils.h"

Thread thread_motors;
// Thread thread_watchdog;

static BufferedSerial serial(USBTX, USBRX, 9600);

constexpr uint8_t buff_size = 128;
char buff[buff_size] {};

// Watchdog &watchdog = Watchdog::get_instance();

Thread thread_ble_event_queue;

void schedule_ble_events(BLE::OnEventsToProcessCallbackContext *context)
{
	event_queue.call(Callback<void()>(&context->ble, &BLE::processEvents));
}

int main(void)
{
	printf("\nHello, DOX of motors!\n\n");

	BLE &ble = BLE::Instance();

	ble.onEventsToProcess(schedule_ble_events);

	LEDDemo demo(ble, event_queue);

	demo.start();

	thread_ble_event_queue.start(callback(&event_queue, &EventQueue::dispatch_forever));

	printf("\nBLE Ready!\n\n");

	thread_motors.start(motor_thread);

	// watchdog.start(5000);
	// thread_watchdog.start(watchdog_thread);

	while (true) {
		// auto t	   = Kernel::Clock::now() - start;
		// int length = sprintf(buff, "Leka is still alive after: %i s\n", int(t.count() / 1000));
		// serial.write(buff, length);
		rtos::ThisThread::sleep_for(1s);
	}

	return 0;
}
