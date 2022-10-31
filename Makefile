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
# MARK: - Config
#

MBED_GIT_URL     ?= $(shell cat $(ROOT_DIR)/config/mbed_git_url)
MBED_BRANCH      ?= $(shell cat $(ROOT_DIR)/config/mbed_version)
MBED_VERSION     ?= $(shell cat $(ROOT_DIR)/config/mbed_version)
MCUBOOT_GIT_URL  ?= $(shell cat $(ROOT_DIR)/config/mcuboot_git_url)
MCUBOOT_VERSION  ?= $(shell cat $(ROOT_DIR)/config/mcuboot_version)
OS_VERSION       ?= $(shell cat $(ROOT_DIR)/config/os_version)


#
# MARK: - Options
#

# build
BUILD_TYPE   ?= Release
TARGET_BOARD ?= LEKA_V1_2_DEV

# tests
COVERAGE   ?= ON
SANITIZERS ?= OFF
UT_LITE    ?= OFF

# os
ENABLE_LOG_DEBUG    ?= ON
ENABLE_SYSTEM_STATS ?= ON

# bootloader
BUILD_TARGETS_TO_USE_WITH_BOOTLOADER ?= OFF

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

EXCLUDE_FROM_GCOVR_COVERAGE = -e '.*Xcode.*' -e '.*_build.*' -e '.*extern.*' -e '.*tests/unit.*' -e '.*tests/mocks.*' -e '.*_test.*\..*' -e '.*include/boost.*' -e '.*include/semver.*' -e '.*include/interface.*' -e '.*CoreSTM32Hal.*'
EXCLUDE_FROM_LCOV_COVERAGE  =     '*Xcode*'      '*_build*'      '*extern*'      '*tests/unit*'      '*tests/mocks*'      '*_test*.*'        '*include/boost*'      '*include/semver*'      '*include/interface*'      '*CoreSTM32Hal*'

#
# MARK: - .bin path
#

LEKA_OS_BIN_PATH := $(TARGET_BUILD_DIR)/app/os/LekaOS.bin
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

bootloader:
	@echo ""
	@echo "🏗️  Building Bootloader 🤖"
	cmake --build $(TARGET_BUILD_DIR) -t bootloader

spikes:
	@echo ""
	@echo "🏗️  Building spikes 🍱"
	cmake --build $(TARGET_BUILD_DIR) -t spikes

tests_functional:
	@echo ""
	@echo "🏗️  Building functional tests ⚗️"
	cmake --build $(TARGET_BUILD_DIR) -t tests_functional

firmware:
	python3 tools/check_version.py ./config/os_version
	./tools/firmware/build_firmware.sh -r -v $(OS_VERSION)

firmware_no_cleanup:
	python3 tools/check_version.py ./config/os_version
	./tools/firmware/build_firmware.sh -v $(OS_VERSION)

#
# MARK: - Config targets
#

config:
	@$(MAKE) config_cmake_target
	@$(MAKE) config_cmake_build

config_tools:
	@$(MAKE) config_tools_target
# @$(MAKE) config_tools_build

clean:
	@$(MAKE) rm_build

clean_config:
	@$(MAKE) rm_build
	@$(MAKE) rm_config
	@$(MAKE) config

config_cmake_target: mkdir_cmake_config
	@echo ""
	@echo "🏃 Running configuration script for target $(TARGET_BOARD) 📝"
	python3 $(CMAKE_DIR)/scripts/configure_cmake_for_target.py $(TARGET_BOARD) -p $(CMAKE_CONFIG_DIR) -a $(ROOT_DIR)/config/mbed_app.json

config_tools_target: mkdir_tools_config
	@echo ""
	@echo "🏃 Running configuration script for VSCode CMake Tools 📝"
	python3 $(CMAKE_DIR)/scripts/configure_cmake_for_target.py $(TARGET_BOARD) -p $(CMAKE_TOOLS_CONFIG_DIR) -a $(ROOT_DIR)/config/mbed_app.json

config_cmake_build: mkdir_cmake_config
	@echo ""
	@echo "🏃 Running cmake configuration script for target $(TARGET_BOARD) 📝"
	@cmake -S . -B $(TARGET_BUILD_DIR) -GNinja -DCMAKE_CONFIG_DIR="$(CMAKE_CONFIG_DIR)" -DTARGET_BOARD="$(TARGET_BOARD)" -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) -DENABLE_LOG_DEBUG=$(ENABLE_LOG_DEBUG) -DENABLE_SYSTEM_STATS=$(ENABLE_SYSTEM_STATS) -DBUILD_TARGETS_TO_USE_WITH_BOOTLOADER=$(BUILD_TARGETS_TO_USE_WITH_BOOTLOADER) -DOS_VERSION=$(OS_VERSION)

