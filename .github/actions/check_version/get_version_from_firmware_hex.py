#!/usr/bin/env python3

# Leka - LekaOS
# Copyright 2023 APF France handicap
# SPDX-License-Identifier: Apache-2.0

import sys
import argparse
import utils

#
# MARK: - argparse
#


parser = argparse.ArgumentParser(
    description="Get version from firmware hex file")

parser.add_argument("firmware_hex_path", type=str,
                    help="path of firmware hex file")

args = parser.parse_args()


#
# MARK: - Functions
#


def getLineOfVersion() -> str:
    START_OS_MARKER = ":020000040804EE\n"

    firmware_raw = utils.getDataFromFile(args.firmware_hex_path)

    pos_os_start = firmware_raw.find(START_OS_MARKER)
    pos_line_previous_version_line = firmware_raw.find(
        "\n", pos_os_start + len(START_OS_MARKER) + 1
    )
    pos_version_start_line = firmware_raw.find(
        "\n", pos_line_previous_version_line)
    pos_version_end_line = firmware_raw.find("\n", pos_version_start_line + 1)

    return firmware_raw[pos_version_start_line:pos_version_end_line]


def getFirmwareVersionRaw() -> str:
    START_VERSION_MARKER = 18
    VERSION_SIZE = 16

    version_line = getLineOfVersion()

    version_raw = version_line[
        START_VERSION_MARKER: START_VERSION_MARKER + VERSION_SIZE
    ]

    return version_raw


def getFirmwareVersion() -> str:
    raw = getFirmwareVersionRaw()

    major = int(raw[0:2], 16)
    minor = int(raw[2:4], 16)
    revision = int(
        raw[6:8] + raw[4:6], 16
    )
    build = int(
        raw[14:16]
        + raw[12:14]
        + raw[10:12]
        + raw[8:10],
        16,
    )

    version = f"{major}.{minor}.{revision}+{build}"
    return version


#
# MARK: - Main
#


def main():
    firmware_version = getFirmwareVersion()
    utils.addToEnvironement(f"FIRMWARE_VERSION_FILE={firmware_version}")

    return 0


if __name__ == "__main__":
    sys.exit(main())
