# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_mbed_executable(spike_lk_temperature_sensor)

target_sources(spike_lk_temperature_sensor
  PRIVATE
    main.cpp
)

target_include_directories(spike_lk_temperature_sensor
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(spike_lk_temperature_sensor LKCoreTemperatureSensor)
target_link_libraries(spike_lk_temperature_sensor lib_HelloWorld)

target_link_custom_leka_targets(spike_lk_temperature_sensor)
