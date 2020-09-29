# Mbed CMake Template
# Copyright 2020 Ladislas de Toldi (ladislas [at] detoldi.me)
# SPDX-License-Identifier: Apache-2.0

add_mbed_executable(LekaOS)

target_sources(LekaOS
  PRIVATE
    main.cpp
)

target_include_directories(LekaOS
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(LekaOS lib_HelloWorld)

target_link_target_board_libraries(LekaOS DISCO_ORIGINAL LEKA_V1_0_DEV)
