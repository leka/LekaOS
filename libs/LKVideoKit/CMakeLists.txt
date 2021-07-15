# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(LKVideoKit STATIC)

target_include_directories(LKVideoKit
		PUBLIC
		include
)

target_sources(LKVideoKit
		PRIVATE
		source/LKGraphics.cpp
		source/LKVideoKit.cpp
)

target_link_libraries(LKVideoKit CoreVideo LKCoreSTM32Hal LKCoreFatFs)

if (${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")

	# leka_unit_tests_sources(
	# 	tests/LKCalculatorKit_test.cpp
	# )

endif()
