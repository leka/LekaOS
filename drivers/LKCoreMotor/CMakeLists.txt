# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(LKCoreMotor STATIC)

target_include_directories(LKCoreMotor
	PUBLIC
		include
)

target_sources(LKCoreMotor
	PRIVATE
		source/LKCoreMotor.cpp
)

target_link_libraries(LKCoreMotor mbed-os)
