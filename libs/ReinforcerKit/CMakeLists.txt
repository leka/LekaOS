# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_lib_kit(ReinforcerKit
	PUBLIC_INCLUDE_DIRS
		include

	PRIVATE_SOURCES
		source/ReinforcerKit.cpp

	PUBLIC_LINK_LIBRARIES
		MotionKit

	PRIVATE_LINK_LIBRARIES
		LedKit

	UNIT_TESTS_SOURCES
		tests/ReinforcerKit_test.cpp
)
