# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

set(TARGET_NAME spike_mbed_hello_world)

add_executable(${TARGET_NAME})

target_include_directories(${TARGET_NAME}
	PRIVATE
		.
)

target_sources(${TARGET_NAME}
	PRIVATE
		main.cpp
)

target_link_libraries(${TARGET_NAME}
	PRIVATE
		mbed-os
)

mbed_set_post_build(${TARGET_NAME})
