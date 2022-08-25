// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "GameKit.h"

#include "gtest/gtest.h"
#include "mocks/Game.h"

using namespace leka;

using ::testing::InSequence;

class GameKitTest : public ::testing::Test
{
  protected:
	GameKitTest() = default;

	// void SetUp() override {}
	// void TearDown() override {}

	GameKit gamekit;

	mock::Game mock_game {};
};

TEST_F(GameKitTest, initialization)
{
	EXPECT_NE(&mock_game, nullptr);
}

TEST_F(GameKitTest, startGame)
{
	EXPECT_CALL(mock_game, start).Times(1);

	gamekit.start(&mock_game);
}

TEST_F(GameKitTest, startNullPtr)
{
	EXPECT_CALL(mock_game, start).Times(0);

	gamekit.start(nullptr);
}

TEST_F(GameKitTest, stopWithoutGame)
{
	EXPECT_CALL(mock_game, stop).Times(0);

	gamekit.stop();
}

TEST_F(GameKitTest, stopStartedGame)
{
	EXPECT_CALL(mock_game, start).Times(1);
	EXPECT_CALL(mock_game, stop).Times(1);

	gamekit.start(&mock_game);
	gamekit.stop();
}

TEST_F(GameKitTest, startNewGameSequence)
{
	mock::Game mock_new_game;

	{
		InSequence seq;

		EXPECT_CALL(mock_game, start).Times(1);
		EXPECT_CALL(mock_game, stop).Times(1);
		EXPECT_CALL(mock_new_game, start).Times(1);
	}

	gamekit.start(&mock_game);

	gamekit.start(&mock_new_game);
}
