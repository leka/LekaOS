// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "drivers/BufferedSerial.h"
#include "drivers/Ticker.h"
#include "drivers/Watchdog.h"
#include "rtos/ThisThread.h"
#include "rtos/Thread.h"

#include "CoreFlashUtils.h"
#include "HelloWorld.h"
#include "LekaBluetooth.h"
#include "LekaRFID.h"
#include "LekaScreen.h"
#include "LekaTouch.h"
#include "LekaWifi.h"
#include "LogKit.h"

using namespace leka;
using namespace std::chrono;

HelloWorld hello;
Bluetooth leka_bluetooth;
RFID leka_rfid;
Screen leka_screen;
Touch leka_touch;
Wifi leka_wifi;

static mbed::BufferedSerial serial(USBTX, USBRX, 115200);

constexpr uint8_t buff_size = 128;
std::array<char, buff_size> buff {};

rtos::Thread bluetooth_thread;
rtos::Thread flash_thread;
rtos::Thread rfid_thread;
rtos::Thread screen_thread;
rtos::Thread touch_thread;
rtos::Thread wifi_thread;

mbed::Ticker kicker;
const uint32_t TIMEOUT_MS = 5000;

void watchdogKick()
{
	mbed::Watchdog::get_instance().kick();
}

auto main() -> int
{
	log_info("Hello, Investigation Day!\n\n");

	auto start = rtos::Kernel::Clock::now();

	auto &watchdog = mbed::Watchdog::get_instance();
	watchdog.start(TIMEOUT_MS);
	kicker.attach_us(watchdogKick, 1000);

	bluetooth_thread.start(mbed::callback(&leka_bluetooth, &Bluetooth::start));
	flash_thread.start(flash_loop);
	rfid_thread.start(mbed::callback(&leka_rfid, &RFID::start));
	screen_thread.start(mbed::callback(&leka_screen, &Screen::start));
	touch_thread.start(mbed::callback(&leka_touch, &Touch::start));
	wifi_thread.start(mbed::callback(&leka_wifi, &Wifi::start));
	hello.start();

	while (true) {
		auto t = rtos::Kernel::Clock::now() - start;
		log_info("A message from your board %s --> \"%s\" at %i s\n", MBED_CONF_APP_TARGET_NAME, hello.world,
				 int(t.count() / 1000));

		rtos::ThisThread::sleep_for(1s);
	}
}
