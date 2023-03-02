# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_lib_kit(Utils
	PUBLIC_INCLUDE_DIRS
		include

	PRIVATE_SOURCES
		source/CastUtils.cpp
		source/MathUtils.cpp
		source/MemoryUtils.cpp
		source/Utils.cpp

	UNIT_TESTS_SOURCES
		tests/CastUtils_test.cpp
		tests/MathUtils_test_checksum.cpp
		tests/MathUtils_test_invSqrt.cpp
		tests/MathUtils_test_linear_function.cpp
		tests/MathUtils_test_map.cpp
		tests/MathUtils_test_random8.cpp
		tests/MemoryUtils_test.cpptests/Utils_test.cpp
)
