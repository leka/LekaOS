# Leka - LekaOS
# Copyright APF France handicap
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.31)
cmake_policy(VERSION 3.31)

# set directories
set(ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})
set(MBED_OS_DIR ${ROOT_DIR}/extern/mbed-os)

#
# MARK: - Project + Mbed Config
#

# mbed json config
set(MBED_APP_JSON_PATH ${ROOT_DIR}/config/mbed_app.json5)

# mbed toolchain setup
include(${MBED_OS_DIR}/tools/cmake/mbed_toolchain_setup.cmake)

# project definition
project(LekaOS VERSION 2.0.0 LANGUAGES C CXX ASM)

# mbed project setup
include(${MBED_OS_DIR}/tools/cmake/mbed_project_setup.cmake)

# custom targets config
# set(CUSTOM_TARGETS_JSON_PATH ${ROOT_DIR}/targets/custom_targets.json5)

# add custom targets first
# add_subdirectory(targets)

# add mbed next, mandatory each time
add_subdirectory(${MBED_OS_DIR})

#
# MARK: - Library, drivers, etc.
#

# add_subdirectory(YourLibrary)

# set spikes dir
set(SPIKES_DIR ${ROOT_DIR}/spikes)

add_subdirectory(${SPIKES_DIR}/mbed_hello_world)
