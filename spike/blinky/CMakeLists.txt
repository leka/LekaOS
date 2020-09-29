# Mbed CMake Template
# Copyright 2020 Ladislas de Toldi (ladislas [at] detoldi.me)
# SPDX-License-Identifier: Apache-2.0

add_mbed_executable(spike_blinky)

target_sources(spike_blinky
  PRIVATE
    main.cpp
)

target_include_directories(spike_blinky
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
