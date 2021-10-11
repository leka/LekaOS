# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

#
# MARK: - Constants
#

ROOT_DIR          := $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
CMAKE_DIR         := $(ROOT_DIR)/cmake
MBED_OS_DIR       := $(ROOT_DIR)/extern/mbed-os
MCUBOOT_DIR       := $(ROOT_DIR)/extern/mcuboot

#
# MARK: - Arguments
#

PORT            ?= /dev/tty.usbmodem14303
MBED_GIT_URL    ?= $(shell cat $(ROOT_DIR)/.mbed_git_url)
MBED_BRANCH     ?= $(shell cat $(ROOT_DIR)/.mbed_version)
MBED_VERSION    ?= $(shell cat $(ROOT_DIR)/.mbed_version)
MCUBOOT_VERSION ?= $(shell cat $(ROOT_DIR)/.mcuboot_version)
BAUDRATE        ?= 115200
BUILD_TYPE      ?= Release
TARGET_BOARD    ?= LEKA_V1_2_DEV

#
# MARK: - Options
#

ENABLE_CODE_ANALYSIS ?= OFF
USE_BOOTLOADER       ?= FALSE

#
# MARK: - Build dirs
#

PROJECT_BUILD_DIR       := $(ROOT_DIR)/_build
TARGET_BUILD_DIR        := $(PROJECT_BUILD_DIR)/${TARGET_BOARD}
CMAKE_CONFIG_DIR        := $(TARGET_BUILD_DIR)/cmake_config
UNIT_TESTS_BUILD_DIR    := $(ROOT_DIR)/_build_unit_tests
UNIT_TESTS_COVERAGE_DIR := $(UNIT_TESTS_BUILD_DIR)/_coverage

#
# MARK: - VSCode CMake Tools
#

CMAKE_TOOLS_BUILD_DIR := $(ROOT_DIR)/_build_cmake_tools
CMAKE_TOOLS_CONFIG_DIR := $(CMAKE_TOOLS_BUILD_DIR)/cmake_config

#
# MARK: - Coverage exclusions
#

EXCLUDE_FROM_GCOVR_COVERAGE = -e '.*_build.*' -e '.*extern.*' -e '.*tests/unit.*' -e '.*_test.*\.cpp' -e '.*LKCoreSTM32Hal.*' -e '.*LKCoreFatFs.*'
EXCLUDE_FROM_LCOV_COVERAGE  = '*Xcode*' '*_build*' '*extern*' '*tests/unit*' '*_test*.cpp' '*LKCoreSTM32Hal*' '*LKCoreFatFs*'

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
	@$(MAKE) config_cmake_target
	@$(MAKE) config_cmake_build

config_tools:
	@$(MAKE) config_tools_target
	@$(MAKE) config_tools_build

clean:
	@$(MAKE) rm_build

clean_config:
	@$(MAKE) rm_build
	@$(MAKE) rm_config
	@$(MAKE) config

config_cmake_target: mkdir_cmake_config
	@echo ""
	@echo "🏃 Running configuration script for target $(TARGET_BOARD) 📝"
	python3 $(CMAKE_DIR)/scripts/configure_cmake_for_target.py $(TARGET_BOARD) -p $(CMAKE_CONFIG_DIR) -a $(ROOT_DIR)/mbed_app.json

config_tools_target: mkdir_cmake_config
	@echo ""
	@echo "🏃 Running configuration script for VSCode CMake Tools 📝"
	python3 $(CMAKE_DIR)/scripts/configure_cmake_for_target.py $(TARGET_BOARD) -p $(CMAKE_TOOLS_CONFIG_DIR) -a $(ROOT_DIR)/mbed_app.json

config_cmake_build: mkdir_tools_config
	@echo ""
	@echo "🏃 Running cmake configuration script for target $(TARGET_BOARD) 📝"
	@cmake -S . -B $(TARGET_BUILD_DIR) -GNinja -DCMAKE_CONFIG_DIR="$(CMAKE_CONFIG_DIR)" -DTARGET_BOARD="$(TARGET_BOARD)" -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) -DENABLE_CODE_ANALYSIS=$(ENABLE_CODE_ANALYSIS) -DUSE_BOOTLOADER=$(USE_BOOTLOADER)

config_tools_build: mkdir_tools_config
	@echo ""
	@echo "🏃 Running cmake configuration script for target $(TARGET_BOARD) 📝"
	@cmake -S . -B $(CMAKE_TOOLS_BUILD_DIR) -GNinja -DCMAKE_CONFIG_DIR="$(CMAKE_TOOLS_CONFIG_DIR)" -DTARGET_BOARD="$(TARGET_BOARD)" -DCMAKE_BUILD_TYPE=Debug -DENABLE_CODE_ANALYSIS=$(ENABLE_CODE_ANALYSIS)
	@ln -sf $(CMAKE_TOOLS_BUILD_DIR)/compile_commands.json ./

#
# MARK: - Tests targets
#

ut:
	@$(MAKE) unit_tests

utc:
	@$(MAKE) unit_tests
	@$(MAKE) coverage

unit_tests:
	@$(MAKE) build_unit_tests
	@$(MAKE) run_unit_tests

coverage:
	@echo ""
	@echo "🔬 Generating code coverage 📝"
	@echo ""
	@gcovr --root . $(EXCLUDE_FROM_GCOVR_COVERAGE)
	@gcovr --root . $(EXCLUDE_FROM_GCOVR_COVERAGE) --html-details $(UNIT_TESTS_COVERAGE_DIR)/coverage.html
	@echo ""
	@echo "📝 Html report can be viewed with:"
	@echo "    open $(UNIT_TESTS_COVERAGE_DIR)/coverage.html\n"

