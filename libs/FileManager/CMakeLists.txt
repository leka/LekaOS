# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(FileManager STATIC)

target_include_directories(FileManager
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_sources(FileManager
	PRIVATE
		source/FileManager.cpp
)

target_link_libraries(FileManager
	PUBLIC
		mbed-os-static
		CoreFs
)
