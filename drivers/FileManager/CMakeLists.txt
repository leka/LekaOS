# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(driver_file_manager STATIC)

target_include_directories(driver_file_manager
  PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${CMAKE_CURRENT_SOURCE_DIR}/include/internal
)

target_sources(driver_file_manager
  PRIVATE
    source/FileManager.cpp
)

target_link_libraries(driver_file_manager mbed-os)
