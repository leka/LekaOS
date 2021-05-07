# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

#
# Mark: - Utils
#

add_library(Utils STATIC)

target_include_directories(Utils
  	PUBLIC
		include
)

target_sources(Utils
	PRIVATE
		source/MathUtils.cpp
		source/Utils.cpp
)

#
# Mark: - CircularBuffer
#

add_library(CircularBuffer STATIC)


target_include_directories(CircularBuffer
  	PUBLIC
		include
)

target_sources(CircularBuffer
	PRIVATE
		source/CircularBuffer.cpp
)

target_link_libraries(CircularBuffer
	PRIVATE
		mbed-os
)

#
# Mark: - LekaOSUnitTests
#

if (${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")

	leka_unit_tests_sources(
		tests/MathUtils_test_map.cpp
		tests/MathUtils_test_linear_function.cpp
		tests/CircularBuffer_test.cpp
	)

endif()
