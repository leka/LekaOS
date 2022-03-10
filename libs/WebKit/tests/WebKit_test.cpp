// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "WebKit.h"

#include "gtest/gtest.h"
#include "mocks/leka/File.h"
#include "mocks/leka/Network.h"

using namespace leka;

class WebKitTest : public testing::Test
{
  protected:
	// void SetUp() override {}
	// void TearDown() override {}

	mock::Network network_mock;
	mock::File web_file_handler_mock;
	WebKit web_kit {network_mock, web_file_handler_mock};
};

TEST_F(WebKitTest, initialization)
{
	EXPECT_NE(&web_kit, nullptr);
}

TEST_F(WebKitTest, connect)
{
	auto ssid = "USER_SSID";
	auto pass = "USER_PASS";

	EXPECT_CALL(network_mock, connect(ssid, pass)).Times(1);

	web_kit.connect(ssid, pass);
}

TEST_F(WebKitTest, setCertificateStore)
{
	auto certificates_path_list = std::to_array({"certificate/path/1.txt", "certificate/path/2.txt"});

	// TODO: Add an expectation

	web_kit.setCertificateStore(certificates_path_list);
}
