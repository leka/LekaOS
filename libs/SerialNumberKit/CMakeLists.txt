# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_lib_kit(SerialNumberKit
	PUBLIC_INCLUDE_DIRS
		include

	PRIVATE_SOURCES
		source/SerialNumberKit.cpp
		source/SerialNumberFormater.cpp

	PRIVATE_LINK_LIBRARIES
		FileManagerKit

	UNIT_TESTS_SOURCES
		tests/SerialNumberKit_test.cpp
		tests/SerialNumberFormater_test.cpp
)
