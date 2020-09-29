gd # Mbed CMake Template
# Copyright 2020 Ladislas de Toldi (ladislas [at] detoldi.me)
# SPDX-License-Identifier: Apache-2.0

#
# MARK: - Constants
#

ROOT_DIR    := $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
CMAKE_DIR   := $(ROOT_DIR)/cmake
BUILD_DIR   := $(ROOT_DIR)/build
MBED_OS_DIR := $(ROOT_DIR)/lib/_vendor/mbed-os

#
# MARK:- Arguments
#

BRANCH       ?= master
PROJECT      ?=
PROGRAM      ?= src/LekaOS.bin
BAUDRATE     ?= 115200
BUILD_TYPE   ?= Release
TARGET_BOARD ?= -x LEKA_V1_0_DEV

#
# MARK:- Targets
#

all:
	@echo ""
	@echo "🏗️  Building application 🚧"
	ninja -C ./build -f build.ninja $(PROJECT)

clean:
	@echo ""
	@echo "⚠️  Cleaning up build & cmake/config directories 🧹"
	rm -rf $(BUILD_DIR)
	rm -rf $(CMAKE_DIR)/config

config:
	@$(MAKE) clean
	@echo ""
	@$(MAKE) config_target
	@echo ""
	@$(MAKE) config_cmake

config_target:
	@echo ""
	@echo "🏃 Running target configuration script 📝"
	python3 $(CMAKE_DIR)/scripts/configure_cmake_for_target.py $(TARGET_BOARD) -p $(CMAKE_DIR)/config -a $(ROOT_DIR)/mbed_app.json

config_cmake:
	@echo ""
	@echo "🏃 Running cmake configuration script 📝"
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR); cmake -GNinja -DTARGET_BOARD="$(TARGET_BOARD)" -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) ..

clone_mbed:
	@echo ""
	@echo "🧬 Cloning Mbed OS ⚗️"
	@rm -rf $(MBED_OS_DIR)
	git clone --depth=1 --branch=$(BRANCH) https://github.com/ARMmbed/mbed-os $(MBED_OS_DIR)
	@echo ""
	@echo "🔗 Symlinking templates to Mbed OS directory 🗂️"
	ln -srf $(CMAKE_DIR)/templates/Template_MbedOS_CMakelists.txt $(MBED_OS_DIR)/CMakeLists.txt
	ln -srf $(CMAKE_DIR)/templates/Template_MbedOS_mbedignore.txt $(MBED_OS_DIR)/.mbedignore

flash:
	@diskutil list | grep "DIS_" | awk '{print $$5}' | xargs -I {} diskutil unmount '/dev/{}'
	@diskutil list | grep "DIS_" | awk '{print $$5}' | xargs -I {} diskutil mount   '/dev/{}'
	cp build/$(PROGRAM) /Volumes/DIS_F769NI

term:
	mbed sterm -b $(BAUDRATE)
