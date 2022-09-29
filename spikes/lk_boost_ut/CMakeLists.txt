# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_mbed_executable(spike_lk_boost_ut)

target_include_directories(spike_lk_boost_ut
	PRIVATE
	.
)

target_sources(spike_lk_boost_ut
	PRIVATE
	main.cpp
	tests/test_array.cpp
	tests/test_expect.cpp
	tests/test_minimal.cpp
	tests/test_mutable.cpp
	tests/test_parametrized.cpp
	tests/test_should.cpp
	tests/test_skip.cpp
	tests/test_spec.cpp
)

target_link_libraries(spike_lk_boost_ut
)

target_link_custom_leka_targets(spike_lk_boost_ut)
