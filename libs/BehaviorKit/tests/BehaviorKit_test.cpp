// Leka - LekaOS
// Copyright 2023 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "BehaviorKit.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "mocks/leka/Behavior.h"
#include "stubs/leka/EventLoopKit.h"

using namespace leka;

using ::testing::Return;

class BehaviorKitTest : public ::testing::Test
{
  protected:
	BehaviorKitTest() = default;

	// void SetUp() override {}
	// void TearDown() override {}

	stub::EventLoopKit stub_event_loop {};
	BehaviorKit behaviorkit {stub_event_loop};

	mock::Behavior mock_behavior_a {};
	mock::Behavior mock_behavior_b {};
};

TEST_F(BehaviorKitTest, initialization)
{
	ASSERT_NE(&behaviorkit, nullptr);
}

TEST_F(BehaviorKitTest, startFirstBehavior)
{
	auto behaviors = std::to_array<interface::Behavior *>({&mock_behavior_a});
	behaviorkit.registerBehaviors(behaviors);

	EXPECT_CALL(mock_behavior_a, run);

	behaviorkit.start(&mock_behavior_a);
}

TEST_F(BehaviorKitTest, startBehaviorNullPtr)
{
	// nothing expected

	behaviorkit.start(nullptr);
}

TEST_F(BehaviorKitTest, startFirstBehaviorID)
{
	auto behaviors = std::to_array<interface::Behavior *>({
		&mock_behavior_a,
		&mock_behavior_b,
	});
	behaviorkit.registerBehaviors(behaviors);

	auto behavior_a_id = BehaviorID {1};
	auto behavior_b_id = BehaviorID {2};

	EXPECT_CALL(mock_behavior_a, id).WillRepeatedly(Return(behavior_a_id));
	EXPECT_CALL(mock_behavior_b, id).WillRepeatedly(Return(behavior_b_id));
	EXPECT_CALL(mock_behavior_b, run);

	behaviorkit.start(behavior_b_id);
}

TEST_F(BehaviorKitTest, startBehaviorIDNotRegistered)
{
	auto behaviors = std::to_array<interface::Behavior *>({
		&mock_behavior_a,
		&mock_behavior_b,
	});
	behaviorkit.registerBehaviors(behaviors);

	auto behavior_a_id = BehaviorID {1};
	auto behavior_b_id = BehaviorID {2};

	EXPECT_CALL(mock_behavior_a, id).WillRepeatedly(Return(behavior_a_id));
	EXPECT_CALL(mock_behavior_b, id).WillRepeatedly(Return(behavior_b_id));

	behaviorkit.start(BehaviorID {3});
}

TEST_F(BehaviorKitTest, startBehaviorNotRegistered)
{
	auto behaviors = std::to_array<interface::Behavior *>({
		&mock_behavior_a,
	});
	behaviorkit.registerBehaviors(behaviors);

	EXPECT_CALL(mock_behavior_b, run).Times(0);

	behaviorkit.start(&mock_behavior_b);
}

TEST_F(BehaviorKitTest, startAnyBehavior)
{
	auto behaviors = std::to_array<interface::Behavior *>({
		&mock_behavior_a,
		&mock_behavior_b,
	});
	behaviorkit.registerBehaviors(behaviors);

	EXPECT_CALL(mock_behavior_a, run);

	behaviorkit.start(&mock_behavior_a);

	EXPECT_CALL(mock_behavior_a, stop);
	EXPECT_CALL(mock_behavior_b, run);

	behaviorkit.start(&mock_behavior_b);
}
