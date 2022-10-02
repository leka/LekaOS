// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "WebKit.h"
#include "gtest/gtest.h"
#include "mocks/leka/File.h"
#include "mocks/leka/Network.h"

using namespace leka;

using ::testing::Return;

class WebKitRedirectionUrlTest : public testing::Test
{
  protected:
	// void SetUp() override {}
	// void TearDown() override {}

	mock::Network network_mock;
	mock::File web_file_handler_mock;
	WebKit web_kit {network_mock, web_file_handler_mock};
};

TEST_F(WebKitRedirectionUrlTest, responseHasRedirectionUrlNoField)
{
	auto expected_result = false;

	auto dummy_response = HttpResponse {};

	auto actual_result = web_kit.responseHasRedirectionUrl(dummy_response);

	EXPECT_EQ(actual_result, expected_result);
}

TEST_F(WebKitRedirectionUrlTest, responseHasRedirectionUrlMissingField)
{
	auto expected_result = false;

	auto dummy_response = HttpResponse {};

	std::string another_field = "Another field";
	dummy_response.headers_fields.push_back(&another_field);

	auto actual_result = web_kit.responseHasRedirectionUrl(dummy_response);

	EXPECT_EQ(actual_result, expected_result);
}

TEST_F(WebKitRedirectionUrlTest, responseHasRedirectionUrl)
{
	auto expected_result = true;

	auto dummy_response = HttpResponse {};

	std::string expected_field = "Location";
	dummy_response.headers_fields.push_back(&expected_field);

	auto actual_result = web_kit.responseHasRedirectionUrl(dummy_response);

	EXPECT_EQ(actual_result, expected_result);
}

TEST_F(WebKitRedirectionUrlTest, getRedirectionUrl)
{
	std::string expected_result = "expected_result";

	auto dummy_response = HttpResponse {};

	std::string impostor_field = "Impostor";
	std::string expected_field = "Location";
	dummy_response.headers_fields.push_back(&impostor_field);
	dummy_response.headers_fields.push_back(&expected_field);

	std::string impostor_result = "impostor_result";
	dummy_response.headers_values.push_back(&impostor_result);
	dummy_response.headers_values.push_back(&expected_result);

	web_kit.getRedirectionUrl(dummy_response);
	auto actual_result = web_kit.getUrl();

	EXPECT_EQ(actual_result, expected_result);
	EXPECT_NE(actual_result, impostor_result);
}
