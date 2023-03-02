# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_lib_core(CoreFlashMemory
	PUBLIC_INCLUDE_DIRS
		include

	PRIVATE_SOURCES
		source/CoreFlashIS25LP016D.cpp
		source/CoreFlashManagerIS25LP016D.cpp

	PRIVATE_LINK_LIBRARIES
		mbed_headers
		CoreQSPI

	UNIT_TESTS_SOURCES
		tests/CoreFlashIS25LP016D_test.cpp
		tests/CoreFlashManagerIS25LP016D_test.cpp
)
