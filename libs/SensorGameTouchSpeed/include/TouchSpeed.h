#pragma once

#include "RGB.h"
namespace leka {
class TouchSpeed
{
  public:
	void HelloWorld();
	auto determineMaximumDuration(int level) -> int;
	auto is_target_touched() -> bool;

	auto setRandomCaptorWithColors() -> void;

	auto update_touched_colors() -> void;
	auto setColorWinIfWon(leka::RGB) -> void;
	auto setColorLoseIfLose(leka::RGB) -> void;

	auto playOneRound(int maximum_duration) -> bool;
	void MaxDurationForRound();
	auto isMaxDurationRound() -> bool;
	auto setFlags() -> void;
	auto isTargetHit() -> bool;
	auto isTargetHitBeforeTimesUp() -> bool;

	void playLevel(int level);
	void isNextLevelReached();

	void game();
};
}	// namespace leka
