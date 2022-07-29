// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "GameKit.h"

#include "gtest/gtest.h"
#include "mocks/Game.h"
#include "stubs/leka/EventLoopKit.h"

using namespace leka;
using namespace std::chrono;

using ::testing::InSequence;

class GameKitTest : public ::testing::Test
{
  protected:
	GameKitTest() = default;

	// void SetUp() override {}
	// void TearDown() override {}

	stub::EventLoopKit stub_event_loop {};

	GameKit gamekit {stub_event_loop};

	mock::Game mock_game {};
};

TEST_F(GameKitTest, initialization)
{
	EXPECT_NE(&mock_game, nullptr);
}

TEST_F(GameKitTest, startGame)
{
	EXPECT_CALL(mock_game, startCalled).Times(1);
	EXPECT_FALSE(mock_game.isRunning());

	gamekit.start(&mock_game);
	EXPECT_TRUE(mock_game.isRunning());
}

TEST_F(GameKitTest, startNullPtr)
{
	EXPECT_CALL(mock_game, startCalled).Times(0);
	EXPECT_FALSE(mock_game.isRunning());

	gamekit.start(nullptr);
	EXPECT_FALSE(mock_game.isRunning());
}

TEST_F(GameKitTest, runGame)
{
	auto kMaxStageNumber = 10;
	EXPECT_CALL(mock_game, startCalled).Times(1);
	EXPECT_CALL(mock_game, stageCalled).Times(kMaxStageNumber);

	gamekit.init();
	gamekit.start(&mock_game);
}

TEST_F(GameKitTest, stopWithoutGame)
{
	EXPECT_CALL(mock_game, stopCalled).Times(0);

	gamekit.stop();
}

TEST_F(GameKitTest, stopStartedGame)
{
	EXPECT_CALL(mock_game, startCalled).Times(1);
	EXPECT_CALL(mock_game, stopCalled).Times(1);
	EXPECT_FALSE(mock_game.isRunning());

	gamekit.start(&mock_game);
	EXPECT_TRUE(mock_game.isRunning());

	gamekit.stop();
	EXPECT_FALSE(mock_game.isRunning());
}

TEST_F(GameKitTest, startNewGameSequence)
{
	mock::Game mock_new_game;

	{
		InSequence seq;

		EXPECT_CALL(mock_game, startCalled).Times(1);
		EXPECT_CALL(mock_game, stopCalled).Times(1);
		EXPECT_CALL(mock_new_game, startCalled).Times(1);
	}

	EXPECT_FALSE(mock_game.isRunning());
	EXPECT_FALSE(mock_new_game.isRunning());

	gamekit.start(&mock_game);

	EXPECT_TRUE(mock_game.isRunning());
	EXPECT_FALSE(mock_new_game.isRunning());

	gamekit.start(&mock_new_game);

	EXPECT_FALSE(mock_game.isRunning());
	EXPECT_TRUE(mock_new_game.isRunning());
}
