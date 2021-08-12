# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(CoreAudio STATIC)

target_include_directories(CoreAudio
	PUBLIC
		include
		include/internal
)

target_sources(CoreAudio
	PRIVATE
		source/DACTimer.cpp
		source/DACDriver.cpp
)

target_link_libraries(CoreAudio
		mbed-os
		LKCoreSTM32Hal
)

if (${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")

	leka_unit_tests_sources(
		tests/CoreDACTimer_test.cpp
		tests/CoreDAC_test.cpp
	)

endif()
