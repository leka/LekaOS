# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(lib_HelloWorld STATIC)

target_include_directories(lib_HelloWorld
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_sources(lib_HelloWorld
  PRIVATE
    source/HelloWorld.cpp
)

target_link_libraries(lib_HelloWorld mbed-os)
