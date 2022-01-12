# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(CoreFs STATIC)

target_include_directories(CoreFs
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_sources(CoreFs
	PRIVATE
		source/CoreFatFs.cpp
)

target_link_libraries(CoreFs mbed-os)
