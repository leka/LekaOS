# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(lib_BLE STATIC)

target_include_directories(lib_BLE
	PUBLIC
	include
)

target_sources(lib_BLE
	PRIVATE
	include/LKBLE.h
)

target_link_libraries(lib_BLE
	mbed-os
	LogKit
	lib_PrettyPrinter
)
