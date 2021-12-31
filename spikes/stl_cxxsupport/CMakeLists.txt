# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_executable(spike_stl_cxxsupport)

target_include_directories(spike_stl_cxxsupport
	PRIVATE
		.
)

target_sources(spike_stl_cxxsupport
	PRIVATE
		main.cpp
)

target_link_libraries(spike_stl_cxxsupport
	PUBLIC
		mbed-os-static
)

mbed_set_post_build(spike_stl_cxxsupport)
