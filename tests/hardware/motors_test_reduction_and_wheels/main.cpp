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

// This projet is intended to test different types of motors using BLE to control their movement
// An app such as NRF Connect has to be installed on your iPhone/iPad
// When scanning, you should be able to discover the robot named "LekaDev"
// When connected, you'll have access to the `0xA000` sercie, with two characteristics:
//
//     - `0xA001` --> the motor will roll, turn, spin, roll backward
//     - `0xA002` --> the robot will accelerate and decelerate progressively
//
// To start/stop each behavior, you need to write `bool = true` or `byte = 0x01` to the behavior
// you want to test.

int main(void)
{
	printf("\nHello, BLE Motors Test!\n\n");

	BLE &ble = BLE::Instance();

	ble.onEventsToProcess(schedule_ble_events);

	MotorsBLEProgram motors_ble(ble, event_queue);

	motors_ble.start();

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
