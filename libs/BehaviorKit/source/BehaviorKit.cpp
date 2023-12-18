// Leka - LekaOS
// Copyright 2023 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "BehaviorKit.h"
#include <algorithm>

using namespace leka;

BehaviorKit::BehaviorKit(interface::Timeout &timeout) : _timeout(timeout)
{
	_timeout.onTimeout([this] { stop(); });
}

void BehaviorKit::registerBehaviors(std::span<interface::Behavior *> behaviors)
{
	_behaviors = behaviors;
}

void BehaviorKit::setTimeoutDuration(std::chrono::seconds duration)
{
	_timeout_duration = duration;
}

void BehaviorKit::start(interface::Behavior *behavior)
{
	stop();

	_behavior = nullptr;
	for (auto *b: _behaviors) {
		if (b == behavior) {
			_behavior = b;
		}
	}

	if (_behavior == nullptr) {
		return;
	}

	_behavior->run();

	if (_timeout_duration != std::chrono::seconds {0}) {
		_timeout.start(_timeout_duration);
	}
}

void BehaviorKit::start(BehaviorID id)
{
	interface::Behavior *found_behavior = nullptr;

	for (auto *behavior: _behaviors) {
		if (id == behavior->id()) {
			found_behavior = behavior;
			break;
		}
	}

	start(found_behavior);
}

void BehaviorKit::stop()
{
	if (_behavior != nullptr) {
		_behavior->stop();
	}
	_timeout.stop();
}
