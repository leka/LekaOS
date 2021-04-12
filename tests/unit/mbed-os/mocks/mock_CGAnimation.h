
// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_ANIMATION_MOCK_H_
#define _LEKA_OS_ANIMATION_MOCK_H_

#include "CGAnimation.h"
#include "gmock/gmock.h"

namespace leka::animation {

class AnimationMock : public interface::CGAnimation
{
  public:
	MOCK_METHOD(void, start, (), (override));
	MOCK_METHOD(void, run, (), (override));
	MOCK_METHOD(void, stop, (), (override));
};

}	// namespace leka::animation

#endif	 // _LEKA_OS_ANIMATION_MOCK_H_
