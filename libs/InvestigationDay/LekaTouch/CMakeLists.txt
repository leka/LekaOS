# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(lib_LekaTouch STATIC)

target_include_directories(lib_LekaTouch
	PUBLIC
	include
	include/internal
)

target_sources(lib_LekaTouch
	PRIVATE
	source/MCP23017.cpp
	source/LekaTouch.cpp
)

target_link_libraries(lib_LekaTouch
	mbed-os
	LogKit
)
