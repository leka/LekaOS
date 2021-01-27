# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(driver_ID STATIC)

target_include_directories(driver_ID
  PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_sources(driver_ID
  PRIVATE
    source/ID.cpp
)

target_link_libraries(driver_ID mbed-os)
