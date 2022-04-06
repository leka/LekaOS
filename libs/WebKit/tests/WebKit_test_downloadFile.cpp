// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "WebKit.h"
#include "gtest/gtest.h"
#include "mocks/leka/File.h"
#include "mocks/leka/Network.h"

using namespace leka;

using ::testing::_;
using ::testing::AnyNumber;
using ::testing::DoAll;
using ::testing::InSequence;
using ::testing::Matcher;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::StrEq;

class WebKitDownloadFileTest : public testing::Test
{
  protected:
	// void SetUp() override {}
	// void TearDown() override {}

	mock::Network network_mock;
	mock::File web_file_handler_mock;
	WebKit web_kit {network_mock, web_file_handler_mock};
};

ACTION_P(GetCallback, pointer)
{
	*pointer = std::function<void(const char *at, uint32_t length)>(std::get<2>(args));
}

TEST_F(WebKitDownloadFileTest, downloadFileUnsetCertificatesStore)
{
	auto expected_file_downloaded = false;

	auto downloadable_file =
		WebKit::DownloadableFile {.url = "https://url/to/download/file.bin", .to_path = "path/to/downloaded/file.bin"};

	EXPECT_CALL(web_file_handler_mock, open(Matcher<const char *>(_), _)).WillRepeatedly(Return(false));

	auto actual_file_downloaded = web_kit.downloadFile(downloadable_file);

	EXPECT_EQ(actual_file_downloaded, expected_file_downloaded);
}

TEST_F(WebKitDownloadFileTest, downloadFileCannotOpenFile)
{
	auto expected_file_downloaded = false;

	auto downloadable_file =
		WebKit::DownloadableFile {.url = "https://url/to/download/file.bin", .to_path = "path/to/downloaded/file.bin"};
	auto certificates_path_list = std::to_array<const char *>({"certificate/path/1.txt", "certificate/path/2.txt"});
	std::string certificate		= "-----BEGIN-----";

	web_kit.setCertificateStore(certificates_path_list);

	EXPECT_CALL(web_file_handler_mock, open(Matcher<const char *>(_), StrEq("r"))).WillRepeatedly(Return(true));
	EXPECT_CALL(web_file_handler_mock, size).Times(AnyNumber());
	EXPECT_CALL(web_file_handler_mock, read(Matcher<char *>(_), _))
		.WillRepeatedly(DoAll(::testing::SetArrayArgument<0>(certificate.begin(), certificate.end()),
							  Return(std::size(certificate))));
	EXPECT_CALL(web_file_handler_mock, close).Times(AnyNumber());

	EXPECT_CALL(web_file_handler_mock, open(Matcher<const char *>(StrEq(downloadable_file.to_path)), StrEq("w")))
		.WillOnce(Return(false));

	auto actual_file_downloaded = web_kit.downloadFile(downloadable_file);

	EXPECT_EQ(actual_file_downloaded, expected_file_downloaded);
}

TEST_F(WebKitDownloadFileTest, downloadFile)
{
	auto expected_file_downloaded = true;

	auto downloadable_file =
		WebKit::DownloadableFile {.url = "https://url/to/download/file.bin", .to_path = "path/to/downloaded/file.bin"};
	auto certificates_path_list = std::to_array<const char *>({"certificate/path/1.txt", "certificate/path/2.txt"});
	std::string certificate		= "-----BEGIN-----";

	HttpResponse dummy_response_with_redirection;
	HttpResponse dummy_response_final_file;
	std::string some_field		   = "Field";
	std::string redirection_field  = "Location";
	std::string redirection_result = "new_url";
	dummy_response_final_file.headers_fields.push_back(&some_field);
	dummy_response_with_redirection.headers_fields.push_back(&redirection_field);
	dummy_response_with_redirection.headers_values.push_back(&redirection_result);

	std::function<void(const char *at, uint32_t length)> save_to_file_function;

	web_kit.setCertificateStore(certificates_path_list);

	EXPECT_CALL(web_file_handler_mock, open(Matcher<const char *>(_), StrEq("r"))).WillRepeatedly(Return(true));
	EXPECT_CALL(web_file_handler_mock, size).Times(AnyNumber());
	EXPECT_CALL(web_file_handler_mock, read(Matcher<char *>(_), _))
		.WillRepeatedly(DoAll(::testing::SetArrayArgument<0>(certificate.begin(), certificate.end()),
							  Return(std::size(certificate))));
	EXPECT_CALL(web_file_handler_mock, close).Times(AnyNumber());

	EXPECT_CALL(web_file_handler_mock, open(Matcher<const char *>(StrEq(downloadable_file.to_path)), StrEq("w")))
		.WillRepeatedly(Return(true));

	{
		InSequence seq;

		EXPECT_CALL(network_mock, sendRequest(_, StrEq(downloadable_file.url), _))
			.WillOnce(DoAll(GetCallback(&save_to_file_function), ReturnRef(dummy_response_with_redirection)));
		EXPECT_CALL(network_mock, sendRequest(_, StrEq(redirection_result), _))
			.WillOnce(ReturnRef(dummy_response_final_file));
	}

	auto actual_file_downloaded = web_kit.downloadFile(downloadable_file);

	auto data_to_write = "data";
	EXPECT_CALL(web_file_handler_mock, write(data_to_write, 4)).Times(1);
	save_to_file_function(data_to_write, 4);

	EXPECT_EQ(actual_file_downloaded, expected_file_downloaded);
}
