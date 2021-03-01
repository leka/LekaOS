// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "mbed.h"

#include "HelloWorld.h"
#include "LekaBluetooth.h"
#include "LekaFirmware.h"
#include "LekaRFID.h"
#include "LekaScreen.h"
#include "LekaTouch.h"
#include "LekaWifi.h"

using namespace leka;

HelloWorld hello;
Bluetooth leka_bluetooth;
Firmware leka_firmware;
RFID leka_rfid;
Screen leka_screen;
Touch leka_touch;
Wifi leka_wifi;

static BufferedSerial serial(USBTX, USBRX, 9600);

constexpr uint8_t buff_size = 128;
char buff[buff_size] {};

Thread bluetooth_thread;
Thread firmware_thread;
Thread rfid_thread;
Thread screen_thread;
Thread touch_thread;
Thread wifi_thread;

Ticker kicker;
const uint32_t TIMEOUT_MS = 5000;

void watchdogKick()
{
	Watchdog::get_instance().kick();
}

int main(void)
{
	auto start = Kernel::Clock::now();

	printf("\nHello, Investigation Day!\n\n");

	rtos::ThisThread::sleep_for(2s);

	Watchdog &watchdog = Watchdog::get_instance();
	watchdog.start(TIMEOUT_MS);
	kicker.attach_us(watchdogKick, 1000);

	bluetooth_thread.start(callback(&leka_bluetooth, &Bluetooth::start));
	// firmware_thread.start(callback(&leka_firmware, &Firmware::start));
	rfid_thread.start(callback(&leka_rfid, &RFID::start));
	screen_thread.start(callback(&leka_screen, &Screen::start));
	touch_thread.start(callback(&leka_touch, &Touch::start));
	wifi_thread.start(callback(&leka_wifi, &Wifi::start));
	hello.start();

	while (true) {
		auto t	   = Kernel::Clock::now() - start;
		int length = sprintf(buff, "A message from your board %s --> \"%s\" at %i s\n", MBED_CONF_APP_TARGET_NAME,
							 hello.world, int(t.count() / 1000));
		serial.write(buff, length);
		rtos::ThisThread::sleep_for(1s);
	}

	return 0;
}
