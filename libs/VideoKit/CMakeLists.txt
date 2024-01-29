# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_lib_kit(VideoKit
	PUBLIC_INCLUDE_DIRS
		include

	PRIVATE_SOURCES
		source/VideoKit.cpp

	PUBLIC_LINK_LIBRARIES
		CoreVideo
		CoreSTM32Hal
		FileManagerKit

	PRIVATE_LINK_LIBRARIES
		CoreLL

	UNIT_TESTS_SOURCES
		tests/VideoKit_test.cpp
)
