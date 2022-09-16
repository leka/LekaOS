// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "BLEServiceUpdate.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace leka;

using ::testing::MockFunction;

class BLEServiceUpdateTest : public testing::Test
{
  protected:
	// void SetUp() override {}
	// void TearDown() override {}

	BLEServiceUpdate service_update {};

	BLEServiceUpdate::data_received_handle_t data_received_handle {};
	BLEServiceUpdate::data_requested_handle_t data_requested_handle {};

	bool default_request_update_sent {false};
	FirmwareVersion default_version {0x00, 0x00, 0x0000};

	void onDataReceivedProcess(const uint8_t *data)
	{
		data_received_handle.data = data;

		service_update.onDataReceived(data_received_handle);
	}
};

TEST_F(BLEServiceUpdateTest, initialisation)
{
	EXPECT_NE(&service_update, nullptr);
}

TEST_F(BLEServiceUpdateTest, onRequestUpdateReceivedTrue)
{
	bool request_update_sent	  = true;
	auto request_update_sent_data = std::make_shared<uint8_t>(request_update_sent);

	testing::MockFunction<void()> mock_callback;
	service_update.onUpdateRequested(mock_callback.AsStdFunction());

	EXPECT_CALL(mock_callback, Call).Times(1);

	onDataReceivedProcess(request_update_sent_data.get());
}

TEST_F(BLEServiceUpdateTest, onRequestUpdateReceivedFalse)
{
	bool request_update_sent	  = false;
	auto request_update_sent_data = std::make_shared<uint8_t>(request_update_sent);

	testing::MockFunction<void()> mock_callback;
	service_update.onUpdateRequested(mock_callback.AsStdFunction());

	EXPECT_CALL(mock_callback, Call).Times(0);

	onDataReceivedProcess(request_update_sent_data.get());
}

TEST_F(BLEServiceUpdateTest, onRequestUpdateReceivedNotSameHandle)
{
	bool request_update_sent = default_request_update_sent;
	bool sent_value			 = true;
	auto sent_value_data	 = std::make_shared<uint8_t>(sent_value);

	data_received_handle.handle = 0xFFFF;

	testing::MockFunction<void()> mock_callback;
	service_update.onUpdateRequested(mock_callback.AsStdFunction());

	EXPECT_CALL(mock_callback, Call).Times(0);

	onDataReceivedProcess(sent_value_data.get());
}

TEST_F(BLEServiceUpdateTest, onFactoryResetNotificationReceivedTrue)
{
	bool request_factory_reset_sent		 = true;
	auto request_factory_reset_sent_data = std::make_shared<uint8_t>(request_factory_reset_sent);

	testing::MockFunction<void(bool)> mock_callback;
	service_update.onFactoryResetNotification(mock_callback.AsStdFunction());

	EXPECT_CALL(mock_callback, Call(request_factory_reset_sent)).Times(1);

	onDataReceivedProcess(request_factory_reset_sent_data.get());
}

TEST_F(BLEServiceUpdateTest, onFactoryResetNotificationReceivedFalse)
{
	bool request_factory_reset_sent		 = false;
	auto request_factory_reset_sent_data = std::make_shared<uint8_t>(request_factory_reset_sent);

	testing::MockFunction<void(bool)> mock_callback;
	service_update.onFactoryResetNotification(mock_callback.AsStdFunction());

	EXPECT_CALL(mock_callback, Call(request_factory_reset_sent)).Times(1);

	onDataReceivedProcess(request_factory_reset_sent_data.get());
}

TEST_F(BLEServiceUpdateTest, onFactoryResetNotificationReceivedNotSameHandle)
{
	bool sent_value		 = true;
	auto sent_value_data = std::make_shared<uint8_t>(sent_value);

	data_received_handle.handle = 0xFFFF;

	testing::MockFunction<void(bool)> mock_callback;
	service_update.onFactoryResetNotification(mock_callback.AsStdFunction());

	EXPECT_CALL(mock_callback, Call).Times(0);

	onDataReceivedProcess(sent_value_data.get());
}

TEST_F(BLEServiceUpdateTest, getVersionMajorDefault)
{
	auto actual_version = service_update.getVersion();
	EXPECT_EQ(actual_version.major, default_version.major);
}

