# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_mbed_executable(spike_investigation_day_all)

target_sources(spike_investigation_day_all
  PRIVATE
    main.cpp
)

target_include_directories(spike_investigation_day_all
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(spike_investigation_day_all lib_LekaBluetooth)
target_link_libraries(spike_investigation_day_all lib_LekaFirmware)
target_link_libraries(spike_investigation_day_all lib_LekaRfid)
target_link_libraries(spike_investigation_day_all lib_LekaScreen)
target_link_libraries(spike_investigation_day_all lib_LekaTouch)
target_link_libraries(spike_investigation_day_all lib_LekaWifi)
target_link_libraries(spike_investigation_day_all lib_HelloWorld)

target_link_target_board_libraries(spike_investigation_day_all DISCO_ORIGINAL LEKA_V1_0_DEV)
