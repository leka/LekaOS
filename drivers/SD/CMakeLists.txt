# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(driver_SD STATIC)

target_include_directories(driver_SD
  PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${CMAKE_CURRENT_SOURCE_DIR}/include/internal
)

target_sources(driver_SD
  PRIVATE
    source/SD.cpp
)

target_link_libraries(driver_SD mbed-os)
