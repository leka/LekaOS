# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_lib_kit(ConfigKit
	PUBLIC_INCLUDE_DIRS
		include

	PRIVATE_SOURCES
		source/ConfigKit.cpp

	PRIVATE_LINK_LIBRARIES
		FileManagerKit

	UNIT_TESTS_SOURCES
		tests/ConfigKit_test.cpp
)
