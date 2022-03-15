// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// ? LCOV_EXCL_START - Exclude from coverage report

#include "Sick.h"

#include "MathUtils.h"

namespace leka::led::animation {

void Sick::start()
{
	turnLedBlack();
}

void Sick::stop()
{
	turnLedBlack();
}

void Sick::run()
{
	switch (_stage) {
		case 1:
			stage1();
			break;
		case 2:
			stage2and5();
			break;
		case 3:
			stage3and6();
			break;
		case 4:
			stage4and7();
			break;
		case 5:
			stage2and5();
			break;
		case 6:
			stage3and6();
			break;
		case 7:
			stage4and7();
			break;
		case 8:
			stage8();
			break;
		default:
			break;
	}
	_belt.show();
}

auto Sick::mapStep(uint8_t step, uint8_t max_input_value) const -> float
{
	return utils::math::map(step, uint8_t {0}, max_input_value, 0.F, 1.F);
}

void Sick::stage1()
{
	static constexpr auto green_sick = RGB {0x10, 0xF0, 0x30};

	static constexpr auto kMaxInputValue	= uint8_t {120};
	static constexpr auto kNumberOfLedsBelt = uint8_t {20};
	if (auto pos = mapStep(_step, kMaxInputValue); pos != 1.F) {
		RGB color = ColorKit::colorGradient(RGB::black, green_sick, pos);
		_belt.setColorAtIndex(0, color);
		_belt.setColorAtIndex(kNumberOfLedsBelt - 1, color);
		_step++;
	} else {
		_step = 0;
		_stage++;
	}
}

void Sick::stage2and5()
{
	static constexpr auto green_sick = RGB {0x10, 0xF0, 0x30};

	static constexpr auto kMaxInputValue2	= uint8_t {1};
	static constexpr auto kNumberOfLedsBelt = uint8_t {20};
	if (auto pos = mapStep(_step, kMaxInputValue2); pos != 1.F) {
		_belt.setColorAtIndex(_sneeze_index_1, green_sick);
		_belt.setColorAtIndex(kNumberOfLedsBelt - (_sneeze_index_1 + 1), green_sick);
		_step++;
	} else {
		++_sneeze_index_1;
		_step = 0;
	}
	if (_sneeze_index_1 > kNumberOfLedsBelt / 8) {
		++_stage;
	}
}

void Sick::stage3and6()
{
	static constexpr auto green_sick	  = RGB {0x10, 0xF0, 0x30};
	static constexpr auto deep_green_sick = RGB {0, 0x64, 0x10};

	static constexpr auto kMaxInputValue2	= uint8_t {1};
	static constexpr auto kNumberOfLedsBelt = uint8_t {20};
	if (auto pos = mapStep(_step, kMaxInputValue2); pos != 1.F) {
		_belt.setColorAtIndex(_sneeze_index_1, green_sick);
		_belt.setColorAtIndex(kNumberOfLedsBelt - (_sneeze_index_1 + 1), green_sick);
		_belt.setColorAtIndex(_sneeze_index_2, deep_green_sick);
		_belt.setColorAtIndex(kNumberOfLedsBelt - (_sneeze_index_2 + 1), deep_green_sick);
		_step++;
	} else {
		++_sneeze_index_1;
		++_sneeze_index_2;
		_step = 0;
	}
	if (_sneeze_index_1 > kNumberOfLedsBelt / 4) {
		++_stage;
	}
}

void Sick::stage4and7()
{
	static constexpr auto green_sick		= RGB {0x10, 0xF0, 0x30};
	static constexpr auto deep_green_sick	= RGB {0, 0x64, 0x10};
	static constexpr auto yellow_green_sick = RGB {0x10, 0x64, 0};

	static constexpr auto kMaxInputValue2	= uint8_t {1};
	static constexpr auto kNumberOfLedsBelt = uint8_t {20};
	if (auto pos = mapStep(_step, kMaxInputValue2); pos != 1.F) {
		_belt.setColorAtIndex(_sneeze_index_1, green_sick);
		_belt.setColorAtIndex(kNumberOfLedsBelt - (_sneeze_index_1 + 1), green_sick);
		_belt.setColorAtIndex(_sneeze_index_2, deep_green_sick);
		_belt.setColorAtIndex(kNumberOfLedsBelt - (_sneeze_index_2 + 1), deep_green_sick);
		_belt.setColorAtIndex(_sneeze_index_3, yellow_green_sick);
		_belt.setColorAtIndex(kNumberOfLedsBelt - (_sneeze_index_3 + 1), yellow_green_sick);
		_step++;
	} else {
		++_sneeze_index_1;
		++_sneeze_index_2;
		++_sneeze_index_3;
		_step = 0;
	}
	if (_sneeze_index_1 > 3 * kNumberOfLedsBelt / 8) {
		++_stage;
		_sneeze_index_1 = 0;
		_sneeze_index_2 = 0;
		_sneeze_index_3 = 0;
	}
}

void Sick::stage8()
{
	static constexpr auto kNumberOfLedsBelt = uint8_t {20};
	_belt.setColorAtIndex(_sneeze_index_1, RGB::black);
	_belt.setColorAtIndex(kNumberOfLedsBelt - (_sneeze_index_1 + 1), RGB::black);
	++_sneeze_index_1;
	if (_sneeze_index_1 == 0) {
		++_stage;
	}
}

void Sick::turnLedBlack()
{
	_ears.setColor(RGB::black);
	_belt.setColor(RGB::black);
	_ears.show();
	_belt.show();
}

}	// namespace leka::led::animation

// ? LCOV_EXCL_STOP - Exclude from coverage report
