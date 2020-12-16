# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_test(NAME LekaOSUnitTests COMMAND LekaOSUnitTests)

add_executable(LekaOSUnitTests)

add_subdirectory(./utest)

target_sources(LekaOSUnitTests
	PRIVATE
		main.cpp
)

target_link_libraries(LekaOSUnitTests
	PRIVATE
		utest_h
		mbed-os
)

add_subdirectory(./Examples)
