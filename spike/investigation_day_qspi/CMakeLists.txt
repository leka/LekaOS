# Mbed CMake Template
# Copyright 2020 Ladislas de Toldi (ladislas [at] detoldi.me)
# SPDX-License-Identifier: Apache-2.0

add_mbed_executable(spike_investigation_day_qspi)

target_sources(spike_investigation_day_qspi
  PRIVATE
    main.cpp
)

target_include_directories(spike_investigation_day_qspi
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(spike_investigation_day_qspi lib_HelloWorld)
target_link_libraries(spike_investigation_day_qspi lib_LekaFirmware)

target_link_target_board_libraries(spike_investigation_day_qspi DISCO_ORIGINAL LEKA_V1_0_DEV)
