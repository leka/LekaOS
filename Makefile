# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

#
# MARK: - Constants
#

ROOT_DIR    := $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
CMAKE_DIR   := $(ROOT_DIR)/cmake
BUILD_DIR   := $(ROOT_DIR)/build
MBED_OS_DIR := $(ROOT_DIR)/extern/mbed-os

#
# MARK:- Arguments
#

PORT         ?= /dev/tty.usbmodem14303
BRANCH       ?= master
PROJECT      ?=
VERSION      ?= mbed-os-6.3.0
BAUDRATE     ?= 115200
BIN_PATH     ?= $(BUILD_DIR)/src/LekaOS.bin
BUILD_TYPE   ?= Release
TARGET_BOARD ?= -x LEKA_V1_0_DEV

#
# MARK:- Targets
#

all:
	@echo ""
	@echo "🏗️  Building application 🚧"
	ninja -C ./build -f build.ninja $(PROJECT)

lekaos:
	@echo ""
	@echo "🏗️  Building LekaOS 🚧"
	ninja -C ./build -f build.ninja LekaOS

clean:
	@echo ""
	@echo "⚠️  Cleaning up build & cmake/config directories 🧹"
	rm -rf $(BUILD_DIR)
	rm -rf $(CMAKE_DIR)/config

config_leka_disco:
	@$(MAKE) config TARGET_BOARD="-x LEKA_DISCO"

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

curl_mbed:
	@echo ""
	@echo "🧬 Curling Mbed OS ⚗️"
	@rm -rf $(MBED_OS_DIR)
	@mkdir -p $(MBED_OS_DIR)
	curl -O -L https://github.com/ARMmbed/mbed-os/archive/$(VERSION).tar.gz
	tar -xzf $(VERSION).tar.gz --strip-components=1 -C extern/mbed-os
	rm -rf $(VERSION).tar.gz
	@echo ""
	@echo "🔗 Symlinking templates to Mbed OS directory 🗂️"
	ln -srf $(CMAKE_DIR)/templates/Template_MbedOS_CMakelists.txt $(MBED_OS_DIR)/CMakeLists.txt
	ln -srf $(CMAKE_DIR)/templates/Template_MbedOS_mbedignore.txt $(MBED_OS_DIR)/.mbedignore

flash:
	openocd -f interface/stlink.cfg -c 'transport select hla_swd' -f target/stm32f7x.cfg -c 'program $(BIN_PATH) 0x08000000' -c exit
	sleep 1
	@$(MAKE) reset

reset:
	openocd -f interface/stlink.cfg -c 'transport select hla_swd' -f target/stm32f7x.cfg -c init -c 'reset run' -c exit

term:
	mbed sterm -b $(BAUDRATE) -p $(PORT)
