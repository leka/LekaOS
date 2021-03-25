# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

#
# MARK: - Constants
#

ROOT_DIR          := $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
CMAKE_DIR         := $(ROOT_DIR)/cmake
MBED_OS_DIR       := $(ROOT_DIR)/extern/mbed-os
PROJECT_BUILD_DIR := $(ROOT_DIR)/_build

#
# MARK: - Arguments
#

PORT          ?= /dev/tty.usbmodem14303
BRANCH        ?= $(shell cat $(ROOT_DIR)/.mbed_version)
VERSION       ?= $(shell cat $(ROOT_DIR)/.mbed_version)
BAUDRATE      ?= 115200
BUILD_TYPE    ?= Release
TARGET_BOARD  ?= LEKA_V1_2_DEV
CODE_ANALYSIS ?= OFF

#
# MARK: - Build dirs
#

CMAKE_CONFIG_DIR        := $(PROJECT_BUILD_DIR)/cmake_config
TARGET_BUILD_DIR        := $(PROJECT_BUILD_DIR)/${TARGET_BOARD}
UNIT_TESTS_BUILD_DIR    := $(ROOT_DIR)/_build_unit_tests
UNIT_TESTS_COVERAGE_DIR := $(UNIT_TESTS_BUILD_DIR)/_coverage

EXCLUDE_FROM_COVERAGE = --exclude-directories '.*googletest.*' --exclude-directories '.*mbed-os.*' --exclude-directories '.*template.*' \
						-e '.*gtest.*' -e '.*mock.*' -e '.*_test\.cpp' -e 'extern.*' -e '.*st_.*' -e '.*LKCoreSTM32Hal.*' -e '.*LKCoreFatFs.*' \

#
# MARK: - .bin path
#

LEKA_OS_BIN_PATH := $(TARGET_BUILD_DIR)/src/LekaOS.bin
BIN_PATH         ?= $(LEKA_OS_BIN_PATH)

#
# MARK: - Build targets
#

.PHONY: spikes tests config build_unit_tests

all:
	@echo ""
	@echo "🏗️  Building everything! 🌈"
	cmake --build $(TARGET_BUILD_DIR)
	@rm -rf $(ROOT_DIR)/compile_commands.json
	@cp $(TARGET_BUILD_DIR)/compile_commands.json ./

os:
	@echo ""
	@echo "🏗️  Building LekaOS 🤖"
	cmake --build $(TARGET_BUILD_DIR) -t LekaOS

spikes:
	@echo ""
	@echo "🏗️  Building spikes 🍱"
	cmake --build $(TARGET_BUILD_DIR) -t spikes

tests_functional:
	@echo ""
	@echo "🏗️  Building functional tests ⚗️"
	cmake --build $(TARGET_BUILD_DIR) -t tests_functional

#
# MARK: - Config targets
#

config:
	@$(MAKE) config_target
	@$(MAKE) config_cmake

clean:
	@$(MAKE) rm_build

clean_config:
	@$(MAKE) rm_build
	@$(MAKE) rm_config
	@$(MAKE) config

config_target: mkdir_config
	@echo ""
	@echo "🏃 Running configuration script for target $(TARGET_BOARD) 📝"
	python3 $(CMAKE_DIR)/scripts/configure_cmake_for_target.py $(TARGET_BOARD) -p $(PROJECT_BUILD_DIR)/cmake_config/$(TARGET_BOARD) -a $(ROOT_DIR)/mbed_app.json

config_cmake: mkdir_build
	@echo ""
	@echo "🏃 Running cmake configuration script for target $(TARGET_BOARD) 📝"
	@cmake -S . -B $(TARGET_BUILD_DIR) -GNinja -DTARGET_BOARD="$(TARGET_BOARD)" -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) -DENABLE_CODE_ANALYSIS=$(CODE_ANALYSIS)

#
# MARK: - Tests targets
#

ut:
	@$(MAKE) unit_tests
	@$(MAKE) coverage

unit_tests:
	@$(MAKE) build_unit_tests
	@$(MAKE) run_unit_tests

coverage:
	@echo ""
	@echo "🔬 Generating code coverage 📝"
	@echo ""
	@gcovr -r . $(EXCLUDE_FROM_COVERAGE)
	@gcovr -r . $(EXCLUDE_FROM_COVERAGE) --html-details $(UNIT_TESTS_COVERAGE_DIR)/coverage.html
	@echo ""
	@echo "📝 Html report can be viewed with:"
	@echo "    open $(UNIT_TESTS_COVERAGE_DIR)/coverage.html\n"

coverage_json:
	@echo ""
	@echo "🔬 Generating code coverage in json 📝"
	@gcovr -r . -e tests/unit/mbed-os -e googletest -e $(UNIT_TESTS_BUILD_DIR) --json > $(UNIT_TESTS_COVERAGE_DIR)/coverage.json
	@echo "📝 Json report is available at: $(UNIT_TESTS_COVERAGE_DIR)/coverage.json 📝"

coverage_lcov:
	@echo ""
	@echo "🔬 Generating code coverage using lcov 📝"
	@mkdir -p $(UNIT_TESTS_COVERAGE_DIR)
	@lcov --capture --directory . --output-file $(UNIT_TESTS_COVERAGE_DIR)/_tmp_coverage.info
	@lcov --remove $(UNIT_TESTS_COVERAGE_DIR)/_tmp_coverage.info '/usr/*' '*extern*' '*googletest*' '*v1*' '*Xcode*' '*CommandLineTools*' '*tests/unit*' -o $(UNIT_TESTS_COVERAGE_DIR)/coverage.info
	@lcov --list $(UNIT_TESTS_COVERAGE_DIR)/coverage.info

