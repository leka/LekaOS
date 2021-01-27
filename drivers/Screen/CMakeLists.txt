# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(driver_Screen STATIC)

target_include_directories(driver_Screen
  PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_sources(driver_Screen
  PRIVATE
    source/otm8009a.c
    source/otm8009a_conf.cpp
    source/dsi.cpp
    source/Screen.cpp
    source/sdram.c
    source/Display.cpp
    source/ltdc.cpp
)

target_link_libraries(driver_Screen mbed-os)
