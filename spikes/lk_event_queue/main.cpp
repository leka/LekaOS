// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "drivers/DigitalOut.h"
#include "rtos/ThisThread.h"

#include "CoreEventQueue.h"
#include "LogKit.h"

using namespace leka;
using namespace std::chrono;

mbed::DigitalOut blinky(LED1, 0);

class LEDManager
{
  public:
	explicit LEDManager() = default;

	void init() { event_queue.dispatch_forever(); }

	void turnOnRef(mbed::DigitalOut &led)
	{
		event_queue.call([&led] { led.write(1); });
	}

	void turnOffRef(mbed::DigitalOut &led)
	{
		event_queue.call([&led] { led.write(0); });
	}

	void blinkRef(mbed::DigitalOut &led)
	{
		for (auto i = 0; i < 10; ++i) {
			// ? this-> can be omitted but is kept for clarity
			event_queue.call([&led, this] { this->turnOnRef(led); });
			rtos::ThisThread::sleep_for(100ms);
			event_queue.call([&led, this] { this->turnOffRef(led); });
			rtos::ThisThread::sleep_for(100ms);
		}
	}

	void turnOnPtr(mbed::DigitalOut *led)
	{
		event_queue.call([led] { led->write(1); });
	}

	void turnOffPtr(mbed::DigitalOut *led)
	{
		event_queue.call([led] { led->write(0); });
	}

	void blinkPtr(mbed::DigitalOut *led)
	{
		for (auto i = 0; i < 10; ++i) {
			// ? this-> can be omitted but is kept for clarity
			event_queue.call([this, led] { this->turnOnPtr(led); });
			rtos::ThisThread::sleep_for(100ms);
			event_queue.call([this, led] { this->turnOffPtr(led); });
			rtos::ThisThread::sleep_for(100ms);
		}
	}

  private:
	CoreEventQueue event_queue {};
};

auto manager = LEDManager {};

static void void_function()
{
	log_info("void_function");
}

auto int_function_with_param(int i) -> int
{
	log_info("void_function with param: %i", i);
	return 1;
}

auto int_function_with_multiple_params(int i, const std::string &std, bool b) -> int
{
	log_info("void_function with param: %i, %s, %i", i, std.c_str(), b);
	return 1;
}

CoreEventQueue equeue {};

auto main() -> int
{
	logger::init();

	log_info("Hello, World!\n\n");

	manager.init();

	while (true) {
		log_info("Turn on ref");
		manager.turnOnRef(blinky);
		rtos::ThisThread::sleep_for(1s);

		log_info("Turn off ref");
		manager.turnOffRef(blinky);
		rtos::ThisThread::sleep_for(1s);

		log_info("Turn blink ref");
		manager.blinkRef(blinky);
		rtos::ThisThread::sleep_for(1s);

		log_info("Turn on pointer");
		manager.turnOnPtr(&blinky);
		rtos::ThisThread::sleep_for(1s);

		log_info("Turn off pointer");
		manager.turnOffPtr(&blinky);
		rtos::ThisThread::sleep_for(1s);

		log_info("Turn blink pointer");
		manager.blinkPtr(&blinky);
		rtos::ThisThread::sleep_for(1s);

		log_info("Call others");
		equeue.dispatch_forever();
		equeue.call(void_function);
		equeue.call(int_function_with_param, 42);
		int a		= 42;
		auto lambda = [&a] {
			log_info("lambda with capture: %i", a);
			return true;
		};
		equeue.call(lambda);
		equeue.call(int_function_with_multiple_params, 42, std::string {"Hello, World"}, true);
	}
}
