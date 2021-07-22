# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(CoreAudio STATIC)

target_include_directories(CoreAudio
	PUBLIC
		include
		include/internal
)

target_sources(CoreAudio
	PRIVATE
		source/CoreAudio.cpp
		source/CoreDACTimer.cpp
		source/CoreDAC.cpp
		source/WavFile.cpp
		source/WavReader.cpp
)

target_link_libraries(CoreAudio
		mbed-os
		LKCoreSTM32Hal
		LKCoreFatFs
)

if (${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")

	leka_unit_tests_sources(
		tests/CoreDACTimer_test.cpp
		tests/CoreDAC_test.cpp
		#tests/CoreAudio_test.cpp
		# TODO : handle inclusion of last test files when functionnal
	)

endif()
