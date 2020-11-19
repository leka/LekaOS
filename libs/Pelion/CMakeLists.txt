# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(lib_Pelion STATIC)

target_include_directories(lib_Pelion
  PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${CMAKE_CURRENT_SOURCE_DIR}/include/internal
)

target_link_libraries(lib_Pelion mbed-os)
