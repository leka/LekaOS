# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_mbed_executable(functional_ut_boost_ut)

target_include_directories(functional_ut_boost_ut
	PRIVATE
	${TESTS_FUNCTIONAL_INCLUDE_DIR}
)

target_sources(functional_ut_boost_ut
	PRIVATE
	test_array.cpp
	test_expect.cpp
	test_minimal.cpp
	test_mutable.cpp
	test_parametrized.cpp
	test_should.cpp
	test_skip.cpp
	test_spec.cpp
)

target_link_libraries(functional_ut_boost_ut
)

add_main_to_functional_test(functional_ut_boost_ut)

target_link_custom_leka_targets(functional_ut_boost_ut)
