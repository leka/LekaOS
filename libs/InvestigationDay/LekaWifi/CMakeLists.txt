# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(lib_LekaWifi STATIC)

target_include_directories(lib_LekaWifi
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_sources(lib_LekaWifi
	PRIVATE
	source/LekaWifi.cpp
)

target_link_libraries(lib_LekaWifi mbed-os)
