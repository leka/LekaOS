# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(driver_FastLED STATIC)

target_include_directories(driver_FastLED
  PUBLIC
  	${CMAKE_CURRENT_SOURCE_DIR}/include
	${CMAKE_CURRENT_SOURCE_DIR}/include/internal
	${CMAKE_CURRENT_SOURCE_DIR}/include/internal/lib8tion
)

target_sources(driver_FastLED
  PRIVATE
  	source/FastLED.cpp
	source/bitswap.cpp
	source/colorutils.cpp
	source/colorpalettes.cpp
	source/hsv2rgb.cpp
	source/lib8tion.cpp
)

target_link_libraries(driver_FastLED mbed-os)
