# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(external)

add_lib_kit(IMUKit
	PUBLIC_INCLUDE_DIRS
		include

	PRIVATE_SOURCES
		source/IMUKit.cpp

	PRIVATE_LINK_LIBRARIES
		CoreIMU
		Fusion

	UNIT_TESTS_SOURCES
		tests/IMUKit_test.cpp
)
