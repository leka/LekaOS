# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_lib_core(CoreLED
	PUBLIC_INCLUDE_DIRS
		include

	PRIVATE_SOURCES
		source/CoreLED.cpp

	PUBLIC_LINK_LIBRARIES

	# CoreSPI
		ColorKit

	UNIT_TESTS_SOURCES
		tests/CoreLED_test_brightness.cpp
		tests/CoreLED_test_setColor.cpp
		tests/CoreLED_test_showHideColor.cpp
		tests/CoreLED_test_status.cpp
		tests/CoreLED_test.cpp
)
