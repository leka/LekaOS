# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_lib_kit(BehaviorKit
	PUBLIC_INCLUDE_DIRS
		include

	PRIVATE_SOURCES
		source/BehaviorKit.cpp

	PRIVATE_LINK_LIBRARIES
		mbed_headers
		LedKit

	UNIT_TESTS_SOURCES
		tests/BehaviorKit_test.cpp
)
