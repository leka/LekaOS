# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_lib_kit(LogKit
	PUBLIC_INCLUDE_DIRS
		include

	PRIVATE_SOURCES
		source/LogKit.cpp

	PUBLIC_LINK_LIBRARIES
	mbed_headers
		ContainerKit

	PRIVATE_LINK_LIBRARIES
		ContainerKit

	UNIT_TESTS_SOURCES
		tests/LogKit_test_log.cpp
		tests/LogKit_test_fifo.cpp
		tests/LogKit_test_format.cpp
		tests/LogKit_test_now.cpp
		tests/LogKit_test_sink.cpp
)
