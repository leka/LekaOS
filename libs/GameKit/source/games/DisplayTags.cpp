// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// LCOV_EXCL_START

#include "DisplayTags.h"

namespace leka::game {

void DisplayTags::start()
{
	_videokit.displayImage("fs/home/img/system/robot-misc-robot-misc-screen_empty_white.jpg");
	_backup_callback			  = _rfidkit.getCallback();
	auto on_tag_detected_callback = [this](const MagicCard &card) {
		if (card == MagicCard::remote_standard) {
			stop();
		} else {
			std::stringstream sstream;
			sstream << std::hex << card.getId();
			_videokit.displayImage("fs/home/img/id/00" + sstream.str() + ".jpg");
		}
	};
	_rfidkit.onTagActivated(on_tag_detected_callback);
}

void DisplayTags::stop()
{
	_rfidkit.onTagActivated(_backup_callback);
}

}	// namespace leka::game

// LCOV_EXCL_STOP
