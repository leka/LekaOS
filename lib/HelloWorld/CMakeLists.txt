# Mbed CMake Template
# Copyright 2020 Ladislas de Toldi (ladislas [at] detoldi.me)
# SPDX-License-Identifier: Apache-2.0

add_library(lib_HelloWorld STATIC)

target_sources(lib_HelloWorld
  PRIVATE
    HelloWorld.cpp
)

target_include_directories(lib_HelloWorld
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(lib_HelloWorld mbed-os)
