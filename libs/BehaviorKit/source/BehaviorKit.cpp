// Leka - LekaOS
// Copyright 2023 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "BehaviorKit.h"
#include <algorithm>

using namespace leka;

BehaviorKit::BehaviorKit(interface::EventLoop &event_loop) : _event_loop(event_loop)
{
	_event_loop.registerCallback([this] { run(); });
}

void BehaviorKit::registerBehaviors(std::span<interface::Behavior *> behaviors)
{
	_behaviors = behaviors;
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

	_event_loop.start();
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

void BehaviorKit::run()
{
	_behavior->run();
}

void BehaviorKit::stop()
{
	if (_behavior != nullptr) {
		_behavior->stop();
	}
}
