# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(SerialNumberKit STATIC)

target_include_directories(SerialNumberKit
	PUBLIC
	include
)

target_sources(SerialNumberKit
	PRIVATE
	source/SerialNumberKit.cpp
	source/SerialNumberFormater.cpp
)

target_link_libraries(SerialNumberKit
	FileManagerKit
)

if(${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")
	leka_unit_tests_sources(
		tests/SerialNumberKit_test.cpp
		tests/SerialNumberFormater_test.cpp
	)
endif()