TEST_F(BLEServiceUpdateTest, getVersionMajorAnyValue)
{
	auto expected_version_major {0x2A};
	auto expected_version_major_data = std::make_shared<uint8_t>(expected_version_major);

	onDataReceivedProcess(expected_version_major_data.get());

	auto actual_version = service_update.getVersion();
	EXPECT_EQ(actual_version.major, expected_version_major);
}

TEST_F(BLEServiceUpdateTest, getVersionMajorNotSameHandle)
{
	auto expected_version_major = default_version.major;
	auto sent_value {0x2B};
	auto sent_value_data = std::make_shared<uint8_t>(sent_value);

	data_received_handle.handle = 0xFFFF;

	onDataReceivedProcess(sent_value_data.get());

	auto actual_version = service_update.getVersion();
	EXPECT_EQ(actual_version.major, expected_version_major);
	EXPECT_NE(actual_version.major, sent_value);
}

TEST_F(BLEServiceUpdateTest, getVersionMinorDefault)
{
	auto actual_version = service_update.getVersion();
	EXPECT_EQ(actual_version.minor, default_version.minor);
}

TEST_F(BLEServiceUpdateTest, getVersionMinorAnyValue)
{
	auto expected_version_minor {0x2C};
	auto expected_version_minor_data = std::make_shared<uint8_t>(expected_version_minor);

	onDataReceivedProcess(expected_version_minor_data.get());

	auto actual_version = service_update.getVersion();
	EXPECT_EQ(actual_version.minor, expected_version_minor);
}

TEST_F(BLEServiceUpdateTest, getVersionMinorNotSameHandle)
{
	auto expected_version_minor = default_version.minor;
	auto sent_value {0x2D};
	auto sent_value_data = std::make_shared<uint8_t>(sent_value);

	data_received_handle.handle = 0xFFFF;

	onDataReceivedProcess(sent_value_data.get());

	auto actual_version = service_update.getVersion();
	EXPECT_EQ(actual_version.minor, expected_version_minor);
	EXPECT_NE(actual_version.minor, sent_value);
}

TEST_F(BLEServiceUpdateTest, getVersionRevisionDefault)
{
	auto actual_version = service_update.getVersion();
	EXPECT_EQ(actual_version.revision, default_version.revision);
}

TEST_F(BLEServiceUpdateTest, getVersionRevisionAnyValue)
{
	auto expected_version_revision {0x2F2E};

	auto convert_uint16_t_to_big_endian = [](uint16_t value) {
		return std::array<uint8_t, 2> {
			static_cast<uint8_t>((value >> 8)),
			static_cast<uint8_t>(value & 0xFF),
		};
	};

	auto sent_value = convert_uint16_t_to_big_endian(expected_version_revision);

	EXPECT_EQ(static_cast<uint16_t>((static_cast<uint16_t>(sent_value[1]) << 8) | sent_value[0]), 0x2E2F);

	onDataReceivedProcess(sent_value.data());

	auto actual_version = service_update.getVersion();
	EXPECT_EQ(actual_version.revision, expected_version_revision);
}

TEST_F(BLEServiceUpdateTest, getVersionRevisionNotSameHandle)
{
	auto expected_version_revision = default_version.revision;

	auto convert_uint16_t_to_big_endian = [](uint16_t value) {
		return static_cast<uint16_t>((value & 0x00FF) << 8 | (value & 0xFF00) >> 8);
	};

	auto sent_value		 = convert_uint16_t_to_big_endian(0x3031);
	auto sent_value_data = std::make_shared<uint8_t>(sent_value);

	EXPECT_EQ(sent_value, 0x3130);

	data_received_handle.handle = 0xFFFF;

	onDataReceivedProcess(sent_value_data.get());

	auto actual_version = service_update.getVersion();
	EXPECT_EQ(actual_version.revision, expected_version_revision);
	EXPECT_NE(actual_version.revision, sent_value);
}

TEST_F(BLEServiceUpdateTest, onDataRequested)
{
	service_update.onDataRequested(data_requested_handle);

	// nothing expected
}
