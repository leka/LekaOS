#!/bin/bash

mkdir _tmp
mkdir _release

# Variables
BOOTLOADER_HEX_SOURCE="_build/LEKA_V1_2_DEV/bootloader/bootloader.hex"
BOOTLOADER_HEX="_tmp/bootloader.hex"

APPLICATION_HEX_SOURCE="_build/LEKA_V1_2_DEV/src/LekaOS.hex"
APPLICATION_VERSION="1.2.3+4"
APPLICATION_HEX="_tmp/application.hex"
APPLICATION_SIGNED_HEX="_tmp/application-signed.hex"

UPDATE_BIN_SOURCE="_build/LEKA_V1_2_DEV/spikes/lk_ble/spike_lk_ble.bin"
UPDATE_VERSION="1.2.3+5"
UPDATE_BIN="_tmp/update.bin"
UPDATE_RELEASE="_release/update-signed.bin"

FIRMWARE_HEX="_tmp/firmwware.hex"
FIRMWARE_RELEASE="_release/firmware.bin"

# Compile bootloader
make deep_clean
make config ENABLE_BOOTLOADER=OFF
make

# Get bootloader binary
cp $BOOTLOADER_HEX_SOURCE $BOOTLOADER_HEX

# Compile applications
make deep_clean
make config ENABLE_BOOTLOADER=ON
make

# Get application binary
cp $APPLICATION_HEX_SOURCE $APPLICATION_HEX

# Sign application with private key
imgtool sign -k signing-keys.pem --align 4 -v $APPLICATION_VERSION --header-size 4096 --pad-header -S 0x180000 $APPLICATION_HEX $APPLICATION_SIGNED_HEX

# Merge bootloader and application
hexmerge.py -o $FIRMWARE_HEX --no-start-addr $BOOTLOADER_HEX $APPLICATION_SIGNED_HEX
arm-none-eabi-objcopy -I ihex -O binary $FIRMWARE_HEX $FIRMWARE_RELEASE

# Get update binary
cp $UPDATE_BIN_SOURCE $UPDATE_BIN

# Sign update with private key
imgtool sign -k signing-keys.pem --align 4 -v $UPDATE_VERSION --header-size 4096 --pad-header -S 0x180000 $UPDATE_BIN $UPDATE_RELEASE

rm -rf _tmp
