# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_lib_kit(BatteryKit
	PUBLIC_INCLUDE_DIRS
		include
		include/activities
		include/activities/include

	PRIVATE_SOURCES
		source/BatteryKit.cpp

	PUBLIC_LINK_LIBRARIES
		CoreEventQueue

	UNIT_TESTS_SOURCES
		tests/BatteryKit_test.cpp
)
