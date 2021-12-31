# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_executable(LekaOS)

target_include_directories(LekaOS
	PRIVATE
		.
)

target_sources(LekaOS
	PRIVATE
		main.cpp
)

target_link_libraries(LekaOS
	PUBLIC
		mbed-os-static
		HelloWorld
)

mbed_set_post_build(LekaOS)
