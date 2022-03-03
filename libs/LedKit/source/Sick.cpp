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
			stage2and6();
			break;
		case 3:
			stage3and7();
			break;
		case 4:
			stage4and8();
			break;
		case 5:
			stage5and9();
			break;
		case 6:
			stage2and6();
			break;
		case 7:
			stage3and7();
			break;
		case 8:
			stage4and8();
			break;
		case 9:
			stage5and9();
			break;
		case 10:
			stage10();
			break;
		default:
			break;
	}
	_belt.show();
}

auto Sick::mapStep(uint8_t step) const -> float
{
	constexpr auto kMaxInputValue = uint8_t {100};
	return utils::math::map(step, uint8_t {0}, kMaxInputValue, 0.F, 1.F);
}

void Sick::stage1()
{
	static constexpr auto green_sick		= RGB {0x10, 0xF0, 0x30};
	static constexpr auto kNumberOfLedsBelt = uint8_t {20};
	if (auto pos = mapStep(_step); pos != 1.F) {
		RGB color = ColorKit::colorGradient(RGB::black, green_sick, pos);
		_belt.setColorAtIndex(0, color);
		_belt.setColorAtIndex(kNumberOfLedsBelt - 1, color);
		_step++;
	} else {
		_stage++;
	}
}

void Sick::stage2and6()
{
	static constexpr auto green_sick		= RGB {0x10, 0xF0, 0x30};
	static constexpr auto kNumberOfLedsBelt = uint8_t {20};
	_belt.setColorAtIndex(_sneeze_index_1, green_sick);
	_belt.setColorAtIndex(kNumberOfLedsBelt - (_sneeze_index_1 + 1), green_sick);
	++_sneeze_index_1;
	if (_sneeze_index_1 > kNumberOfLedsBelt / 8) {
		++_stage;
	}
}

void Sick::stage3and7()
{
	static constexpr auto green_sick		= RGB {0x10, 0xF0, 0x30};
	static constexpr auto deep_green_sick	= RGB {0, 0x64, 0x10};
	static constexpr auto kNumberOfLedsBelt = uint8_t {20};
	_belt.setColorAtIndex(_sneeze_index_1, green_sick);
	_belt.setColorAtIndex(kNumberOfLedsBelt - (_sneeze_index_1 + 1), green_sick);
	_belt.setColorAtIndex(_sneeze_index_2, deep_green_sick);
	_belt.setColorAtIndex(kNumberOfLedsBelt - (_sneeze_index_2 + 1), deep_green_sick);
	++_sneeze_index_1;
	++_sneeze_index_2;
	if (_sneeze_index_1 > kNumberOfLedsBelt / 4) {
		++_stage;
	}
}

void Sick::stage4and8()
{
	static constexpr auto green_sick		= RGB {0x10, 0xF0, 0x30};
	static constexpr auto deep_green_sick	= RGB {0, 0x64, 0x10};
	static constexpr auto yellow_green_sick = RGB {0x10, 0x64, 0};
	static constexpr auto kNumberOfLedsBelt = uint8_t {20};
	_belt.setColorAtIndex(_sneeze_index_1, green_sick);
	_belt.setColorAtIndex(kNumberOfLedsBelt - (_sneeze_index_1 + 1), green_sick);
	_belt.setColorAtIndex(_sneeze_index_2, deep_green_sick);
	_belt.setColorAtIndex(kNumberOfLedsBelt - (_sneeze_index_2 + 1), deep_green_sick);
	_belt.setColorAtIndex(_sneeze_index_3, yellow_green_sick);
	_belt.setColorAtIndex(kNumberOfLedsBelt - (_sneeze_index_3 + 1), yellow_green_sick);
	++_sneeze_index_1;
	++_sneeze_index_2;
	++_sneeze_index_3;
	if (_sneeze_index_1 > 3 * kNumberOfLedsBelt / 8) {
		++_stage;
		_sneeze_index_1 = 0;
		_sneeze_index_2 = 0;
		_sneeze_index_3 = 0;
	}
}

void Sick::stage5and9()
{
	static constexpr auto green_sick		= RGB {0x10, 0xF0, 0x30};
	static constexpr auto deep_green_sick	= RGB {0, 0x64, 0x10};
	static constexpr auto yellow_green_sick = RGB {0x10, 0x64, 0};
	static constexpr auto blue_green_sick	= RGB {0x10, 0x64, 0x20};
	static constexpr auto kNumberOfLedsBelt = uint8_t {20};
	_belt.setColorAtIndex(_sneeze_index_1, green_sick);
	_belt.setColorAtIndex(kNumberOfLedsBelt - (_sneeze_index_1 + 1), green_sick);
	_belt.setColorAtIndex(_sneeze_index_2, deep_green_sick);
	_belt.setColorAtIndex(kNumberOfLedsBelt - (_sneeze_index_2 + 1), deep_green_sick);
	_belt.setColorAtIndex(_sneeze_index_3, yellow_green_sick);
	_belt.setColorAtIndex(kNumberOfLedsBelt - (_sneeze_index_3 + 1), yellow_green_sick);
	_belt.setColorAtIndex(_sneeze_index_4, blue_green_sick);
	_belt.setColorAtIndex(kNumberOfLedsBelt - (_sneeze_index_4 + 1), blue_green_sick);
	++_sneeze_index_1;
	++_sneeze_index_2;
	++_sneeze_index_3;
	++_sneeze_index_4;
	if (_sneeze_index_1 > kNumberOfLedsBelt / 2) {
		++_stage;
		_sneeze_index_1 = 0;
		_sneeze_index_2 = 0;
		_sneeze_index_3 = 0;
		_sneeze_index_3 = 0;
	}
}

void Sick::stage10()
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
