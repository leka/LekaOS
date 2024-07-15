# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_lib_kit(WebKit
	PUBLIC_INCLUDE_DIRS
		include

	PRIVATE_SOURCES
		source/WebKit.cpp

	PRIVATE_LINK_LIBRARIES
		mbed_headers
		FileManagerKit

	UNIT_TESTS_SOURCES
		tests/WebKit_test_downloadFile.cpp
		tests/WebKit_test_redirectionUrl.cpp
		tests/WebKit_test_updateCertificate.cpp
		tests/WebKit_test.cpp
)
