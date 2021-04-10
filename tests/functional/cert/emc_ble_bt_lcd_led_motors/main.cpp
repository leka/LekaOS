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
	#include "FileManager.h"
leka::FileManager sd_card;
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
Thread thread_motors;
#endif

#if USE_RFID
	#include "RFIDUtils.h"
char buff_rfid[40] {};
#endif

#include "WatchdogUtils.h"

Thread thread_watchdog;

AnalogIn batteries_level(BATTERY_VOLTAGE);

static BufferedSerial serial(USBTX, USBRX, 9600);

constexpr uint8_t buff_size = 128;
char buff[buff_size] {};

Watchdog &watchdog = Watchdog::get_instance();

void pairBluetooth()
{
	while (!leka_bluetooth.isPaired()) {
		leka_bluetooth.pairing();
		rtos::ThisThread::sleep_for(1s);
	}
}

int main(void)
{
	printf("\nHello, Investigation Day!\n\n");

	auto start	  = Kernel::Clock::now();
	auto duration = Kernel::Clock::now() - start;

	watchdog.start(5000);
	thread_watchdog.start(watchdog_thread);

#if USE_BLUETOOTH
	thread_bluetooth.start(pairBluetooth);
#endif

#if USE_MOTORS
	leka::LKCoreMotor motor_right(PinName::MOTOR_RIGHT_DIRECTION_1, PinName::MOTOR_RIGHT_DIRECTION_2,
								  PinName::MOTOR_RIGHT_PWM);
	leka::LKCoreMotor motor_left(PinName::MOTOR_LEFT_DIRECTION_1, PinName::MOTOR_LEFT_DIRECTION_2,
								 PinName::MOTOR_LEFT_PWM);

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
		int length = sprintf(buff, "Leka is still alive after: %2i:%2i:%2i\nBattery at 0x%X\n\n",
							 int(std::chrono::duration_cast<std::chrono::hours>(duration).count()),
							 int(std::chrono::duration_cast<std::chrono::minutes>(duration).count()) % 60,
							 int(std::chrono::duration_cast<std::chrono::seconds>(duration).count()) % 60,
							 batteries_level.read_u16());
#if USE_BLUETOOTH_AS_SERIAL
		leka_bluetooth.sendMessage(buff, length);
#else
		serial.write(buff, length);
#endif

#if USE_TOUCH
		length = sprintf(buff, "Touch value: %x | LF(+1) LB(+2) RB(+4) RF(+8) LE(+10) RE(+20)\n",
						 leka_touch.updateSensorsStatus());
	#if USE_BLUETOOTH_AS_SERIAL
		leka_bluetooth.sendMessage(buff, length);
	#else
		serial.write(buff, length);
	#endif
#endif

#if USE_RFID
		getRfid(buff_rfid);
		length = snprintf(buff, buff_size, "RFID response is %s\n\n", buff_rfid);
	#if USE_BLUETOOTH_AS_SERIAL
		leka_bluetooth.sendMessage(buff, length);
	#else
		serial.write(buff, length);
	#endif
#endif

		rtos::ThisThread::sleep_for(1s);
	}
}
