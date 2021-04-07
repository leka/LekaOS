# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(LKAnimationKit STATIC)

target_include_directories(LKAnimationKit
  	PUBLIC
		include
)

target_sources(LKAnimationKit
  	PRIVATE
    	source/LKAnimationKit.cpp
)

target_link_libraries(LKAnimationKit mbed-os)
target_link_libraries(LKAnimationKit LKCoreLL)
target_link_libraries(LKAnimationKit LKCoreSTM32Hal)
target_link_libraries(LKAnimationKit LKCoreVideo)


if (${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")

	leka_unit_tests_sources(
		tests/LKAnimationKit_test.cpp
	)

endif()
