#!/bin/bash

mkdir _tmp
mkdir _release

# Variables
BOOTLOADER_HEX_SOURCE="_build/LEKA_V1_2_DEV/bootloader/bootloader.hex"
BOOTLOADER_HEX="_tmp/bootloader.hex"

APPLICATION_HEX_SOURCE="_build/LEKA_V1_2_DEV/src/LekaOS.hex"
APPLICATION_VERSION="1.2.3+4"
APPLICATION_SIGNED_HEX="_tmp/application-signed.hex"

FIRMWARE_HEX="_tmp/firmwware.hex"
FIRMWARE_RELEASE="_release/firmware.bin"

# Get bootloader
./bootloader.sh $BOOTLOADER_HEX_SOURCE $BOOTLOADER_HEX

# Get application
./application.sh $APPLICATION_HEX_SOURCE $APPLICATION_VERSION $APPLICATION_SIGNED_HEX

# Merge bootloader and application
hexmerge.py -o $FIRMWARE_HEX --no-start-addr $BOOTLOADER_HEX $APPLICATION_SIGNED_HEX
arm-none-eabi-objcopy -I ihex -O binary $FIRMWARE_HEX $FIRMWARE_RELEASE
