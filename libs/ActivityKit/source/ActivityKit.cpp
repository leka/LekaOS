// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "ActivityKit.h"

using namespace leka;

void ActivityKit::start(interface::Activity *activity)
{
	stop();

	_activity = activity;

	if (_activity == nullptr) {
		return;
	}
	_activity->start();
}

void ActivityKit::stop()
{
	if (_activity != nullptr) {
		_activity->stop();
	}
}
