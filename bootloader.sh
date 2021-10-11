#!/bin/bash

mkdir _tmp
mkdir _release

# Compile bootloader
make deep_clean
make config ENABLE_BOOTLOADER=OFF
make

# Get bootloader binary
cp _build/LEKA_V1_2_DEV/bootloader/bootloader.hex _tmp/bootloader.hex

# Compile applications
make deep_clean
make config ENABLE_BOOTLOADER=ON
make

# Get application binary
cp _build/LEKA_V1_2_DEV/src/LekaOS.hex _tmp/application.hex

# Sign application with private key
imgtool sign -k signing-keys.pem --align 4 -v 1.2.3+4 --header-size 4096 --pad-header -S 0x180000 _tmp/application.hex _tmp/application-signed.hex

# Merge bootloader and application
hexmerge.py -o _tmp/firmware.hex --no-start-addr _tmp/bootloader.hex _tmp/application-signed.hex
arm-none-eabi-objcopy -I ihex -O binary _tmp/firmware.hex _release/firmware.bin

# Get update binary
cp _build/LEKA_V1_2_DEV/spikes/lk_ble/spike_lk_ble.bin _tmp/update.bin

# Sign update with private key
imgtool sign -k signing-keys.pem --align 4 -v 1.2.3+5 --header-size 4096 --pad-header -S 0x180000 _tmp/update.bin _release/update-signed.bin

rm -rf _tmp
