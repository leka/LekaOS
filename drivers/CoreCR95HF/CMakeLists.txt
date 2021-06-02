# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(CoreCR95HF STATIC)

target_include_directories(CoreCR95HF
	PUBLIC
		include
)

target_sources(CoreCR95HF
	PRIVATE
		source/CoreCR95HF.cpp
)

target_link_libraries(CoreCR95HF
		mbed-os
		CoreBufferedSerial
		interface
)

if (${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")

	leka_unit_tests_sources(
		tests/CoreCR95HF_test.cpp
	)

endif()
