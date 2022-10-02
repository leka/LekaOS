/* CHOICE OF FEATURES */
#define USE_LEDS	  1
#define USE_RFID	  1
#define USE_TOUCH	  1
#define USE_MOTORS	  1
#define USE_HAPTIC	  1
#define USE_SD		  1
#define USE_SCREEN	  1
#define USE_BLE		  1
#define USE_BLUETOOTH 1

#include "LogKit.h"

#if USE_BLUETOOTH
	#define USE_BLUETOOTH_AS_SERIAL 1
#endif
/* END OF CHOICE OF FEATURES */

#include "mbed.h"

#if USE_BLE
	#include "BLEUtils.h"
Thread thread_ble;
#endif

#if USE_HAPTIC and USE_SD
DigitalOut audio_enable(SOUND_ENABLE, 1);
	#include "AudioUtils.h"
Thread thread_audio;
AnalogOut audio_output(MCU_SOUND_OUT);
#endif

#if USE_SD
	#include "FATFileSystem.h"
	#include "SDBlockDevice.h"
auto sd_bd = SDBlockDevice {SD_SPI_MOSI, SD_SPI_MISO, SD_SPI_SCK};
auto fatfs = FATFileSystem {"fs"};
#endif

#if USE_LEDS
	#include "LedUtils.h"
Thread thread_led;
#endif

#if USE_BLUETOOTH
	#include "LekaBluetooth.h"
Thread thread_bluetooth;
Bluetooth leka_bluetooth;
#endif

#if USE_SCREEN
	#include "LekaScreen.h"
Thread thread_lcd;
Screen lcd;
#endif

#if USE_TOUCH
	#include "LekaTouch.h"
Touch leka_touch;
#endif

#if USE_MOTORS
	#include "MotorsUtils.h"
	#include "CorePwm.h"
Thread thread_motors;
#endif

#if USE_RFID
	#include "RFIDUtils.h"
auto rfid_buffer = std::array<char, 40> {};
#endif

#include "WatchdogUtils.h"

using namespace leka;

Thread thread_watchdog;

AnalogIn batteries_level(BATTERY_VOLTAGE);

constexpr auto serial_buffer_size = uint8_t {128};
auto serial_buffer				  = std::array<char, serial_buffer_size> {};

Watchdog &watchdog = Watchdog::get_instance();

#if USE_BLUETOOTH
void pairBluetooth()
{
	while (!leka_bluetooth.isPaired()) {
		leka_bluetooth.pairing();
		rtos::ThisThread::sleep_for(1s);
	}
}
#endif

#if USE_SD
void initializeSD()
{
	constexpr auto default_sd_bd_frequency = uint64_t {25'000'000};

	sd_bd.init();
	sd_bd.frequency(default_sd_bd_frequency);

	fatfs.mount(&sd_bd);
}
#endif

auto main() -> int
{
	logger::init();

	log_info("Hello, Investigation Day!\n");

	auto start	  = Kernel::Clock::now();
	auto duration = Kernel::Clock::now() - start;

	watchdog.start(5000);
	thread_watchdog.start(watchdog_thread);

#if USE_BLUETOOTH
	thread_bluetooth.start(pairBluetooth);
#endif

#if USE_MOTORS
	auto motor_left_dir_1 = mbed::DigitalOut {MOTOR_LEFT_DIRECTION_1};
	auto morot_left_dir_2 = mbed::DigitalOut {MOTOR_LEFT_DIRECTION_2};
	auto morot_left_speed = CorePwm {MOTOR_LEFT_PWM};

	auto motor_right_dir_1 = mbed::DigitalOut {MOTOR_RIGHT_DIRECTION_1};
	auto morot_right_dir_2 = mbed::DigitalOut {MOTOR_RIGHT_DIRECTION_2};
	auto morot_right_speed = CorePwm {MOTOR_RIGHT_PWM};

	auto motor_left	 = CoreMotor {motor_left_dir_1, morot_left_dir_2, morot_left_speed};
	auto motor_right = CoreMotor {motor_right_dir_1, morot_right_dir_2, morot_right_speed};

	Motors motors {.left = motor_left, .right = motor_right};

	thread_motors.start(callback(motor_thread, &motors));
#endif

#if USE_SCREEN
	thread_lcd.start({&lcd, &Screen::start});
#endif

#if USE_LEDS
	initLed();
	rtos::ThisThread::sleep_for(2s);
	thread_led.start(led_thread);
#endif

#if USE_SD
	initializeSD();
#endif

#if USE_HAPTIC and USE_SD
	audio_pause_duration = 20s;
	thread_audio.start(callback(playSoundPeriodically, &audio_output));
#endif

#if USE_BLE
	BLE &ble = BLE::Instance();
	ble.onEventsToProcess(schedule_ble_events);
	BeaconDemo ble_beacon(ble, event_queue);
	thread_ble.start({&ble_beacon, &BeaconDemo::start});
#endif

#if USE_RFID
	initRfid();
#endif

	rtos::ThisThread::sleep_for(10s);
	while (true) {
		duration   = Kernel::Clock::now() - start;
		int length = snprintf(
			serial_buffer.data(), serial_buffer_size, "Leka is still alive after: %2i:%2i:%2i\nBattery at 0x%X\n",
			int(std::chrono::duration_cast<std::chrono::hours>(duration).count()),
			int(std::chrono::duration_cast<std::chrono::minutes>(duration).count()) % 60,
			int(std::chrono::duration_cast<std::chrono::seconds>(duration).count()) % 60, batteries_level.read_u16());
#if USE_BLUETOOTH_AS_SERIAL
		leka_bluetooth.sendMessage(serial_buffer.data(), length);
#else
		log_info(serial_buffer.data());
#endif

#if USE_TOUCH
		length = snprintf(serial_buffer.data(), serial_buffer_size,
						  "Touch value: %x | LF(+1) LB(+2) RB(+4) RF(+8) LE(+10) RE(+20)\n",
						  leka_touch.updateSensorsStatus());
	#if USE_BLUETOOTH_AS_SERIAL
		leka_bluetooth.sendMessage(serial_buffer.data(), length);
	#else
		log_info(serial_buffer.data());
	#endif
#endif

#if USE_RFID
		getRfid(rfid_buffer.data());
		length = snprintf(serial_buffer.data(), serial_buffer_size, "RFID response is %s\n", rfid_buffer.data());
	#if USE_BLUETOOTH_AS_SERIAL
		leka_bluetooth.sendMessage(serial_buffer.data(), length);
	#else
		log_info(serial_buffer.data());
	#endif
#endif

		rtos::ThisThread::sleep_for(1s);
	}
}