view_coverage:
	@echo ""
	@echo "🔬 Opening code coverage in browser 📝"
	@open $(UNIT_TESTS_COVERAGE_DIR)/coverage.html

build_unit_tests:
	@echo ""
	@echo "🏗️  Building unit tests 🧪"
	cmake --build $(UNIT_TESTS_BUILD_DIR)

run_unit_tests:
	@echo ""
	@echo "🏃‍♂️ Running unit tests 🧪"
	@$(UNIT_TESTS_BUILD_DIR)/LekaOSUnitTestsExec

config_unit_tests: mkdir_build_unit_tests
	@echo ""
	@echo "🏃 Running unit tests cmake configuration script 📝"
	cmake -S ./tests/unit -B $(UNIT_TESTS_BUILD_DIR) -GNinja -DCMAKE_BUILD_TYPE=Debug -DCOVERAGE=True

clean_unit_tests:
	@$(MAKE) rm_unit_tests

rm_unit_tests:
	@echo ""
	@echo "⚠️  Cleaning up unit tests build directories 🧹"
	rm -rf $(UNIT_TESTS_BUILD_DIR)

#
# MARK: - Tools targets
#

clang_format:
	@echo ""
	@echo "🏃‍♂️ Running clang-format 🎨"
	python3 tools/run-clang-format.py -r --extension=h,c,cpp --color=always --style=file .

clang_format_fix:
	@echo ""
	@echo "🏃‍♂️ Running clang-format & fixing files 🎨"
	python3 tools/run-clang-format.py -r --extension=h,c,cpp --color=always --style=file . -i

code_analysis: mkdir_build
	@echo ""
	@echo "🏃‍♂️ Running cppcheck code analysis 🔬"
	@mkdir -p $(PROJECT_BUILD_DIR)/cppcheck
	cmake -S . -B $(PROJECT_BUILD_DIR)/cppcheck -GNinja -DTARGET_BOARD="$(TARGET_BOARD)" -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) -DENABLE_CODE_ANALYSIS=ON
	cmake --build $(PROJECT_BUILD_DIR)/cppcheck

#
# MARK: - Mbed targets
#

mbed_clone:
	@echo ""
	@echo "🧬 Cloning Mbed OS 📦"
	@rm -rf $(MBED_OS_DIR)
	git clone --depth=1 --branch=$(BRANCH) https://github.com/ARMmbed/mbed-os $(MBED_OS_DIR)
	@$(MAKE) mbed_symlink_files

mbed_curl:
	@echo ""
	@echo "🧬 Curling Mbed OS 📦"
	@rm -rf $(MBED_OS_DIR)
	@mkdir -p $(MBED_OS_DIR)
	curl -O -L https://github.com/ARMmbed/mbed-os/archive/$(VERSION).tar.gz
	tar -xzf $(VERSION).tar.gz --strip-components=1 -C extern/mbed-os
	rm -rf $(VERSION).tar.gz
	@$(MAKE) mbed_symlink_files

mbed_symlink_files:
	@echo ""
	@echo "🔗 Symlinking templates to Mbed OS directory 🗂️"
	ln -srf $(CMAKE_DIR)/templates/Template_MbedOS_CMakelists.txt $(MBED_OS_DIR)/CMakeLists.txt
	ln -srf $(CMAKE_DIR)/templates/Template_MbedOS_mbedignore.txt $(MBED_OS_DIR)/.mbedignore

#
# MARK: - Utils targets
#

mkdir_build:
	@mkdir -p $(TARGET_BUILD_DIR)

mkdir_config:
	@mkdir -p $(CMAKE_CONFIG_DIR)/$(TARGET_BOARD)

mkdir_build_unit_tests:
	@mkdir -p $(UNIT_TESTS_BUILD_DIR)
	@mkdir -p $(UNIT_TESTS_COVERAGE_DIR)

rm_build:
	@echo ""
	@echo "⚠️  Cleaning up $(TARGET_BOARD) build directory 🧹"
	rm -rf $(TARGET_BUILD_DIR)

rm_build_all:
	@echo ""
	@echo "⚠️  Cleaning up all build directories 🧹"
	rm -rf $(PROJECT_BUILD_DIR)

rm_config:
	@echo ""
	@echo "⚠️  Cleaning up $(TARGET_BOARD) cmake_config directory 🧹"
	rm -rf $(CMAKE_CONFIG_DIR)/$(TARGET_BOARD)

deep_clean:
	@$(MAKE) rm_build_all
	@$(MAKE) rm_unit_tests

ccache_prebuild:
	@echo ""
	@echo "🪆 Ccache config ⚙️"
	@ccache -p
	@echo ""
	@echo "🪆 Ccache pre build stats 📉"
	@ccache -s
	@ccache -z

ccache_postbuild:
	@echo ""
	@echo "🪆 Ccache post build stats 📈"
	@ccache -s

flash:
	openocd -f interface/stlink.cfg -c 'transport select hla_swd' -f target/stm32f7x.cfg -c 'program $(BIN_PATH) 0x08000000' -c exit
	sleep 1
	@$(MAKE) reset

reset:
	openocd -f interface/stlink.cfg -c 'transport select hla_swd' -f target/stm32f7x.cfg -c init -c 'reset run' -c exit

term:
	mbed sterm -b $(BAUDRATE) -p $(PORT)
