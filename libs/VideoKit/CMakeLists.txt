# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(VideoKit STATIC)

target_include_directories(VideoKit
	PUBLIC
		include
)

target_sources(VideoKit
	PRIVATE
		source/Font.cpp
		source/Graphics.cpp
		source/VideoKit.cpp
)

target_link_libraries(VideoKit CoreVideo LKCoreSTM32Hal LKCoreFatFs)

if (${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")

	# leka_unit_tests_sources(
	# 	tests/LKCalculatorKit_test.cpp
	# )

endif()
