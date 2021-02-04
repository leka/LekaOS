# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_mbed_executable(hardware_motors_test_reduction_and_wheels)

target_include_directories(hardware_motors_test_reduction_and_wheels
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(hardware_motors_test_reduction_and_wheels
  PRIVATE
    main.cpp
)

target_link_libraries(hardware_motors_test_reduction_and_wheels LKCoreMotor)

target_link_libraries(hardware_motors_test_reduction_and_wheels lib_LekaMotors)
target_link_libraries(hardware_motors_test_reduction_and_wheels lib_PrettyPrinter)

target_link_custom_leka_targets(hardware_motors_test_reduction_and_wheels)
