# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_lib_core(CoreIMU
	PUBLIC_INCLUDE_DIRS
		include
		extern/include

	PRIVATE_SOURCES
		extern/source/lsm6dsox_reg.c
		source/CoreLSM6DSOX.cpp

	PUBLIC_LINK_LIBRARIES
		CoreEventQueue
		CoreInterruptIn

	UNIT_TESTS_SOURCES
		tests/CoreLSM6DSOX_test.cpp
)
