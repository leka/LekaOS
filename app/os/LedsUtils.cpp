#include "LedsUtils.h"
#include <array>

#include "rtos/ThisThread.h"

using namespace leka;
using namespace std::chrono_literals;

const TProgmemPalette16 fire_palette_p = {CRGB::Red, 0xFF3300, 0xFF6600, 0xFF9900, 0xFFCC00, 0xFFFF00,

										  CRGB::Red, 0xFF1500, 0xFF2A00, 0xFF3F00, 0xFF5400,

										  CRGB::Red, 0xFF1500, 0xFF2A00, 0xFF3F00, 0xFF5400};

void LedsUtils::initialize()
{
	leds_controller.addLeds<SK9822, LED_BELT_SPI_MOSI, LED_BELT_SPI_SCK, BGR>(leds_belt_control.data(),
																			  leds_belt_output, leds_belt_count);
	leds_controller.addLeds<SK9822, LED_EARS_SPI_MOSI, LED_EARS_SPI_SCK, BGR>(leds_ears_control.data(),
																			  leds_ears_output, leds_ears_count);

	turnOff(LedsRange::all);
}

void LedsUtils::initializationAnimation()
{
	setBrightness(0x10);
	for (auto &led: leds_belt_control) {
		for (uint16_t intensity = 0; intensity < 0x10; intensity += 0x1) {
			led = CRGB(intensity, intensity, intensity);
			leds_controller.show();
			rtos::ThisThread::sleep_for(10ms);
		}
	}
	for (int i = 0; i < 3; i++) {
		turnOff(LedsRange::belt);
		rtos::ThisThread::sleep_for(400ms);
		setBrightness(0x10);
		turnOn(LedsRange::belt, CRGB::White);
		rtos::ThisThread::sleep_for(400ms);
	}
	rtos::ThisThread::sleep_for(800ms);

	turnOff(LedsRange::all);
}

void LedsUtils::setBrightness(uint8_t brightness)
{
	if (brightness != 0x00) {
		_brightness = brightness;
	}
	leds_controller.setBrightness(brightness);
}

void LedsUtils::turnOff(LedsRange range)
{
	auto is_belt = [&]() { return (range & LedsRange::belt) == LedsRange::belt; };
	auto is_ears = [&]() { return (range & LedsRange::ears) == LedsRange::ears; };
	auto is_all	 = [&]() { return (range & LedsRange::all) == LedsRange::all; };

	if (is_all()) {
		leds_controller.setBrightness(0x00);
		leds_controller.clear();
	} else {
		if (is_belt()) {
			for (auto &led: leds_belt_control) {
				led = 0;
			}
		}
		if (is_ears()) {
			for (auto &led: leds_ears_control) {
				led = 0;
			}
		}
	}

	leds_controller.show();
}

void LedsUtils::turnOn(LedsRange range, CRGB color)
{
	auto is_belt			 = [&]() { return (range & LedsRange::belt) == LedsRange::belt; };
	auto is_belt_front_right = [&]() { return (range & LedsRange::front_right) == LedsRange::front_right; };
	auto is_belt_back_right	 = [&]() { return (range & LedsRange::back_right) == LedsRange::back_right; };
	auto is_belt_back_left	 = [&]() { return (range & LedsRange::back_left) == LedsRange::back_left; };
	auto is_belt_front_left	 = [&]() { return (range & LedsRange::front_left) == LedsRange::front_left; };

	auto is_ears	  = [&]() { return (range & LedsRange::ears) == LedsRange::ears; };
	auto is_ear_left  = [&]() { return (range & LedsRange::ear_left) == LedsRange::ear_left; };
	auto is_ear_right = [&]() { return (range & LedsRange::ear_right) == LedsRange::ear_right; };

	setBrightness(_brightness);

	if (is_belt()) {
		for (auto &led: leds_belt_control) {
			led = color;
		}
	} else {
		if (is_belt_front_left()) {
			for (auto index = 0; index < 5; index++) {
				leds_belt_control.at(index) = color;
			}
		}
		if (is_belt_back_left()) {
			for (auto index = 5; index < 10; index++) {
				leds_belt_control.at(index) = color;
			}
		}
		if (is_belt_back_right()) {
			for (auto index = 10; index < 15; index++) {
				leds_belt_control.at(index) = color;
			}
		}
		if (is_belt_front_right()) {
			for (auto index = 15; index < 20; index++) {
				leds_belt_control.at(index) = color;
			}
		}
	}

	if (is_ears()) {
		for (auto &led: leds_ears_control) {
			led = color;
		}
	} else {
		if (is_ear_left()) {
			leds_ears_control.at(0) = color;
		}
		if (is_ear_right()) {
			leds_ears_control.at(1) = color;
		}
	}

	leds_controller.show();
}

