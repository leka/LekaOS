# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(driver_Motor STATIC)

target_include_directories(driver_Motor
  PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_sources(driver_Motor
  PRIVATE
    source/Motor.cpp
)

target_link_libraries(driver_Motor mbed-os)
