# Mbed CMake Template
# Copyright 2020 Ladislas de Toldi (ladislas [at] detoldi.me)
# SPDX-License-Identifier: Apache-2.0

add_mbed_executable(spike_investigation_day_led)

target_sources(spike_investigation_day_led
  PRIVATE
    main.cpp
)

target_include_directories(spike_investigation_day_led
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(spike_investigation_day_led lib_Led)
target_link_libraries(spike_investigation_day_led lib_HelloWorld)
