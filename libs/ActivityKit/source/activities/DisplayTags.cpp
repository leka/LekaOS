// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// LCOV_EXCL_START

#include "DisplayTags.h"

namespace leka::activity {

void DisplayTags::start()
{
	_videokit.displayImage("fs/home/img/system/robot-misc-robot-misc-screen_empty_white.jpg");

	_backup_callback = _rfidkit.getCallback();

	auto on_tag_detected_callback = [this](const MagicCard &card) {
		snprintf(_path_buffer.data(), _path_buffer.size(), "fs/home/img/id/%.4x.jpg", card.getId());
		_videokit.displayImage(_path_buffer.data());

		_backup_callback(card);
	};

	_rfidkit.onTagActivated(on_tag_detected_callback);
}

void DisplayTags::stop()
{
	_rfidkit.onTagActivated(_backup_callback);
}

}	// namespace leka::activity

// LCOV_EXCL_STOP