void LedsUtils::pulsation(CRGB color)
{
	auto initial_brightness = _brightness;

	for (int pulser_value = 0x1E; pulser_value > 0x0A; pulser_value--) {
		setBrightness(pulser_value);
		turnOn(LedsRange::belt, color);
		rtos::ThisThread::sleep_for(50ms);
	}
	for (int pulser_value = 0x0A; pulser_value < 0x1E; pulser_value++) {
		setBrightness(pulser_value);
		turnOn(LedsRange::belt, color);
		rtos::ThisThread::sleep_for(50ms);
	}

	_brightness = initial_brightness;
}

void LedsUtils::runGlitterReinforcer()
{
	auto get_random_color = []() { return std::rand() / ((RAND_MAX + 1u) / 0x1000000); };

	turnOff(LedsRange::all);

	for (int j = 0; j < 17; j++) {
		for (int q = 0; q < 3; q++) {
			for (int i = 0; i < leds_belt_count; i += 2) {
				if (i + q < leds_belt_count) {
					leds_belt_control[i + q] = get_random_color();
				}
			}
			setBrightness(_brightness);
			leds_controller.show();

			rtos::ThisThread::sleep_for(100ms);
			turnOff(LedsRange::all);
		}
	}

	turnOff(LedsRange::all);
}

void LedsUtils::runGreenReinforcer()
{
	auto colors = std::to_array<CRGB>({CRGB {52, 201, 36}, CRGB {1, 215, 88}});

	for (uint8_t i = 0; i < 5; i++) {
		for (auto &color_to_display: colors) {
			turnOn(LedsRange::all, color_to_display);
			rtos::ThisThread::sleep_for(300ms);
			turnOff(LedsRange::all);
			rtos::ThisThread::sleep_for(200ms);
		}
	}
}

void LedsUtils::runVioletGreenBlinkReinforcer()
{
	auto colors = std::to_array<CRGB>({CRGB {255, 0, 255}, CRGB {50, 255, 120}});

	for (uint8_t i = 0; i < 10; i++) {
		turnOn(LedsRange::belt, colors.at(i % 2));
		turnOn(LedsRange::ears, colors.at((i + 1) % 2));
		rtos::ThisThread::sleep_for(500ms);
	}
	turnOff(LedsRange::all);
}

void LedsUtils::runPaletteColors(CRGBPalette16 palette)
{
	int start_index_palette = 0;

	while (start_index_palette < 714) {
		start_index_palette++;

		{
			auto index_palette = start_index_palette;
			for (auto &led: leds_belt_control) {
				led = ColorFromPalette(palette, index_palette, _brightness, _blending);
				index_palette += 3;
			}
		}

		leds_controller.show();
		rtos::ThisThread::sleep_for(7ms);
	}
}

void LedsUtils::runReinforcer(LedsReinforcer reinforcer)
{
	setBrightness(_brightness);

	switch (reinforcer) {
		case LedsReinforcer::green:
			runGreenReinforcer();
			break;
		case LedsReinforcer::violet_green_blink:
			runVioletGreenBlinkReinforcer();
			break;
		case LedsReinforcer::fire:
			runPaletteColors(fire_palette_p);
			break;
		case LedsReinforcer::glitters:
			runGlitterReinforcer();
			break;
		case LedsReinforcer::rainbow:
			runPaletteColors(RainbowColors_p);
			break;
		default:
			return;
	}
}