config_tools_build: mkdir_tools_config
	@echo ""
	@echo "🏃 Running cmake configuration script for target $(TARGET_BOARD) 📝"
	@cmake -S . -B $(CMAKE_TOOLS_BUILD_DIR) -GNinja -DCMAKE_CONFIG_DIR="$(CMAKE_TOOLS_CONFIG_DIR)" -DTARGET_BOARD="$(TARGET_BOARD)" -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) -DENABLE_LOG_DEBUG=ON -DENABLE_SYSTEM_STATS=ON -DOS_VERSION=$(OS_VERSION)

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
	@gcovr --root . $(EXCLUDE_FROM_GCOVR_COVERAGE) --exclude-throw-branches --exclude-unreachable-branches
	@gcovr --root . $(EXCLUDE_FROM_GCOVR_COVERAGE) --exclude-throw-branches --exclude-unreachable-branches --html-details $(UNIT_TESTS_COVERAGE_DIR)/coverage.html
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
	@gcovr --root . $(EXCLUDE_FROM_GCOVR_COVERAGE) --exclude-throw-branches --exclude-unreachable-branches --sonarqube $(UNIT_TESTS_COVERAGE_DIR)/coverage.xml
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
	cmake -S ./tests/unit -B $(UNIT_TESTS_BUILD_DIR) -GNinja -DCMAKE_BUILD_TYPE=Debug -DCOVERAGE=$(COVERAGE) -DSANITIZERS=$(SANITIZERS) -DOS_VERSION=$(OS_VERSION) -DUT_LITE=$(UT_LITE)
	@mkdir -p $(CMAKE_TOOLS_BUILD_DIR)/unit_tests
	@ln -sf $(UNIT_TESTS_BUILD_DIR)/compile_commands.json $(CMAKE_TOOLS_BUILD_DIR)/unit_tests/compile_commands.json

config_unit_tests_asan:
	@$(MAKE) config_unit_tests SANITIZERS=ON

config_unit_tests_lite:
	@$(MAKE) config_unit_tests SANITIZERS=ON UT_LITE=ON

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
	@git diff --name-status develop            \
		| grep -E -v "_test"                   \
		| grep -E -v "extern"                  \
		| grep -E -v "gtest"                   \
		| grep -E -v "gmock"                   \
		| grep -E -v "mocks"                   \
		| grep -E -v "stubs"                   \
		| grep -E "^A|^M" | sed "s/^[AM]\t//g" \
		| grep -E "\.h\$$|\.cpp\$$"            \
		|| echo "No files added nor modified!"
	@echo ""
	@git diff --name-status develop            \
		| grep -E -v "_test"                   \
		| grep -E -v "extern"                  \
		| grep -E -v "gtest"                   \
		| grep -E -v "gmock"                   \
		| grep -E -v "mocks"                   \
		| grep -E -v "stubs"                   \
		| grep -E "^A|^M" | sed "s/^[AM]\t//g" \
		| grep -E "\.h\$$|\.cpp\$$"            \
		| xargs --no-run-if-empty ruby tools/run-clang-tidy.rb $(CMAKE_TOOLS_BUILD_DIR)

#
# MARK: - Mbed targets
#

pull_deps:
	@$(MAKE) mbed_clone
	@$(MAKE) mcuboot_clone

mbed_clone:
	@echo ""
	@echo "🧬 Cloning Mbed OS 📦"
	@rm -rf $(MBED_OS_DIR)
	git clone --depth=1 --branch=$(MBED_BRANCH) $(MBED_GIT_URL) $(MBED_OS_DIR)
	@$(MAKE) mbed_symlink_files

mbed_symlink_files:
	@echo ""
	@echo "🔗 Symlinking templates to Mbed OS directory 🗂️"
	ln -srf $(CMAKE_DIR)/templates/mbed/CMakeLists.txt $(MBED_OS_DIR)/CMakeLists.txt
	ln -srf $(CMAKE_DIR)/templates/mbed/.mbedignore    $(MBED_OS_DIR)/.mbedignore

#
# MARK: - Mcuboot targets
#

mcuboot_clone:
	@echo ""
	@echo "🧬 Cloning MCUBoot 📦"
	@rm -rf $(MCUBOOT_DIR)
	git clone --depth=1 --branch=$(MCUBOOT_VERSION) $(MCUBOOT_GIT_URL) $(MCUBOOT_DIR)
	@$(MAKE) mcuboot_symlink_files

mcuboot_symlink_files:
	@echo ""
	@echo "🔗 Symlinking CMakeLists.txt to MCUBoot directory 🗂️"
	ln -srf $(CMAKE_DIR)/templates/mcuboot/boot.cmake $(MCUBOOT_DIR)/boot/CMakeLists.txt
	ln -srf $(CMAKE_DIR)/templates/mcuboot/mbed.cmake $(MCUBOOT_DIR)/boot/mbed/CMakeLists.txt

#
# MARK: - Utils targets
#

mkdir_cmake_config:
	@mkdir -p $(CMAKE_CONFIG_DIR)

mkdir_tools_config:
	@mkdir -p $(CMAKE_TOOLS_CONFIG_DIR)

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
