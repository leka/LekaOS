# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(CoreTimeout STATIC)

target_include_directories(CoreTimeout
	PUBLIC
	include
)

target_sources(CoreTimeout
	PRIVATE
	source/CoreTimeout.cpp
)

target_link_libraries(CoreTimeout
	mbed-os
)
