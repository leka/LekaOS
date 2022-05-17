// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "../../../include/StateMachine.h"	 // TODO (@ladislas) - remove relative path
#include "CoreMutex.h"
#include "SMLogger.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace leka;
using namespace leka::system;

namespace lksm = leka::system::robot::sm;

using testing::Return;

class StateMachineTest : public testing::Test
{
  protected:
	// void SetUp() override {}
	// void TearDown() override {}

	robot::sm::logger logger {};

	boost::sml::sm<robot::StateMachine, boost::sml::testing, boost::sml::logger<robot::sm::logger>,
				   boost::sml::thread_safe<CoreMutex>>
		sm {logger};
};
