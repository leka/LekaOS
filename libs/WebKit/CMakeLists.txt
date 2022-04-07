# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(WebKit STATIC)

target_include_directories(WebKit
	PUBLIC
		include
)

target_sources(WebKit
	PRIVATE
		source/WebKit.cpp
)

target_link_libraries(WebKit
	mbed-os
	FileManagerKit
)

if (${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")

	leka_unit_tests_sources(
		tests/WebKit_test_updateCertificate.cpp
		tests/WebKit_test_redirectionUrl.cpp
		tests/WebKit_test_downloadFile.cpp
		tests/WebKit_test.cpp
	)

endif()
