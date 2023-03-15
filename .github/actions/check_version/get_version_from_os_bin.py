#!/usr/bin/env python3

# Leka - LekaOS
# Copyright 2023 APF France handicap
# SPDX-License-Identifier: Apache-2.0

import sys
import argparse
import struct
import utils

#
# MARK: - argparse
#


parser = argparse.ArgumentParser(description="Get version from os bin file")

parser.add_argument("os_bin_path", type=str, help="path of os bin file")

args = parser.parse_args()


#
# MARK: - Functions
#


def getOSVersion() -> str:
    raw = utils.getDataFromFile(args.os_bin_path, mode="rb")[20:28]

    major = raw[0]
    minor = raw[1]
    revision = struct.unpack("h", raw[2:4])[0]
    build = struct.unpack("I", raw[4:8])[0]

    version = f"{major}.{minor}.{revision}+{build}"
    return version


#
# MARK: - Main
#


def main():
    os_version = getOSVersion()
    utils.addToEnvironement(f"OS_VERSION_FILE={os_version}")

    return 0


if __name__ == "__main__":
    sys.exit(main())
