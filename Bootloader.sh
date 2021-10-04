#!/bin/bash

# Compile bootloader
make deep_clean && make config ENABLE_BOOTLOADER=OFF && make

# Get bootloader binary
cp _build/LEKA_V1_2_DEV/bootloader/bootloader.hex bootloader/bootloader.hex

# Compile applications
make deep_clean && make config ENABLE_BOOTLOADER=ON && make

# Get application binary
cp _build/LEKA_V1_2_DEV/src/LekaOS.hex src/LekaOS.hex

# Sign application with private key
imgtool sign -k signing-keys.pem --align 4 -v 1.2.3+4 --header-size 4096 --pad-header -S 0x180000 src/LekaOS.hex src/LekaOS-signed.hex

# Merge bootloader and application
hexmerge.py -o firmware.hex --no-start-addr bootloader/bootloader.hex src/LekaOS-signed.hex
arm-none-eabi-objcopy -I ihex -O binary firmware.hex firmware.bin

# Get update binary
cp _build/LEKA_V1_2_DEV/spikes/lk_ble/spike_lk_ble.hex update.hex

# Sign update with private key
imgtool sign -k signing-keys.pem --align 4 -v 1.2.3+5 --header-size 4096 --pad-header -S 0x180000 update.hex update-signed.hex
arm-none-eabi-objcopy -I ihex -O binary update-signed.hex update.bin
