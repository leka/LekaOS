# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_lib_kit(CommandKit
	PUBLIC_INCLUDE_DIRS
		include

	PRIVATE_SOURCES
		source/CommandKit.cpp

	PUBLIC_LINK_LIBRARIES
		ContainerKit

	PRIVATE_LINK_LIBRARIES
		ReinforcerKit
		mbed_headers

	UNIT_TESTS_SOURCES
		tests/CommandKit_test_push.cpp
		tests/CommandKit_test_register.cpp
		tests/CommandKit_test.cpp
		tests/CommandRunner_test.cpp
)
