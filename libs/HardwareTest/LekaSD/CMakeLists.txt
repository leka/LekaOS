# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(lib_LekaSD STATIC)

target_include_directories(lib_LekaSD
  PUBLIC
  	${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_sources(lib_LekaSD
  PRIVATE
	source/LekaSD.cpp
)

target_link_libraries(lib_LekaSD mbed-os)
