// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// LCOV_EXCL_START

#include "Underwater.h"

namespace leka::led::animation {

static constexpr auto blue_water = RGB {0x00, 128, 255};

void Underwater::setLeds(interface::LED &ears, interface::LED &belt)
{
	_ears = &ears;
	_belt = &belt;
}

auto Underwater::isRunning() -> bool
{
	return _running;
}

void Underwater::start()
{
	if (_ears == nullptr || _belt == nullptr) {
		return;
	}

	turnLedBlack();
	_step			= 0;
	_stage			= 0;
	step_fish_1		= 0;
	step_fish_2		= 0;
	step_fish_3		= 0;
	step_fish_4		= 0;
	position_fish_1 = -2;
	position_fish_2 = -2;
	position_fish_3 = -2;
	position_fish_4 = -2;
	_running		= true;
}

void Underwater::stop()
{
	if (_ears == nullptr || _belt == nullptr) {
		return;
	}

	turnLedBlack();
	_running = false;
}

void Underwater::run()
{
	if (_ears == nullptr || _belt == nullptr) {
		return;
	}

	switch (_stage) {
		case 0:
			stage0();
			break;
		case 1:
			stage1();
			break;
		case 2:
			stage2();
			break;
		case 3:
			stage3();
			break;
		case 4:
			stage4();
			break;
		case 5:
			stage5();
			break;
		case 6:
			stage6();
			break;
		case 7:
			stage7();
			break;
		case 8:
			stage8();
			break;
		case 9:
			stage9();
			break;
		case 10:
			stage10();
			break;
		case 11:
			stage11();
			break;
		default:
			_running = false;
			break;
	}
	_belt->show();
}

void Underwater::stage0()
{
	static constexpr auto kMaxInputValueStageWater = uint8_t {80};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValueStageWater); pos <= 1.F) {
		++_step;
	} else {
		_step = 0;
		++_stage;
	}
}

void Underwater::stage1()
{
	static constexpr auto kTresholdStage1		   = 0.5F / 2;
	static constexpr auto kMaxInputValueStageWater = uint8_t {80};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValueStageWater); pos <= kTresholdStage1) {
		RGB color = ColorKit::colorGradient(RGB::black, blue_water, pos);
		_belt->setColor(color);
		++_step;
	} else {
		++_stage;
	}
}

void Underwater::stage2()
{
	static constexpr auto kTresholdStage2		   = 0.4F;
	static constexpr auto kMaxInputValueStageWater = uint8_t {80};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValueStageWater); pos >= kTresholdStage2) {
		RGB color = ColorKit::colorGradient(RGB::black, blue_water, pos);
		_belt->setColor(color);
		--_step;
	} else {
		++_stage;
	}
}

void Underwater::stage3()
{
	static constexpr auto kTresholdStage3		   = 0.85F;
	static constexpr auto kMaxInputValueStageWater = uint8_t {80};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValueStageWater); pos <= kTresholdStage3) {
		RGB color = ColorKit::colorGradient(RGB::black, blue_water, pos);
		_belt->setColor(color);
		++_step;
	} else {
		++_stage;
	}
}

void Underwater::stage4()
{
	static constexpr auto kTresholdStage4		   = 0.6F;
	static constexpr auto kMaxInputValueStageWater = uint8_t {80};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValueStageWater); pos >= kTresholdStage4) {
		RGB color = ColorKit::colorGradient(RGB::black, blue_water, pos);
		_belt->setColor(color);
		--_step;
	} else {
		++_stage;
	}
}

void Underwater::stage5()
{
	static constexpr auto kTresholdStage5		   = 0.9F;
	static constexpr auto kMaxInputValueStageWater = uint8_t {80};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValueStageWater); pos <= kTresholdStage5) {
		RGB color = ColorKit::colorGradient(RGB::black, blue_water, pos);
		_belt->setColor(color);
		++_step;
	} else {
		++_stage;
	}
}

