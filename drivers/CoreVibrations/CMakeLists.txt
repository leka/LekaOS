# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(CoreVibration STATIC)

target_include_directories(CoreVibration
	PUBLIC
		include
		include/internal
)

target_sources(CoreVibration
	PRIVATE
		source/VibrationEnvelope.cpp
		source/VibrationTemplate.cpp
		source/CoreVibration.cpp
)

target_link_libraries(CoreVibration
		mbed-os
		LKCoreSTM32Hal
		CoreAudio
)

# if (${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")
# 	leka_unit_tests_sources(

# 	)

# endif()