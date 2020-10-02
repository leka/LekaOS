# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(lib_Led STATIC)

target_include_directories(lib_Led
  PUBLIC
  	${CMAKE_CURRENT_SOURCE_DIR}/include
	${CMAKE_CURRENT_SOURCE_DIR}/include/internal
	${CMAKE_CURRENT_SOURCE_DIR}/include/internal/lib8tion
)

target_sources(lib_Led
  PRIVATE
  	source/FastLED.cpp
	source/bitswap.cpp
	source/colorutils.cpp
	source/colorpalettes.cpp
	source/hsv2rgb.cpp
	source/lib8tion.cpp
)

target_link_libraries(lib_Led mbed-os)