void Underwater::stage6()
{
	static constexpr auto kTresholdStage6		   = 0.6F;
	static constexpr auto kMaxInputValueStageWater = uint8_t {80};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValueStageWater); pos >= kTresholdStage6) {
		RGB color = ColorKit::colorGradient(RGB::black, blue_water, pos);
		_belt->setColor(color);
		--_step;
	} else {
		++_stage;
	}
}

void Underwater::stage7()
{
	static constexpr auto kMaxInputValueStageWater = uint8_t {80};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValueStageWater); pos != 1.F) {
		RGB color = ColorKit::colorGradient(RGB::black, blue_water, pos);
		_belt->setColor(color);
		++_step;
	} else {
		_step = 0;
		++_stage;
	}
}

void Underwater::stage8()
{
	static constexpr auto kMaxInputValueStageWater = uint8_t {20};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValueStageWater); pos != 1.F) {
		_belt->setColor(blue_water);
		++_step;
	} else {
		_step = 0;
		++_stage;
	}
}

void Underwater::stage9()
{
	static constexpr auto kFishSpeed1			  = uint8_t {15};
	static constexpr auto kFishSpeed2			  = uint8_t {18};
	static constexpr auto kMaxInputValueStageFish = uint8_t {160};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValueStageFish); pos >= 0.F) {
		showFishAtRight(RGB::pure_blue, position_fish_1, kFishSpeed1, step_fish_1);
	}
	if (auto pos = utils::normalizeStep(_step, kMaxInputValueStageFish); pos >= 1.F / 4) {
		showFishAtLeft(RGB::pure_red, position_fish_2, kFishSpeed2, step_fish_3);
	}
	if (auto pos = utils::normalizeStep(_step, kMaxInputValueStageFish); pos == 1.F) {
		++_stage;
		_step = 0;
		return;
	}
	_step++;
}

void Underwater::stage10()
{
	static constexpr auto kFishSpeed1 = uint8_t {15};
	static constexpr auto kFishSpeed2 = uint8_t {18};

	static constexpr auto kMaxInputValueStageFish  = uint8_t {160};
	static constexpr auto kMaxInputValueStageWater = uint8_t {80};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValueStageFish); pos >= 0.F) {
		showFishAtLeft(RGB::yellow, position_fish_3, kFishSpeed1, step_fish_1);
	}
	if (auto pos = utils::normalizeStep(_step, kMaxInputValueStageFish); pos >= 1.F / 4) {
		showFishAtRight(RGB::pure_red, position_fish_4, kFishSpeed2, step_fish_2);
	}
	if (auto pos = utils::normalizeStep(_step, kMaxInputValueStageFish); pos == 1.F) {
		++_stage;
		_step = kMaxInputValueStageWater;
		return;
	}
	++_step;
}

void Underwater::stage11()
{
	static constexpr auto kMaxInputValueStageWater = uint8_t {80};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValueStageWater); pos != 0.F) {
		RGB color = ColorKit::colorGradient(RGB::black, blue_water, pos);
		_belt->setColor(color);
		--_step;
	} else {
		++_stage;
	}
}

void Underwater::showFishAtRight(const RGB &fish_color, int &fish_position, uint8_t fish_speed, uint8_t &step_fish)
{
	static constexpr auto kNumberOfLedsBelt = uint8_t {20};
	if (auto pos = utils::normalizeStep(step_fish, fish_speed); pos != 1.F) {
		if (fish_position - 1 >= 0 && fish_position - 1 < kNumberOfLedsBelt / 2 && pos <= 1.F / 2) {
			setColorCurrentMinusOne(fish_position - 1, pos, fish_color);
		}
		if (fish_position >= 0 && fish_position < kNumberOfLedsBelt / 2) {
			setColorCurrent(fish_position, pos, fish_color);
		}
		if (fish_position + 1 >= 0 && fish_position + 1 < kNumberOfLedsBelt / 2) {
			setColorCurrentPlusOne(fish_position + 1, pos, fish_color);
		}
		if (fish_position + 2 >= 0 && fish_position + 2 < kNumberOfLedsBelt / 2) {
			setColorCurrentPlusTwo(fish_position + 2, pos, fish_color);
		}
		if (fish_position + 3 >= 0 && fish_position + 3 < kNumberOfLedsBelt / 2 && pos >= 1.F / 2) {
			setColorCurrentPlusThree(fish_position + 3, pos, fish_color);
		}
		++step_fish;
	} else {
		step_fish = 0;
		fish_position += 2;
	}
}

