# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(lib_LekaMotors STATIC)

target_include_directories(lib_LekaMotors
	PUBLIC
		include
)

target_sources(lib_LekaMotors
	PRIVATE
		source/LekaMotors.cpp
)

target_link_libraries(lib_LekaMotors
	mbed-os
	CoreMotor
)
