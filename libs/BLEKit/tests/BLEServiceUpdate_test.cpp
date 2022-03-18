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
	bool request_update_sent = true;

	testing::MockFunction<void()> mock_callback;
	service_update.onUpdateRequested(mock_callback.AsStdFunction());

	EXPECT_CALL(mock_callback, Call).Times(1);

	auto convert_to_handle_data_type = [](bool value) { return std::make_shared<uint8_t>(value).get(); };
	onDataReceivedProcess(convert_to_handle_data_type(request_update_sent));
}

TEST_F(BLEServiceUpdateTest, onRequestUpdateReceivedFalse)
{
	bool request_update_sent = false;

	testing::MockFunction<void()> mock_callback;
	service_update.onUpdateRequested(mock_callback.AsStdFunction());

	EXPECT_CALL(mock_callback, Call).Times(0);

	auto convert_to_handle_data_type = [](bool value) { return std::make_shared<uint8_t>(value).get(); };
	onDataReceivedProcess(convert_to_handle_data_type(request_update_sent));
}

TEST_F(BLEServiceUpdateTest, onRequestUpdateReceivedNotSameHandle)
{
	bool request_update_sent = default_request_update_sent;
	bool sent_value			 = true;

	data_received_handle.handle = 0xFFFF;

	testing::MockFunction<void()> mock_callback;
	service_update.onUpdateRequested(mock_callback.AsStdFunction());

	EXPECT_CALL(mock_callback, Call).Times(0);

	auto convert_to_handle_data_type = [](bool value) { return std::make_shared<uint8_t>(value).get(); };
	onDataReceivedProcess(convert_to_handle_data_type(sent_value));
}

TEST_F(BLEServiceUpdateTest, getVersionMajorDefault)
{
	auto actual_version = service_update.getVersion();
	EXPECT_EQ(actual_version.major, default_version.major);
}

TEST_F(BLEServiceUpdateTest, getVersionMajorAnyValue)
{
	auto expected_version_major {0x2A};

	auto convert_to_handle_data_type = [](uint8_t value) { return std::make_shared<uint8_t>(value).get(); };
	onDataReceivedProcess(convert_to_handle_data_type(expected_version_major));

	auto actual_version = service_update.getVersion();
	EXPECT_EQ(actual_version.major, expected_version_major);
}

TEST_F(BLEServiceUpdateTest, getVersionMajorNotSameHandle)
{
	auto expected_version_major = default_version.major;
	auto sent_value {0x2B};

	data_received_handle.handle = 0xFFFF;

	auto convert_to_handle_data_type = [](uint8_t value) { return std::make_shared<uint8_t>(value).get(); };
	onDataReceivedProcess(convert_to_handle_data_type(sent_value));

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

	auto convert_to_handle_data_type = [](uint8_t value) { return std::make_shared<uint8_t>(value).get(); };
	onDataReceivedProcess(convert_to_handle_data_type(expected_version_minor));

	auto actual_version = service_update.getVersion();
	EXPECT_EQ(actual_version.minor, expected_version_minor);
}

TEST_F(BLEServiceUpdateTest, getVersionMinorNotSameHandle)
{
	auto expected_version_minor = default_version.minor;
	auto sent_value {0x2D};

	data_received_handle.handle = 0xFFFF;

	auto convert_to_handle_data_type = [](uint8_t value) { return std::make_shared<uint8_t>(value).get(); };
	onDataReceivedProcess(convert_to_handle_data_type(sent_value));

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
		return static_cast<uint16_t>((value & 0x00FF) << 8 | (value & 0xFF00) >> 8);
	};
	auto sent_value = convert_uint16_t_to_big_endian(expected_version_revision);
	EXPECT_EQ(sent_value, 0x2E2F);

	auto convert_to_handle_data_type = [](uint16_t *value) { return reinterpret_cast<uint8_t *>(value); };
	onDataReceivedProcess(convert_to_handle_data_type(&sent_value));

	auto actual_version = service_update.getVersion();
	EXPECT_EQ(actual_version.revision, expected_version_revision);
}

TEST_F(BLEServiceUpdateTest, getVersionRevisionNotSameHandle)
{
	auto expected_version_revision = default_version.revision;

	auto convert_uint16_t_to_big_endian = [](uint16_t value) {
		return static_cast<uint16_t>((value & 0x00FF) << 8 | (value & 0xFF00) >> 8);
	};
	auto sent_value = convert_uint16_t_to_big_endian(0x3031);
	EXPECT_EQ(sent_value, 0x3130);

	data_received_handle.handle = 0xFFFF;

	auto convert_to_handle_data_type = [](uint16_t *value) { return reinterpret_cast<uint8_t *>(value); };
	onDataReceivedProcess(convert_to_handle_data_type(&sent_value));

	auto actual_version = service_update.getVersion();
	EXPECT_EQ(actual_version.revision, expected_version_revision);
	EXPECT_NE(actual_version.revision, sent_value);
}
