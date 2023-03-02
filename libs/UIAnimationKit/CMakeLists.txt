# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_lib_kit(UIAnimationKit
	PUBLIC_INCLUDE_DIRS
		include
		include/internal

	PRIVATE_SOURCES
		source/BouncingSquare.cpp
		source/UIAnimationKit.cpp

	PRIVATE_LINK_LIBRARIES
		CoreLL
		CoreSTM32Hal
		CoreVideo

	UNIT_TESTS_SOURCES
		tests/UIAnimationKit_test.cpp
)
