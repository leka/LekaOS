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
    description="Check os & firmware version are equal to os_version config"
)

parser.add_argument(
    "version_config_path", type=str, help="version config filepath"
)
parser.add_argument("firmware_version", type=str, help="firmware_version")
parser.add_argument("os_version", type=str, help="os_version")

args = parser.parse_args()


#
# MARK: - Functions
#


def checkVersionsAreEquals(config_version: str, actual_version: str) -> bool:
    return config_version in actual_version


#
# MARK: - Main
#


def main():
    config_version = utils.getDataFromFile(
        args.version_config_path).replace("\n", "")

    is_firmware_version_correct = checkVersionsAreEquals(
        config_version, args.firmware_version)

    is_os_version_correct = checkVersionsAreEquals(
        config_version, args.os_version)

    utils.addToEnvironement(
        f"FIRMWARE_VERSION_ARE_SAME={is_firmware_version_correct}")
    utils.addToEnvironement(f"OS_VERSION_ARE_SAME={is_os_version_correct}")

    return 0


if __name__ == "__main__":
    sys.exit(main())
