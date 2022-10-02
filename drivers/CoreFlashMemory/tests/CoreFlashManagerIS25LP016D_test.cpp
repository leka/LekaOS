// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreFlashManagerIS25LP016D.h"
#include <array>

#include "gtest/gtest.h"
#include "mocks/leka/QSPI.h"

using namespace leka;
using namespace flash::is25lp016d;

using ::testing::_;
using ::testing::InSequence;

MATCHER_P(setArray, expected_array, "")
{
	if (arg.size() < expected_array.size()) {
		return false;
	}

	for (int i = 0; i < expected_array.size(); i++) {
		arg[i] = expected_array[i];
	}
	return true;
}

class CoreFlashManagerIS25LP016DTest : public ::testing::Test
{
  protected:
	CoreFlashManagerIS25LP016DTest() : flash_manager_is25lp(qspimock) {}

	// void SetUp() override {}
	// void TearDown() override {}

	void MOCK_FUNCTION_chip_available(int times = 1)
	{
		auto work_not_in_progress = ~status::work_in_progress;
		auto returned_value		  = std::to_array({work_not_in_progress});
		EXPECT_CALL(qspimock, sendCommand(command::read_status, _, _, _, setArray(returned_value), _)).Times(times);
	}
	void MOCK_FUNCTION_chip_not_available()
	{
		auto returned_value = std::to_array({status::work_in_progress});
		EXPECT_CALL(qspimock, sendCommand(command::read_status, _, _, _, setArray(returned_value), _)).Times(1);
	}

	void MOCK_FUNCTION_wait_chip_available_and_is_available() { MOCK_FUNCTION_chip_available(2); }
	void MOCK_FUNCTION_wait_chip_available_and_is_not_available()
	{
		{
			InSequence seq;
			MOCK_FUNCTION_chip_available();
			MOCK_FUNCTION_chip_not_available();
		}
	}
	void MOCK_FUNCTION_enable_write_and_write_is_enabled()
	{
		auto returned_value = std::to_array({status::write_enable_latch});
		{
			InSequence seq;

			MOCK_FUNCTION_wait_chip_available_and_is_available();
			EXPECT_CALL(qspimock, sendCommand(command::write_enable, _, _, _, _, _)).Times(1);

			MOCK_FUNCTION_wait_chip_available_and_is_available();
			EXPECT_CALL(qspimock,
						sendCommand(command::read_status, _, _, _, setArray(returned_value), status::register_size))
				.Times(1);
		}
	}
	void MOCK_FUNCTION_enable_write_and_write_is_not_enabled()
	{
		auto write_not_enabled = ~status::write_enable_latch;
		auto returned_value	   = std::to_array({write_not_enabled});
		{
			InSequence seq;

			MOCK_FUNCTION_wait_chip_available_and_is_available();
			EXPECT_CALL(qspimock, sendCommand(command::write_enable, _, _, _, _, _)).Times(1);

			MOCK_FUNCTION_wait_chip_available_and_is_available();
			EXPECT_CALL(qspimock,
						sendCommand(command::read_status, _, _, _, setArray(returned_value), status::register_size))
				.Times(1);
		}
	}

	mock::QSPI qspimock;
	CoreFlashManagerIS25LP016D flash_manager_is25lp;
};

TEST_F(CoreFlashManagerIS25LP016DTest, instantiation)
{
	ASSERT_NE(&flash_manager_is25lp, nullptr);
}

TEST_F(CoreFlashManagerIS25LP016DTest, getStatusRegister)
{
	auto returned_value = std::to_array({0x2A});
	auto expected_value = returned_value[0];

	EXPECT_CALL(qspimock, sendCommand(command::read_status, _, _, _, setArray(returned_value), status::register_size))
		.Times(1);

	auto actual_value = flash_manager_is25lp.getStatusRegister();
	ASSERT_EQ(actual_value, expected_value);
}

TEST_F(CoreFlashManagerIS25LP016DTest, chipIsAvailable)
{
	auto expected_not_available = false;

	auto work_not_in_progress = ~status::work_in_progress;
	auto returned_value		  = std::to_array({work_not_in_progress});

	EXPECT_CALL(qspimock, sendCommand(command::read_status, _, _, _, setArray(returned_value), status::register_size))
		.Times(1);

	auto actual_not_available = flash_manager_is25lp.chipIsNotAvailable();
	ASSERT_EQ(actual_not_available, expected_not_available);
}

TEST_F(CoreFlashManagerIS25LP016DTest, chipIsNotAvailable)
{
	auto expected_not_available = true;

	auto returned_value = std::to_array({status::work_in_progress});

	EXPECT_CALL(qspimock, sendCommand(command::read_status, _, _, _, setArray(returned_value), status::register_size))
		.Times(1);

	auto actual_not_available = flash_manager_is25lp.chipIsNotAvailable();
	ASSERT_EQ(actual_not_available, expected_not_available);
}

TEST_F(CoreFlashManagerIS25LP016DTest, waitForChipAvailable)
{
	MOCK_FUNCTION_chip_available();

	flash_manager_is25lp.waitForChipAvailable();
}

TEST_F(CoreFlashManagerIS25LP016DTest, waitForChipAvailableAndIsNotAvailableFirstTime)
{
	{
		InSequence seq;

		MOCK_FUNCTION_chip_not_available();
		MOCK_FUNCTION_chip_available();
	}

	flash_manager_is25lp.waitForChipAvailable();
}