void Underwater::showFishAtLeft(const RGB &fish_color, int &fish_position, uint8_t fish_speed, uint8_t &step_fish)
{
	static constexpr auto kNumberOfLedsBelt = uint8_t {20};
	if (auto pos = utils::normalizeStep(step_fish, fish_speed); pos != 1.F) {
		if (fish_position - 1 >= 0 && fish_position - 1 < kNumberOfLedsBelt / 2 && pos <= 1.F / 2) {
			setColorCurrentMinusOne(kNumberOfLedsBelt - (fish_position - 1), pos, fish_color);
		}
		if (fish_position >= 0 && fish_position < kNumberOfLedsBelt / 2) {
			setColorCurrent(kNumberOfLedsBelt - fish_position, pos, fish_color);
		}
		if (fish_position + 1 >= 0 && fish_position + 1 < kNumberOfLedsBelt / 2) {
			setColorCurrentPlusOne(kNumberOfLedsBelt - (fish_position + 1), pos, fish_color);
		}
		if (fish_position + 2 >= 0 && fish_position + 2 < kNumberOfLedsBelt / 2) {
			setColorCurrentPlusTwo(kNumberOfLedsBelt - (fish_position + 2), pos, fish_color);
		}
		if (fish_position + 3 >= 0 && fish_position + 3 < kNumberOfLedsBelt / 2 && pos >= 1.F / 2) {
			setColorCurrentPlusThree(kNumberOfLedsBelt - (fish_position + 3), pos, fish_color);
		}
		++step_fish;
	} else {
		step_fish = 0;
		fish_position += 2;
	}
}

void Underwater::setColorCurrentMinusOne(int index, float pos, const RGB &color_movement)
{
	RGB color = ColorKit::colorGradient(blue_water, color_movement, 1.F / 2 - pos);
	_belt->setColorAtIndex(index, color);
}

void Underwater::setColorCurrent(int index, float pos, const RGB &color_movement)
{
	RGB color = ColorKit::colorGradient(blue_water, color_movement, 1.F - pos);

	_belt->setColorAtIndex(index, color);
}

void Underwater::setColorCurrentPlusOne(int index, float pos, const RGB &color_movement)
{
	if (pos <= 1.F / 2) {
		RGB color = ColorKit::colorGradient(blue_water, color_movement, pos + 1.F / 2);
		_belt->setColorAtIndex(index, color);
	} else {
		RGB color = ColorKit::colorGradient(blue_water, color_movement, 1.F + 1.F / 2 - pos);
		_belt->setColorAtIndex(index, color);
	}
}

void Underwater::setColorCurrentPlusTwo(int index, float pos, const RGB &color_movement)
{
	RGB color = ColorKit::colorGradient(blue_water, color_movement, pos);
	_belt->setColorAtIndex(index, color);
}

void Underwater::setColorCurrentPlusThree(int index, float pos, const RGB &color_movement)
{
	RGB color = ColorKit::colorGradient(blue_water, color_movement, pos - 1.F / 2);
	_belt->setColorAtIndex(index, color);
}

void Underwater::turnLedBlack()
{
	_ears->setColor(RGB::black);
	_belt->setColor(RGB::black);
	_ears->show();
	_belt->show();
}

}	// namespace leka::led::animation

// LCOV_EXCL_STOP
