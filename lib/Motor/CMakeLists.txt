# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(lib_Motor STATIC)

target_include_directories(lib_Motor
  PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${CMAKE_CURRENT_SOURCE_DIR}/include/internal
)

target_sources(lib_Motor
  PRIVATE
    source/Motor.cpp
)

target_link_libraries(lib_Motor mbed-os)
