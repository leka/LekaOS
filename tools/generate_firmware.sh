#!/bin/bash

# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

#
# MARK: - Requirements
#

ROOT_DIR=$(pwd)

if [[ $ROOT_DIR != *LekaOS ]]; then
	echo "Script *must* be run from the root of the project with:"
	echo "    ./tools/generate_firmware.sh"
	exit 1
fi

#
# MARK: - Variables
#

# paths
CONFIG_DIR="$ROOT_DIR/config"
FIRMWARE_BUILD_DIR="$ROOT_DIR/_build_firmware/app"
RELEASE_DIR="$FIRMWARE_BUILD_DIR/release"

# version + build number
OS_VERSION="$(cat $CONFIG_DIR/os_version)+$(date +%s)"

# bootloader
BOOTLOADER_HEX_PATH="_build/LEKA_V1_2_DEV/app/bootloader/bootloader.hex"

# os
OS_HEX_PATH="$FIRMWARE_BUILD_DIR/os/LekaOS.hex"
OS_SIGNED_HEX_PATH="$RELEASE_DIR/LekaOS-$OS_VERSION.hex"
OS_SIGNED_BIN_PATH="$RELEASE_DIR/LekaOS-$OS_VERSION.bin"

# firmware
FIRMWARE_HEX="$RELEASE_DIR/Firmware-$OS_VERSION.hex"
FIRMWARE_BIN="$RELEASE_DIR/Firmware-$OS_VERSION.bin"

#
# MARK: - Generate MCUboot Application (= os)
#

# ? Note: MCUboot uses the term "application" where Leka uses the term "os"
# ?       Both refer to the same thing, application == os

echo " -- Create directory $RELEASE_DIR"
rm -rf $RELEASE_DIR
mkdir -p $RELEASE_DIR

echo " -- Create signed os hex file"
imgtool sign -k signing-keys.pem --align 4 -v $OS_VERSION --header-size 4096 --pad-header -S 0x180000 $OS_HEX_PATH $OS_SIGNED_HEX_PATH

echo " -- Convert signed os hex file to binary format"
arm-none-eabi-objcopy -I ihex -O binary $OS_SIGNED_HEX_PATH $OS_SIGNED_BIN_PATH

#
# MARK: - Generate firmware = bootloader + application (os)
#

echo " -- Merge bootloader + os to create firmware hex file"
hexmerge.py -o $FIRMWARE_HEX --no-start-addr $BOOTLOADER_HEX_PATH $OS_SIGNED_HEX_PATH

echo " -- Convert firmware hex file to binary format"
arm-none-eabi-objcopy -I ihex -O binary $FIRMWARE_HEX $FIRMWARE_BIN
