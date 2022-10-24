# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

register_functional_test(
	TARGET
		functional_ut_boost_ut

	INCLUDE_DIRECTORIES

	SOURCES
		test_array.cpp
		test_expect.cpp
		test_minimal.cpp
		test_mutable.cpp
		test_parametrized.cpp
		test_should.cpp
		test_skip.cpp
		test_spec.cpp

	LINK_LIBRARIES
)
