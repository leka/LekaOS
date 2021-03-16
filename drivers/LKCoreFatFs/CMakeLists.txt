# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(LKCoreFatFs STATIC)

target_include_directories(LKCoreFatFs
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_sources(LKCoreFatFs
	PRIVATE
		source/LKCoreFatFs.cpp
)

target_link_libraries(LKCoreFatFs mbed-os)
