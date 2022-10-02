# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(lib_LekaBluetooth STATIC)

target_include_directories(lib_LekaBluetooth
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${CMAKE_CURRENT_SOURCE_DIR}/include/internal
)

target_sources(lib_LekaBluetooth
	PRIVATE
	source/BM64Converter.cpp
	source/LekaBluetooth.cpp
)

target_link_libraries(lib_LekaBluetooth mbed-os)
