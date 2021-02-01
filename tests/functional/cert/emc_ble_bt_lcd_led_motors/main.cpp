#include "mbed.h"

#include "BLEUtils.h"

#include "LedUtils.h"
#include "LekaScreen.h"
#include "MotorsUtils.h"
#include "WatchdogUtils.h"

Thread thread_led;
Thread thread_motors;
Thread thread_watchdog;
Thread thread_lcd;
Thread thread_ble;

Screen lcd;
AnalogIn batteries_level(BATTERY_VOLTAGE);

static BufferedSerial serial(USBTX, USBRX, 9600);

constexpr uint8_t buff_size = 128;
char buff[buff_size] {};

Watchdog &watchdog = Watchdog::get_instance();

int main(void)
{
	printf("\nHello, Investigation Day!\n\n");

	auto start	  = Kernel::Clock::now();
	auto duration = Kernel::Clock::now() - start;

	thread_motors.start(motor_thread);
	thread_lcd.start({&lcd, &Screen::start});

	watchdog.start(5000);
	thread_watchdog.start(watchdog_thread);

	initLed();
	rtos::ThisThread::sleep_for(2s);
	thread_led.start(led_thread);

	BLE &ble = BLE::Instance();
	ble.onEventsToProcess(schedule_ble_events);
	BeaconDemo ble_beacon(ble, event_queue);
	thread_ble.start({&ble_beacon, &BeaconDemo::start});

	rtos::ThisThread::sleep_for(10s);
	while (true) {
		duration   = Kernel::Clock::now() - start;
		int length = sprintf(buff, "Leka is still alive after: %2i:%2i:%2i\nBattery at 0x%X\n\n",
							 int(std::chrono::duration_cast<std::chrono::hours>(duration).count()),
							 int(std::chrono::duration_cast<std::chrono::minutes>(duration).count()) % 60,
							 int(std::chrono::duration_cast<std::chrono::seconds>(duration).count()) % 60,
							 batteries_level.read_u16());
		serial.write(buff, length);
		rtos::ThisThread::sleep_for(1s);
	}

	return 0;
}
