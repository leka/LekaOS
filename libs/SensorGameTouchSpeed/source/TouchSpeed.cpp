#include "TouchSpeed.h"

#include "ColorKit.h"
#include "TouchSensorKit.h"
namespace leka {

void TouchSpeed::HelloWorld() {}

int TouchSpeed::determineMaximumDuration(int level)
{
	int easiestLevel	= 6000;
	int maximumDuration = easiestLevel - 1000 * level;
	return maximumDuration;
}

auto TouchSpeed::is_target_touched() -> bool {}

auto TouchSpeed::setRandomCaptorWithColors() -> void {}

auto TouchSpeed::update_touched_colors() -> void {}
auto TouchSpeed::setColorWinIfWon(leka::RGB) -> void {}
auto TouchSpeed::setColorLoseIfLose(leka::RGB) -> void {}

auto TouchSpeed::playOneRound(int maximum_duration) -> bool {}
void TouchSpeed::MaxDurationForRound() {}
auto TouchSpeed::isMaxDurationRound() -> bool {}
auto TouchSpeed::setFlags() -> void {}
auto isTargetHit() -> bool {}
auto isTargetHitBeforeTimesUp() -> bool;

void TouchSpeed::playLevel(int level) {};

void TouchSpeed::game() {};
void TouchSpeed::isNextLevelReached() {};
}	// namespace leka
