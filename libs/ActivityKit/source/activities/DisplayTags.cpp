// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// LCOV_EXCL_START

#include "DisplayTags.h"

namespace leka::activity {

void DisplayTags::start(const std::function<void()> &before_process_callback)
{
	_videokit.displayImage("fs/home/img/system/robot-misc-robot-misc-screen_empty_white.jpg");

	_backup_callback = _rfidkit.getCallback();

	_rfidkit.onTagActivated([this, &before_process_callback](const MagicCard &card) {
		before_process_callback();
		processCard(card);
	});
}

void DisplayTags::stop()
{
	_rfidkit.onTagActivated(_backup_callback);
}

void DisplayTags::processCard(const MagicCard &card)
{
	// TODO (@HPezz) Remove special case when fix is available
	// ! Temporary fix while we wait for a change in fs
	if (card == MagicCard::math_arithmetic_addition_sign_plus) {
		_videokit.displayImage("fs/home/img/id/003B.jpg");
	} else if (card == MagicCard::math_arithmetic_substraction_sign_minus) {
		_videokit.displayImage("fs/home/img/id/003C.jpg");
	} else {
		snprintf(_path_buffer.data(), _path_buffer.size(), "fs/home/img/id/%.4x.jpg", card.getId());
		_videokit.displayImage(_path_buffer.data());

		_backup_callback(card);
	}
};

}	// namespace leka::activity

// LCOV_EXCL_STOP
