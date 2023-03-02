# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_lib_kit(RFIDKit
	PUBLIC_INCLUDE_DIRS
		include

	PRIVATE_SOURCES
		source/RFIDKit.cpp

	PRIVATE_LINK_LIBRARIES
		mbed_headers
		CoreRFIDReader

	UNIT_TESTS_SOURCES
		tests/ISO14443A_test.cpp
		tests/ISO14443AUtils_test.cpp
		tests/MagicCard_test.cpp
		tests/RFIDKit_test.cpp
)