TEST_F(CoreFlashManagerIS25LP016DTest, reset)
{
	{
		InSequence seq;

		MOCK_FUNCTION_wait_chip_available_and_is_available();
		EXPECT_CALL(qspimock, sendCommand(command::reset_enable, _, _, _, _, _)).Times(1);
		MOCK_FUNCTION_wait_chip_available_and_is_available();
		EXPECT_CALL(qspimock, sendCommand(command::reset, _, _, _, _, _)).Times(1);
	}

	flash_manager_is25lp.reset();
}

TEST_F(CoreFlashManagerIS25LP016DTest, resetFailBeforeResetEnable)
{
	{
		InSequence seq;

		MOCK_FUNCTION_wait_chip_available_and_is_not_available();
		EXPECT_CALL(qspimock, sendCommand(command::reset_enable, _, _, _, _, _)).Times(0);
		EXPECT_CALL(qspimock, sendCommand(command::reset, _, _, _, _, _)).Times(0);
	}

	flash_manager_is25lp.reset();
}

TEST_F(CoreFlashManagerIS25LP016DTest, resetFailBeforeReset)
{
	{
		InSequence seq;

		MOCK_FUNCTION_wait_chip_available_and_is_available();
		EXPECT_CALL(qspimock, sendCommand(command::reset_enable, _, _, _, _, _)).Times(1);
		MOCK_FUNCTION_wait_chip_available_and_is_not_available();
		EXPECT_CALL(qspimock, sendCommand(command::reset, _, _, _, _, _)).Times(0);
	}

	flash_manager_is25lp.reset();
}

TEST_F(CoreFlashManagerIS25LP016DTest, enableWrite)
{
	{
		InSequence seq;

		MOCK_FUNCTION_wait_chip_available_and_is_available();
		EXPECT_CALL(qspimock, sendCommand(command::write_enable, _, _, _, _, _)).Times(1);
	}

	flash_manager_is25lp.enableWrite();
}

TEST_F(CoreFlashManagerIS25LP016DTest, enableWriteFail)
{
	{
		InSequence seq;

		MOCK_FUNCTION_wait_chip_available_and_is_not_available();
		EXPECT_CALL(qspimock, sendCommand(command::write_enable, _, _, _, _, _)).Times(0);
	}

	flash_manager_is25lp.enableWrite();
}

TEST_F(CoreFlashManagerIS25LP016DTest, writeIsEnabled)
{
	auto expected_not_enabled = false;

	auto returned_value = std::to_array({status::write_enable_latch});

	{
		InSequence seq;

		MOCK_FUNCTION_wait_chip_available_and_is_available();
		EXPECT_CALL(qspimock,
					sendCommand(command::read_status, _, _, _, setArray(returned_value), status::register_size))
			.Times(1);
	}

	auto actual_not_enabled = flash_manager_is25lp.writeIsNotEnabled();
	ASSERT_EQ(actual_not_enabled, expected_not_enabled);
}

TEST_F(CoreFlashManagerIS25LP016DTest, writeIsNotEnabled)
{
	auto expected_not_enabled = true;

	auto write_not_enabled = ~status::write_enable_latch;
	auto returned_value	   = std::to_array({write_not_enabled});

	{
		InSequence seq;

		MOCK_FUNCTION_wait_chip_available_and_is_available();
		EXPECT_CALL(qspimock,
					sendCommand(command::read_status, _, _, _, setArray(returned_value), status::register_size))
			.Times(1);
	}

	auto actual_not_enabled = flash_manager_is25lp.writeIsNotEnabled();
	ASSERT_EQ(actual_not_enabled, expected_not_enabled);
}

TEST_F(CoreFlashManagerIS25LP016DTest, writeIsNotEnabledDueToChipNotAvailable)
{
	auto expected_not_enabled = true;

	auto returned_value = std::to_array({status::write_enable_latch});

	{
		InSequence seq;

		MOCK_FUNCTION_wait_chip_available_and_is_not_available();
		EXPECT_CALL(qspimock,
					sendCommand(command::read_status, _, _, _, setArray(returned_value), status::register_size))
			.Times(0);
	}

	auto actual_not_enabled = flash_manager_is25lp.writeIsNotEnabled();
	ASSERT_EQ(actual_not_enabled, expected_not_enabled);
}

TEST_F(CoreFlashManagerIS25LP016DTest, erase)
{
	{
		InSequence seq;

		MOCK_FUNCTION_enable_write_and_write_is_enabled();
		MOCK_FUNCTION_wait_chip_available_and_is_available();
		EXPECT_CALL(qspimock, sendCommand(command::erase_chip, _, _, _, _, _)).Times(1);
	}

	flash_manager_is25lp.erase();
}

TEST_F(CoreFlashManagerIS25LP016DTest, eraseFailNotEnableWrite)
{
	{
		InSequence seq;

		MOCK_FUNCTION_enable_write_and_write_is_not_enabled();
		EXPECT_CALL(qspimock, sendCommand(command::erase_chip, _, _, _, _, _)).Times(0);
	}

	flash_manager_is25lp.erase();
}

TEST_F(CoreFlashManagerIS25LP016DTest, eraseFailChipNotAvailable)
{
	{
		InSequence seq;

		MOCK_FUNCTION_enable_write_and_write_is_enabled();
		MOCK_FUNCTION_wait_chip_available_and_is_not_available();
		EXPECT_CALL(qspimock, sendCommand(command::erase_chip, _, _, _, _, _)).Times(0);
	}

	flash_manager_is25lp.erase();
}
