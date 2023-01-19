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
    description="Check filename contains version number"
)

parser.add_argument("file_path", type=str,
                    help="path of file to compare with version")
parser.add_argument("version", type=str, help="version to compare with")

args = parser.parse_args()


#
# MARK: - Main
#


def main():
    are_version_same: bool = args.version in args.file_path

    if "Firmware" in args.file_path:
        utils.addToEnvironement(
            f"FIRMWARE_VERSION_FILE_ARE_SAME={are_version_same}")
    elif "LekaOS" in args.file_path:
        utils.addToEnvironement(f"OS_VERSION_FILE_ARE_SAME={are_version_same}")

    return 0


if __name__ == "__main__":
    sys.exit(main())
