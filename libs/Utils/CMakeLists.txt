# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(Utils STATIC)

target_include_directories(Utils
	PUBLIC
	include
)

target_sources(Utils
	PRIVATE
	source/Utils.cpp
	source/CastUtils.cpp
	source/MathUtils.cpp
	source/MemoryUtils.cpp
)

if(${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")
	leka_unit_tests_sources(
		tests/CastUtils_test.cpp
		tests/MathUtils_test_checksum.cpp
		tests/MathUtils_test_invSqrt.cpp
		tests/MathUtils_test_linear_function.cpp
		tests/MathUtils_test_map.cpp
		tests/MathUtils_test_random8.cpp
		tests/MemoryUtils_test.cpp
		tests/RangesUtils_test.cpp
	)
endif()
