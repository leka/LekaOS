#define USE_LEDS	  1
#define USE_RFID	  1
#define USE_TOUCH	  1
#define USE_MOTORS	  1
#define USE_HAPTIC	  1
#define USE_SD		  1
#define USE_SCREEN	  1
#define USE_BLE		  1
#define USE_BLUETOOTH 0

#include "mbed.h"

#if USE_BLE
	#include "BLEUtils.h"
Thread thread_ble;
#endif

#if USE_HAPTIC and USE_SD and !USE_BLUETOOTH
	#include "AudioUtils.h"
Thread thread_audio;
AnalogOut audio_output(MCU_SOUND_OUT);
#endif

#if USE_SD
	#include "FileManager.h"
leka::FileManager sd_card;
#endif

#if USE_LEDS
	#include "LedUtils.h"
Thread thread_led;
#endif

#if USE_SCREEN
	#include "LekaScreen.h"
Thread thread_lcd;
Screen lcd;
#endif

#if USE_MOTORS
	#include "MotorsUtils.h"
Thread thread_motors;
#endif

#include "WatchdogUtils.h"
Thread thread_watchdog;

DigitalOut audio_enable(SOUND_ENABLE, 1);
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

#if USE_MOTORS
	thread_motors.start(motor_thread);
#endif

#if USE_SCREEN
	thread_lcd.start({&lcd, &Screen::start});
#endif

	watchdog.start(5000);
	thread_watchdog.start(watchdog_thread);

#if USE_LEDS
	initLed();
	rtos::ThisThread::sleep_for(2s);
	thread_led.start(led_thread);
#endif

#if USE_HAPTIC and !USE_BLUETOOTH
	audio_pause_duration = 20s;
	thread_audio.start(callback(playSoundPeriodically, &audio_output));
#endif

#if USE_BLE
	BLE &ble = BLE::Instance();
	ble.onEventsToProcess(schedule_ble_events);
	BeaconDemo ble_beacon(ble, event_queue);
	thread_ble.start({&ble_beacon, &BeaconDemo::start});
#endif

	rtos::ThisThread::sleep_for(10s);
	while (true) {
		duration   = Kernel::Clock::now() - start;
		int length = sprintf(buff, "Leka is still alive after: %2i:%2i:%2i\nBattery at 0x%X\n\n",
							 int(std::chrono::duration_cast<std::chrono::hours>(duration).count()),
							 int(std::chrono::duration_cast<std::chrono::minutes>(duration).count()) % 60,
							 int(std::chrono::duration_cast<std::chrono::seconds>(duration).count()) % 60,
							 batteries_level.read_u16());
#if USE_BLUETOOTH
#else
		serial.write(buff, length);
#endif
		rtos::ThisThread::sleep_for(1s);
	}

	return 0;
}
