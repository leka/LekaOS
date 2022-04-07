// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "WebKit.h"
#include "gtest/gtest.h"
#include "mocks/leka/File.h"
#include "mocks/leka/Network.h"

using namespace leka;

using ::testing::_;
using ::testing::DoAll;
using ::testing::InSequence;
using ::testing::Matcher;
using ::testing::Return;
using ::testing::StrEq;

class WebKitUpdateCertificateTest : public testing::Test
{
  protected:
	// void SetUp() override {}
	// void TearDown() override {}

	mock::Network network_mock;
	mock::File web_file_handler_mock;
	WebKit web_kit {network_mock, web_file_handler_mock};
};

TEST_F(WebKitUpdateCertificateTest, updateCertificateFailOpenFile)
{
	auto expected_valid_certificate = false;

	const auto *certificates_path = "certificate/path/1.txt";

	EXPECT_CALL(web_file_handler_mock, open(Matcher<const char *>(StrEq(certificates_path)), StrEq("r")))
		.WillOnce(Return(false));
	EXPECT_CALL(web_file_handler_mock, size).Times(0);
	EXPECT_CALL(web_file_handler_mock, read(Matcher<char *>(_), _)).Times(0);
	EXPECT_CALL(web_file_handler_mock, close).Times(0);

	auto actual_valid_certificate = web_kit.updateCertificate(certificates_path);

	EXPECT_EQ(actual_valid_certificate, expected_valid_certificate);
}

TEST_F(WebKitUpdateCertificateTest, updateCertificateFailEmptyCertificate)
{
	auto expected_valid_certificate = false;

	const auto *certificates_path = "certificate/path/1.txt";
	auto certificate_size		  = 123;
	std::string certificate		  = "\0";

	{
		InSequence seq;

		EXPECT_CALL(web_file_handler_mock, open(Matcher<const char *>(StrEq(certificates_path)), StrEq("r")))
			.WillOnce(Return(true));
		EXPECT_CALL(web_file_handler_mock, size).WillOnce(Return(certificate_size));
		EXPECT_CALL(web_file_handler_mock, read(Matcher<char *>(_), certificate_size))
			.WillOnce(DoAll(::testing::SetArrayArgument<0>(certificate.begin(), certificate.end()),
							Return(std::size(certificate))));
		EXPECT_CALL(web_file_handler_mock, close).Times(1);
	}

	auto actual_valid_certificate = web_kit.updateCertificate(certificates_path);

	EXPECT_EQ(actual_valid_certificate, expected_valid_certificate);
}

TEST_F(WebKitUpdateCertificateTest, updateCertificate)
{
	auto expected_valid_certificate = true;

	const auto *certificates_path = "certificate/path/1.txt";
	auto certificate_size		  = 123;
	std::string certificate		  = "-----BEGIN-----";

	{
		InSequence seq;

		EXPECT_CALL(web_file_handler_mock, open(Matcher<const char *>(StrEq(certificates_path)), StrEq("r")))
			.WillOnce(Return(true));
		EXPECT_CALL(web_file_handler_mock, size).WillOnce(Return(certificate_size));
		EXPECT_CALL(web_file_handler_mock, read(Matcher<char *>(_), certificate_size))
			.WillOnce(DoAll(::testing::SetArrayArgument<0>(certificate.begin(), certificate.end()),
							Return(std::size(certificate))));
		EXPECT_CALL(web_file_handler_mock, close).Times(1);
	}

	auto actual_valid_certificate = web_kit.updateCertificate(certificates_path);

	EXPECT_EQ(actual_valid_certificate, expected_valid_certificate);
}
