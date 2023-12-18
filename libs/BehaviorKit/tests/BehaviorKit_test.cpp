// Leka - LekaOS
// Copyright 2023 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "BehaviorKit.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "mocks/leka/Behavior.h"
#include "mocks/leka/Timeout.h"

using namespace leka;
using namespace std::chrono_literals;

using ::testing::Return;

class BehaviorKitTest : public ::testing::Test
{
  protected:
	BehaviorKitTest() = default;

	// void SetUp() override {}
	// void TearDown() override {}

	mock::Timeout mock_timeout {};
	BehaviorKit behaviorkit {mock_timeout};

	mock::Behavior mock_behavior_a {};
	mock::Behavior mock_behavior_b {};
};

TEST_F(BehaviorKitTest, initialization)
{
	ASSERT_NE(&behaviorkit, nullptr);
}

TEST_F(BehaviorKitTest, initialize)
{
	EXPECT_CALL(mock_timeout, onTimeout);

	behaviorkit.init();
}

TEST_F(BehaviorKitTest, startFirstBehavior)
{
	EXPECT_CALL(mock_timeout, onTimeout);
	behaviorkit.init();

	auto behaviors = std::to_array<interface::Behavior *>({&mock_behavior_a});
	behaviorkit.registerBehaviors(behaviors);

	EXPECT_CALL(mock_timeout, stop);
	EXPECT_CALL(mock_timeout, start);
	EXPECT_CALL(mock_behavior_a, run);

	behaviorkit.start(&mock_behavior_a);
}

TEST_F(BehaviorKitTest, startBehaviorNullPtr)
{
	EXPECT_CALL(mock_timeout, onTimeout);
	behaviorkit.init();

	EXPECT_CALL(mock_timeout, stop);
	EXPECT_CALL(mock_timeout, start).Times(0);

	behaviorkit.start(nullptr);
}

TEST_F(BehaviorKitTest, startFirstBehaviorID)
{
	EXPECT_CALL(mock_timeout, onTimeout);
	behaviorkit.init();

	auto behaviors = std::to_array<interface::Behavior *>({
		&mock_behavior_a,
		&mock_behavior_b,
	});
	behaviorkit.registerBehaviors(behaviors);

	auto behavior_a_id = BehaviorID {1};
	auto behavior_b_id = BehaviorID {2};

	EXPECT_CALL(mock_timeout, stop);
	EXPECT_CALL(mock_timeout, start);
	EXPECT_CALL(mock_behavior_a, id).WillRepeatedly(Return(behavior_a_id));
	EXPECT_CALL(mock_behavior_b, id).WillRepeatedly(Return(behavior_b_id));
	EXPECT_CALL(mock_behavior_b, run);

	behaviorkit.start(behavior_b_id);
}

TEST_F(BehaviorKitTest, startBehaviorIDNotRegistered)
{
	EXPECT_CALL(mock_timeout, onTimeout);
	behaviorkit.init();

	auto behaviors = std::to_array<interface::Behavior *>({
		&mock_behavior_a,
		&mock_behavior_b,
	});
	behaviorkit.registerBehaviors(behaviors);

	auto behavior_a_id = BehaviorID {1};
	auto behavior_b_id = BehaviorID {2};

	EXPECT_CALL(mock_timeout, stop);
	EXPECT_CALL(mock_timeout, start).Times(0);
	EXPECT_CALL(mock_behavior_a, id).WillRepeatedly(Return(behavior_a_id));
	EXPECT_CALL(mock_behavior_b, id).WillRepeatedly(Return(behavior_b_id));

	behaviorkit.start(BehaviorID {3});
}

TEST_F(BehaviorKitTest, startBehaviorNotRegistered)
{
	EXPECT_CALL(mock_timeout, onTimeout);
	behaviorkit.init();

	auto behaviors = std::to_array<interface::Behavior *>({
		&mock_behavior_a,
	});
	behaviorkit.registerBehaviors(behaviors);

	EXPECT_CALL(mock_timeout, stop);
	EXPECT_CALL(mock_timeout, start).Times(0);
	EXPECT_CALL(mock_behavior_b, run).Times(0);

	behaviorkit.start(&mock_behavior_b);
}

TEST_F(BehaviorKitTest, startAnyBehavior)
{
	EXPECT_CALL(mock_timeout, onTimeout);
	behaviorkit.init();

	auto behaviors = std::to_array<interface::Behavior *>({
		&mock_behavior_a,
		&mock_behavior_b,
	});
	behaviorkit.registerBehaviors(behaviors);

	EXPECT_CALL(mock_timeout, stop);
	EXPECT_CALL(mock_timeout, start);
	EXPECT_CALL(mock_behavior_a, run);

	behaviorkit.start(&mock_behavior_a);

	EXPECT_CALL(mock_timeout, stop);
	EXPECT_CALL(mock_timeout, start);
	EXPECT_CALL(mock_behavior_a, stop);
	EXPECT_CALL(mock_behavior_b, run);

	behaviorkit.start(&mock_behavior_b);
}

TEST_F(BehaviorKitTest, setTimeoutDuration)
{
	EXPECT_CALL(mock_timeout, onTimeout);
	behaviorkit.init();

	auto behaviors = std::to_array<interface::Behavior *>({&mock_behavior_a});
	behaviorkit.registerBehaviors(behaviors);

	behaviorkit.setTimeoutDuration(0s);
	EXPECT_CALL(mock_timeout, stop);
	EXPECT_CALL(mock_timeout, start).Times(0);
	EXPECT_CALL(mock_behavior_a, stop);
	EXPECT_CALL(mock_behavior_a, run);
	behaviorkit.start(&mock_behavior_a);

	behaviorkit.setTimeoutDuration(10s);
	EXPECT_CALL(mock_timeout, stop);
	EXPECT_CALL(mock_timeout, start(std::chrono::microseconds {10s}));
	EXPECT_CALL(mock_behavior_a, run);
	behaviorkit.start(&mock_behavior_a);

	behaviorkit.setTimeoutDuration(20s);
	EXPECT_CALL(mock_timeout, stop);
	EXPECT_CALL(mock_timeout, start(std::chrono::microseconds {20s}));
	EXPECT_CALL(mock_behavior_a, stop);
	EXPECT_CALL(mock_behavior_a, run);
	behaviorkit.start(&mock_behavior_a);
}