coverage_json:
	@echo ""
	@echo "🔬 Generating code coverage in json 📝"
	@gcovr --root . -e tests/unit/mbed-os -e googletest -e $(UNIT_TESTS_BUILD_DIR) --json > $(UNIT_TESTS_COVERAGE_DIR)/coverage.json
	@echo "📝 Json report is available at: $(UNIT_TESTS_COVERAGE_DIR)/coverage.json 📝"

coverage_sonarqube:
	@echo ""
	@echo "🔬 Generating code coverage 📝"
	@echo ""
	@gcovr --root . $(EXCLUDE_FROM_GCOVR_COVERAGE)
	@gcovr --root . $(EXCLUDE_FROM_GCOVR_COVERAGE) --sonarqube $(UNIT_TESTS_COVERAGE_DIR)/coverage.xml
	@echo ""
	@echo "📝 SonarQube XML report can be viewed with:"
	@echo "    open $(UNIT_TESTS_COVERAGE_DIR)/coverage.xml\n"

coverage_lcov:
	@echo ""
	@echo "🔬 Generating code coverage using lcov 📝"
	@mkdir -p $(UNIT_TESTS_COVERAGE_DIR)
	@lcov --capture --directory . --output-file $(UNIT_TESTS_COVERAGE_DIR)/_tmp_coverage.info
	@lcov --remove $(UNIT_TESTS_COVERAGE_DIR)/_tmp_coverage.info $(EXCLUDE_FROM_LCOV_COVERAGE) -o $(UNIT_TESTS_COVERAGE_DIR)/coverage.info
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
	@$(UNIT_TESTS_BUILD_DIR)/LekaOSUnitTestsExec --gtest_color=yes --gtest_output=xml:$(UNIT_TESTS_COVERAGE_DIR)/google_test_detail.xml

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

clang_tidy_diff:
	@echo ""
	@echo "🏃‍♂️ Running clang-tidy on modified files 🧹"
	@echo ""
	@git diff --name-status develop \
		| grep -E -v "_test" | grep -E "^A|^M" | sed "s/^[AM]\t//g" | grep -E "\.h\$$|\.cpp\$$"
	@echo ""
	@git diff --name-status develop \
		| grep -E -v "_test" | grep -E "^A|^M" | sed "s/^[AM]\t//g" | grep -E "\.h\$$|\.cpp\$$" \
		| xargs /usr/local/opt/llvm/bin/clang-tidy -p=. --quiet

clang_tidy_diff_fix:
	@echo ""
	@echo "🏃‍♂️ Running clang-tidy on modified files 🧹"
	@echo ""
	@git diff --name-status develop \
		| grep -E -v "_test" | grep -E "^A|^M" | sed "s/^[AM]\t//g" | grep -E "\.h\$$|\.cpp\$$"
	@echo ""
	@git diff --name-status develop \
		| grep -E -v "_test" | grep -E "^A|^M" | sed "s/^[AM]\t//g" | grep -E "\.h\$$|\.cpp\$$" \
		| xargs /usr/local/opt/llvm/bin/clang-tidy -p=. --quiet --fix --fix-errors

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
	git clone --depth=1 --branch=$(MBED_BRANCH) $(MBED_GIT_URL) $(MBED_OS_DIR)
	@$(MAKE) mbed_symlink_files

mbed_curl:
	@echo ""
	@echo "🧬 Curling Mbed OS 📦"
	@rm -rf $(MBED_OS_DIR)
	@mkdir -p $(MBED_OS_DIR)
	curl -O -L $(MBED_GIT_URL)/archive/$(MBED_VERSION).tar.gz
	tar -xzf $(MBED_VERSION).tar.gz --strip-components=1 -C extern/mbed-os
	rm -rf $(MBED_VERSION).tar.gz
	@$(MAKE) mbed_symlink_files

mbed_symlink_files:
	@echo ""
	@echo "🔗 Symlinking templates to Mbed OS directory 🗂️"
	ln -srf $(CMAKE_DIR)/templates/Template_MbedOS_CMakelists.txt $(MBED_OS_DIR)/CMakeLists.txt
	ln -srf $(CMAKE_DIR)/templates/Template_MbedOS_mbedignore.txt $(MBED_OS_DIR)/.mbedignore

#
# MARK: - Mcuboot targets
#

mcuboot_clone:
	@echo ""
	@echo "🧬 Cloning MCUBoot 📦"
	@rm -rf $(MCUBOOT_DIR)
	@git clone --single-branch https://github.com/mcu-tools/mcuboot.git $(MCUBOOT_DIR)
	@cd $(MCUBOOT_DIR) && git reset --hard $(MCUBOOT_VERSION) && cd $(ROOT_DIR)
	@$(MAKE) mcuboot_symlink_files

mcuboot_symlink_files:
	@echo ""
	@echo "🔗 Symlinking CMakeLists.txt to MCUBoot directory 🗂️"
	ln -srf $(CMAKE_DIR)/templates/Template_MCUBoot_CMakeLists.txt $(MCUBOOT_DIR)/boot/CMakeLists.txt
	ln -srf $(CMAKE_DIR)/templates/Template_MCUBoot_mbed_CMakeLists.txt $(MCUBOOT_DIR)/boot/mbed/CMakeLists.txt

#
# MARK: - Utils targets
#

mkdir_cmake_config:
	@mkdir -p $(CMAKE_CONFIG_DIR)

mkdir_tools_config:
	@mkdir -p $(CMAKE_CONFIG_DIR)

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
	rm -rf $(CMAKE_TOOLS_BUILD_DIR)
	rm -rf ./compile_commands.json

rm_config:
	@echo ""
	@echo "⚠️  Cleaning up $(TARGET_BOARD) cmake_config directory 🧹"
	rm -rf $(CMAKE_CONFIG_DIR)

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
